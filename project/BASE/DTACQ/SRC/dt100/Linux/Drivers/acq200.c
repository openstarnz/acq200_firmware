/* ------------------------------------------------------------------------- */
/* acq200.c driver toplevel for acq200 peripheral                            */
/* ------------------------------------------------------------------------- */
/*   Copyright (C) 2003 Peter Milne, D-TACQ Solutions Ltd
 *                      <Peter dot Milne at D hyphen TACQ dot com>
                                                                               
    This program is free software; you can redistribute it and/or modify
    it under the terms of Version 2 of the GNU General Public License
    as published by the Free Software Foundation;
                                                                               
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
                                                                               
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                */
/* ------------------------------------------------------------------------- */

struct task_struct;

#include <linux/kernel.h>
//#include <linux/interrupt.h>
#include <linux/pci.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/init.h>
#include <linux/timex.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>


#include <linux/proc_fs.h>

#include <asm/uaccess.h>  /* VERIFY_READ|WRITE */

#ifndef EXPORT_SYMTAB
#define EXPORT_SYMTAB
#include <linux/module.h>
#endif

#include "local.h"

#include "acq32busprot.h"

#include "acq200.h"
#include "acq200_hml.h"
#include "acq200_debug.h"

#include "acq32.h"        /* local definitions */





/* WORKTODO - need a more sophisticated way of defining a WAVEGEN
 * suggest PCI SUBVENDOR or somesuch
 */
#define WAVEGEN 1

#define ABS_MAX_DEV 32

char acq200_bridge_driver_name[] = "acq200_bridge";
char acq200_bridge_driver_string[] = "D-TACQ bridge device";
char acq200_bridge_driver_version[] = "$Revision: 1.1.4.36 $ B1006";
char acq200_bridge_copyright[] = "Copyright (c) 2005 D-TACQ Solutions Ltd";






#define OUTBOUND 1
#define INBOUND  -1
#define USE_PREVIOUS 0

static struct {
	int ndevs;
	struct DevGlob* devices[ABS_MAX_DEV];
} GLOB;


#ifdef PGMCOMOUT
#define CHAN(file) CHANNEL((file)->f_dentry->d_inode->i_rdev)
#else
#define CHAN(file) ((file)->f_dentry->d_inode->i_rdev&0x00ff)
#endif

static Message *build_hrd_message(
	struct file *file, 
	unsigned pa,
	unsigned start,
	unsigned length,
	unsigned stride,
	int outbound,
	unsigned short cflags
	)
{
	Message *message = (Message*)LBUF(file);
	switch(outbound){
	default:
	case 0:
		message->header.type = outbound;
		break;
	case 1:
	case -1:
		message->header.type = 
			MAKE_MT_COMMAND(outbound>0?
				MTC_HOST_REQUEST_DATA_ACQBOUND:
				MTC_HOST_REQUEST_DATA_HOSTBOUND);
	}
	message->header.length = 2*HRDR_SZ;

	message->payload.hrdr[0].pci = pa;
	message->payload.hrdr[0].start = start;
	message->payload.hrdr[0].nsamples = length;
	message->payload.hrdr[0].stride = stride;
	message->payload.hrdr[0].chan = CHAN(file)|cflags;
	
	memset( &message->payload.hrdr[1], 0, HRDR_SZ);

	AP(file)->last_direction_hack = outbound;

	return message;
}

static Message *build_outgoing_raw_message(
	struct file *file, 
	unsigned pa,
	unsigned start_byte,
	unsigned nbytes,
	unsigned short cflags
	)
{
	return build_hrd_message(
		file, pa, start_byte/2, nbytes/2, 1, OUTBOUND, cflags);
}

static Message *build_incoming_raw_message(
	struct file *file, 
	unsigned pa,
	unsigned start_byte,
	unsigned nbytes,
	unsigned short cflags
	)
{
	return build_hrd_message(
		file, pa, start_byte/2, nbytes/2, 1, INBOUND, cflags);
}


static void buildRch(
	struct ReturnCommandHandler *rch,
	void (*handler)( 
		struct ReturnCommandHandler* this, 
		struct MESSAGE* messsage ),
	struct Acq32Path *path,
	void *clidata )
{
	rch->handler = handler;
	rch->path = path;
	rch->command_id = MAKE_ID_DTACQ(path->device->m_dpd.i2o_last_id_out+1);
	rch->clidata = clidata;
}


int acq200_bridge_register_device(struct DevGlob* dg)
{
	int idx;
	GLOB.devices[idx = GLOB.ndevs] = dg;
	GLOB.ndevs++;
	return idx;
}
struct DevGlob *acq200_get_device( dev_t i_rdev )
{
	int major = MAJOR( i_rdev );
	int isearch;
    
	for ( isearch = 0; isearch != GLOB.ndevs; ++isearch ){
		struct DevGlob* device = GLOB.devices[isearch];
        
		if ( device->bridge_dev.major == major ||
		     device->wave_dev.major   == major    ){
			return device;
		}
	}
	return 0;
}


struct Acq32Path *acq200_makePathDescriptor(dev_t i_rdev)
{
	struct DevGlob *dg = acq200_get_device(i_rdev);
	
	if (dg != 0){
		struct Acq32Path *path = 
			acq32_makePathDescriptor(i_rdev, dg->acq32_device);

		LEAK("about to KMALLOC acq200_path");
		if ( path != 0 ){
			struct Acq200Path *acq200_path =
				KMALLOC(sizeof(struct Acq200Path), GFP_KERNEL);

			
			if ( acq200_path != 0 ){
				LEAK("acq200_path %p", acq200_path);
				memset(
					acq200_path, 0, 
					sizeof(struct Acq200Path));
				acq200_path->dg = dg;
				path->subclass = acq200_path;
				LEAK("subclass %p", path);
				acq32_request_irq(path);
				LEAK("request_irq done");
				return path;
			}
		}
	}

	return 0;
}


void acq200_freePathDescriptor(struct Acq32Path *path)
{
	LEAK("");
	
	KFREE(path->subclass);
	acq32_free_irq(path);
	acq32_freePathDescriptor(path);
}

int acq200_bridge_open (
	struct inode *inode, struct file *file
)
{
	LEAK("");
	ACQ200_ON_OPEN(inode, file);
	LEAK("");
	return 0;
}

static void i2o_handleLastMessage(
	struct ReturnCommandHandler* self,
	struct MESSAGE* response
	)
{
	self->clidata = (void*)1;
	wake_up_interruptible(&self->path->return_waitq);
	rchRemove(self);
}


int acq200_bridge_release_generic (
	struct inode *inode, struct file *file, int hrd_code
	)
{
	Message *message;
	
	switch(AP(file)->last_direction_hack){
	case OUTBOUND:
		message = build_outgoing_raw_message(
			file, 0, 0, 0, hrd_code|HRD_CHAN_EOF);
		break;
	case INBOUND:
		message = build_incoming_raw_message(
			file, 0, 0, 0, hrd_code|HRD_CHAN_EOF);
		break;
	default:
		ACQ200_ON_CLOSE(file);
		BUG();                  /* ACQ200_ON_CLOSE() is return */
	}

	{
		struct Acq32Device *device = DG(file)->acq32_device;
		struct ReturnCommandHandler *rch = 
			KMALLOC(sizeof(struct ReturnCommandHandler),
				GFP_KERNEL);

		assert(rch != 0);

		buildRch(rch, i2o_handleLastMessage, PD(file), 0);
		rchAppend(device->m_dpd.first_rch, rch);	

		acq32_sendI2O_kbuf(PD(file), message);

		wait_event_interruptible(PD(file)->return_waitq, 
					 rch->clidata!=0);
		KFREE( rch );
		ACQ200_ON_CLOSE(file);
	}
}


static int acq200_bridge_release (
	struct inode *inode, struct file *file
	)
{
	return acq200_bridge_release_generic(
		inode, file, HRD_CHAN_IS_FILE );
}

int acq200_bridge_mmap( struct file* filp, struct vm_area_struct* vma )
// mmap the large contig area of mem at dg.bigbuf to vma
{
	static int first_time = 1;

	if (first_time){
		first_time = 0;
		return -1;
	}
	return remap_pfn_range(
		vma, vma->vm_start,
		pci_resource_start( DG(filp)->pci_dev, 0) >> PAGE_SHIFT,
		vma->vm_end - vma->vm_start,
		vma->vm_page_prot
		);
}



static void create_proc_entries(void)
{

}


static void delete_proc_entries(void)
{

}


static void run_bridge_mknod_helper(int major)
{

}

#define DOUBLE_TROUBLE

/*
 * WORKTODO: hopefully we DO NOT have to run this thru
 * a local kbuf.
 * if so, copy from user to LBUF, LBUF to io in 2K chunks
 */
int acq200_copy_from_io_to_user(
	struct file *file, void *ubuf, const void *iobuf, int count )
{
#ifdef DOUBLE_TROUBLE
	const char* src = iobuf;
	char* dst = ubuf;
	char* tmp = LBUF(file);
	int rv = count;
	int ncopy;

	while( count > 0 ){
		ncopy = min(count, LBUFLEN);
		memcpy_fromio(tmp, src, ncopy);
		copy_to_user(dst, tmp, ncopy);
		src += ncopy;
		dst += ncopy;
		count -= ncopy;
	}
	
	return rv;
#else
	return copy_to_user(ubuf, iobuf, count);
#endif
}

int acq200_copy_from_user_to_io(
	struct file *file, void *iobuf, const void *ubuf, int count )
{
#ifdef DOUBLE_TROUBLE
	const char* src = ubuf;
	char* dst = iobuf;
	char* tmp = LBUF(file);
	int rv = count;
	int ncopy;

	while( count > 0 ){
		ncopy = min(count, LBUFLEN);
		copy_from_user(tmp, src, ncopy);
		memcpy_toio(dst, tmp, ncopy);
		src += ncopy;
		dst += ncopy;
		count -= ncopy;
	}
	return rv;
#else
	return copy_from_user(iobuf, ubuf, count);
#endif
}


static void dumpHrd(const char* name, struct HOST_REQUEST_DATA_RECORD* hrd)
{
	dbg(1, "%10s c:%2d %x pci:%08x start:%d n:%d s:%d\n",
		       name, hrd->chan&0x3f, hrd->chan, hrd->pci, hrd->start, 
		       hrd->nsamples, hrd->stride );
}


#define DUMPHRD(var) dumpHrd(#var, var)

static void i2o_recycleMessage(
    struct ReturnCommandHandler* self,
    struct MESSAGE* response
    )
{
	dbg(1,"rch %p kbuf %p mbuf %08x", 
	    self, self->clidata, response->payload.hrdr[0].pci );
/*
 * WORKTODO: is this wise (free before data consumed => bad idea)
 */
	acq200_bb_pool_free(self->path->device, self->clidata);
	rchRemove(self);
}


static void i2o_handleReadMessageFile(
	struct ReturnCommandHandler* self,
	struct MESSAGE* response
	)
/* output nsamples as self->clidata in BYTES WORKTODO - is this a good idea? */
{
	int nsamples = response->payload.hrdr[0].nsamples;

	DUMPHRD(response->payload.hrdr);

	i2o_recycleMessage(self, response);

	dbg(2, "nsamples %d\n", nsamples);

	if (nsamples == 0xffffffff){
		self->clidata = (void*)nsamples;
	}else{
		self->clidata = (void*)(nsamples*2);
	}
	wake_up_interruptible(&self->path->return_waitq);
}

static void i2o_handleReadMessage(
	struct ReturnCommandHandler* self,
	struct MESSAGE* response
	)
/* output nsamples as self->clidata WORKTODO - is this a good idea? */
{
	int nsamples = response->payload.hrdr[0].nsamples;

	DUMPHRD(response->payload.hrdr);

	i2o_recycleMessage(self, response);

	dbg(2, "nsamples %d\n", nsamples);

	self->clidata = (void*)nsamples;

	wake_up_interruptible(&self->path->return_waitq);
}



ssize_t acq200_bridge_read_generic (
	struct file* file, char* buf, size_t count, loff_t* posp,
	int hrd_code)
{
	struct Acq32Device *device = DG(file)->acq32_device;
	struct ReturnCommandHandler* rch;
	struct IoMapping *kbuf = 
		acq200_bb_pool_alloc(device, &rch, PCI_DMA_TODEVICE);
	Message *message;

	if (kbuf == 0){
		return -ETIMEDOUT;
	}else if (kbuf->len == 0){
		return -ENOMEM;
	}else{
		count = min(count, (size_t)kbuf->len);
	}

	dbg(1, "build count %d", count );

	message = build_incoming_raw_message( 
		file, kbuf->pa, *posp, count, hrd_code);

	buildRch( rch, i2o_handleReadMessageFile, PD(file), kbuf );
	rchAppend(device->m_dpd.first_rch, rch);
	acq32_sendI2O_kbuf(PD(file), message);	

	wait_event_interruptible(PD(file)->return_waitq, rch->clidata!=kbuf);

	if (rch->clidata == (void*)0xffffffff){
		return -EFAULT;
	}else{
		count = (size_t)rch->clidata;
	}

	dbg(1,"copy to user va:%p pa:0x%08lx count:%d",
	    kbuf->va, kbuf->pa, count );

	acq200_copy_from_io_to_user(file, buf, kbuf->va, count );

	
	*posp += count;

	dbg(1, "return %d", count);
	return count;   
}

static ssize_t acq200_bridge_read (
	struct file* file, char* buf, size_t count, loff_t* posp)
{
	return acq200_bridge_read_generic(
		file, buf, count, posp,
		HRD_CHAN_IS_FILE);
}

#define SAMPLES_PER_BUFFER(ch) (0x100000/2)

#ifdef PGMCOMOUT
int acq200_fetchDataToLocalBuffer(         
	struct file *file,
	int channel, 
	short* buffer,
	int nsamples,
	int start,            /* start sample in sample set */
	int stride
	)
/*
 * new style streamed fetch
 * buffer MUST be pre mapped using mmap
 * returns actual num samples fetched
 * MESSAGE_HRDR_LEN restricts the number of HRDR records possible
 *
 * strategy for 2G: Q a bunch of kbufs. wait for return, copy across
 */
{
	struct Acq32Path* path = PD(file);
	struct Acq32Device *device = path->device;
	ChannelMapping* mapping = path->channel_maps[0];
	unsigned long vm_ubuf = (unsigned long)buffer;
	int ipbuf = 0;   // Corresponding buffer index in VMA (!= ireq)
	int ntotal = 0;
	int offset = 0;   /* offset byte in user buffer */

	if ( mapping == NULL ){
		return -EACQ32_NO_MAPPING;
	}else if ( !IN_RANGE( vm_ubuf, 
			      mapping->vma->vm_start, 
			      mapping->vma->vm_end ) ){
		return -EACQ32_OUT_OF_MAPPING_RANGE;
	}else if ( vm_ubuf+nsamples > mapping->vma->vm_end ){
		return -EACQ32_OUT_OF_MAPPING_RANGE;
	}else{
		int sample_offset_in_vma = 
			(vm_ubuf - mapping->vma->vm_start)/2;
		int buf_start_sample;

		ipbuf = sample_offset_in_vma/SAMPLES_PER_BUFFER(channel);
		buf_start_sample = ipbuf*SAMPLES_PER_BUFFER(channel);

		if (  buf_start_sample != sample_offset_in_vma ){
			int buf_delta = 
				sample_offset_in_vma - buf_start_sample;

			PDEBUGL(1)("buf_start_smpl 0x%06x buf_delta 0x%06x\n",
				     buf_start_sample, buf_delta );

			nsamples += buf_delta;
			path->return_samples = -buf_delta;
		}else{
			path->return_samples = 0;
		}
	}
	PDEBUGL(1)( 
		"vm_start 0x%08lx vm_ubuf 0x%08lx vm_end 0x%08lx ipbuf %d\n",
		mapping->vma->vm_start, vm_ubuf, mapping->vma->vm_end, ipbuf );

    /*
     * here with valid buffer request, ireq set to offset in phys buf
     * now assemble buffer requests for transmission to acq200
     */
	while( ntotal < nsamples ){
		struct ReturnCommandHandler* rch;
		struct IoMapping *kbuf = 
			acq200_bb_pool_alloc(path->device, &rch, 
					     PCI_DMA_FROMDEVICE);
		int ns = min(nsamples-ntotal, SAMPLES_PER_BUFFER(channel));
		Message *message;
		int rc;
		int ipoll;

		dbg(1, 
		    "ch %02d ntotal %d nsamples %d va %p pa 0x%08lx ns %d",
		    channel,
		    ntotal, nsamples, kbuf->va, kbuf->pa, ns );

		message = build_hrd_message(
				file, kbuf->pa, start,
				ns,
				stride,
				MAKE_MT_COMMAND(MTC_HOST_REQUEST_DATA),
				channel );

		buildRch(rch, i2o_handleReadMessage, PD(file), kbuf);
		rchAppend(device->m_dpd.first_rch, rch);

		DUMPHRD(message->payload.hrdr);

		acq32_sendI2O_kbuf(PD(file), message);
		
		wait_event_interruptible(
			PD(file)->return_waitq, rch->clidata!=kbuf);

		for (ipoll = 0; *(unsigned*)kbuf->va == 0xdeadbeef; ++ipoll){
			err("kbuf not overwritten yet");
			interruptible_sleep_on_timeout(&path->return_waitq, 1);
			if (ipoll == 3){
				err("TIMEOUT on kbuf wait");
				break;
			}
		}
		if (rch->clidata == (void*)0xffffffff){
			return -EFAULT;
		}else{
			ns = (size_t)rch->clidata;

			if (ns == 0){
				break;
			}
			dbg(1, "ns %d offset %d start %d ntotal %d",
			    ns, offset, start, ntotal);

			rc = acq32_copyToChannelMap(
				mapping, kbuf->va, offset, ns*sizeof(short));

			acq200_bb_pool_free(path->device, kbuf);

			if (rc != 0){
				err("acq200_copyToChannelMap() FAILED");
				break;
			}

			ntotal += ns;
			offset += ns * sizeof(short);
			start  += ns * stride;
		}
	}

	return ntotal;
}	
#endif /* PGMCOMOUT */

ssize_t acq200_bridge_write_generic (
	struct file *file, const char *buf, size_t count, loff_t * posp,
	int hrd_code)
// write data as if from debug port
{	
	struct Acq32Device *device = DG(file)->acq32_device;
	struct ReturnCommandHandler *rch;
	struct IoMapping *kbuf = 
		acq200_bb_pool_alloc(device, &rch, PCI_DMA_TODEVICE);
	Message *message;

	if (kbuf == 0) return -ETIMEDOUT;

	count = min(count, (size_t)kbuf->len);

	if (count == 0) return -ENOMEM;


	acq200_copy_from_user_to_io(file, kbuf->va, buf, count);
	
	message = build_outgoing_raw_message( 
		file, kbuf->pa, *posp, count, hrd_code);

	buildRch(rch, i2o_recycleMessage, PD(file), kbuf );

	dbg(1,"rch %p kbuf %p mbuf %08x", 
	    rch, rch->clidata, message->payload.hrdr[0].pci );

	rchAppend(device->m_dpd.first_rch, rch);
	acq32_sendI2O_kbuf(PD(file), message);

/*
 * and then? - WORKTODO: need to recycle kbuf
 */	
	*posp += count;

	return count;   
}

static ssize_t acq200_bridge_write (
	struct file *file, const char *buf, size_t count, loff_t * posp)
// write data as if from debug port
{	
	dbg(1, "count %d", (int)count);

	return acq200_bridge_write_generic (
		file, buf, count, posp,
		HRD_CHAN_IS_FILE );
}

extern int verify_ioctl( struct inode *inode, struct file *filp,
                         unsigned int cmd, unsigned long arg);
extern int acq32_ioctl (struct inode *inode, struct file *filp,
			unsigned int cmd, unsigned long arg);

int acq200_bridge_ioctl (struct inode *inode, struct file *filp,
                        unsigned int cmd, unsigned long arg)
{
    int rc = verify_ioctl( inode, filp, cmd, arg );
    
    if ( rc != 0 ) return rc;
    
    switch(cmd) {
    case ACQ32_IOREAD_LOCALBUF:
#ifdef PGMCOMOUT
        
        PDEBUGL(2)( "ioctl ACQ32_IOREAD_LOCALBUF\n" );
                        
        if ( IN_RANGE( CHANNEL(filp->f_dentry->d_inode->i_rdev), 0, 31 ) ){

            struct READ_LOCALBUF_DESCR descr;
                
            copy_from_user( &descr, (void*)arg, sizeof(descr) );
                
            return acq200_fetchDataToLocalBuffer( 
                filp,
                CHANNEL(filp->f_dentry->d_inode->i_rdev),
                descr.buffer,
                descr.nsamples,
                descr.istart,
                descr.istride
                );
        }else{
            return -EINVAL;
        }
#else
	return -EINVAL;
#endif /* PGMCOMOUT */
    default:
        return acq32_ioctl( inode, filp, cmd, arg );
    }
}

static int __devinit
acq200_bridge_driver_init( struct DevGlob *dg )
{
	static struct file_operations bridge_fops = {
		.owner = THIS_MODULE,
		.open = acq200_bridge_open,
		.read = acq200_bridge_read,
		.write = acq200_bridge_write,
		.release = acq200_bridge_release,
		.ioctl = acq200_bridge_ioctl,
		.mmap = acq200_bridge_mmap
	};
	int rc;

	sprintf( dg->bridge_dev.drv_name, "acq200-bridge.%d", dg->idx );

	rc = register_chrdev(dg->bridge_dev.major = 0, 
			     dg->bridge_dev.drv_name, 
			     &bridge_fops);

	if (rc < 0){
		err( "can't get major %d", dg->bridge_dev.major );
		return rc;
	}else{
		dg->bridge_dev.major = rc;
		info( "device major set %d\n", dg->bridge_dev.major );
	}
	
	run_bridge_mknod_helper( dg->bridge_dev.major );
	create_proc_entries();
	if ( WAVEGEN ){
		wavegen_driver_init(dg);
	}
	return 0;
} 


static inline int toupper( char c )
/* including ctype.h appears to be bad idea! */
{
	if ( c >= 'a' && c <= 'z' ){
		c = c - 'a' + 'A';
	}
	return c;
}


static int acq200_getImagesDef(
    struct Acq32Device* device,
    struct Acq32ImagesDef* id
    )
{
	static struct Acq32ImagesDef _id[] = {
		{"ACQxxx", "00", "DXXXXX", "xxxxxxxxx", "xxxxxxx", "xxxxxx"},
		{"ACQ1xx", "00", "DXXXXX", "xxxxxxxxx", "xxxxxxx", "xxxxxx"},
		{"ACQ2xx", "00", "DXXXXX", "xxxxxxxxx", "xxxxxxx", "xxxxxx"},
		{"WAV232", "32", "D22222", "wavlinux", "wav-lca", "251203"},
		{"ACQ216", "16", "D22222", "arm-linux", "acq-lca", "251203"},
		{"ACQ196", "96", "D22222", "arm-linux", "spartan", "251204"}
	};
#define ID_ACQXXX &_id[0]
#define ID_ACQ1xx &_id[1]
#define ID_ACQ2xx &_id[2]
#define ID_WAV232 &_id[3]
#define ID_ACQ216 &_id[4]
#define ID_ACQ196 &_id[5]

	struct Acq32ImagesDef* pid = ID_ACQXXX;
	
	dbg(1, "Vendor 0x%04x Device 0x%04x SubV 0x%04x SubD 0x%04x",
	     device->p_pci->vendor,
	     device->p_pci->device,
	     device->p_pci->subsystem_vendor,
	     device->p_pci->subsystem_vendor);

	switch(device->p_pci->vendor){
	case PCI_VENDOR_ID_HINT:
		pid = ID_ACQ216;
		break;
	case PCI_VENDOR_ID_INTEL:
		switch(device->p_pci->device){
		case 0x318:
		case 0x319:
			pid = ID_ACQ196;
			break;
		}
	}

	memcpy( id, pid, sizeof(struct Acq32ImagesDef));
	dbg(1, "%s %s %s", id->model, id->channels, id->serialnum );
	return 0;  /* OKOKOK */
}





static void _acq200_set_mailbox( 
	struct Acq32Device* dev, int mailbox, u32 value )
/* do NOT call directly, always use device->set_mailbox() */
{
    ASSERT( IN_RANGE( mailbox, 0, 3 ) );

    {
	char buf[80];

	sprintf( buf, "%d := 0x%08x\n", mailbox, value );
	PRINTBP( ">pci", buf );
    }

    writel( value, CSR( dev, ACQ200_MAILBOX_0)+mailbox );    
}



static void acq200_dev_i2o_return_mfa(struct Acq32Device* device, unsigned mfa)
{
	dbg(1,"mfa %08x", mfa);
	writel(mfa, CSR(device, ACQ200_OQP));
}

static void* acq200_dev_i2o_mfa2va(struct Acq32Device *device, u32 mfa)
{
	return (void*)((unsigned)device->dmabuf.va+(mfa&ACQ200_PCIWINMSK));
}
static void acq200_incoming_message_isr(struct Acq32Device *device)
{
	u32 mfa = readl(CSR(device, ACQ200_OQP));

	dbg(1, "mfa %08x", mfa );
	/* get out of jail free trap */
	if ( mfa == 0 || mfa == 0xffffffff){
		u32 mask = readl(CSR(device, ACQ200_OIMR));
		mask |= ACQ200_OISR_OPQI;
		writel(mask, CSR(device, ACQ200_OIMR));
		err(" bad mfa 0x%08x masking ints to get back in game",
		    mfa);
		return;
	}

	acq32_incoming_i2o_isr(device, mfa);
}

static irqreturn_t acq200_isr(int irq, void* dev_id)
{
	static int bad_report;
	struct Acq32Device* device = (struct Acq32Device*)dev_id;
	u32 status = readl(CSR(device, ACQ200_OISR));
#define OIMR 3 	 /* OIMR bits not interesting, and masked so not int */

	if ((status & ~OIMR) == 0){
		return IRQ_NONE;
	}

	if (status&ACQ200_OISR_OPQI){
		dbg(1, "irq %d M:%d %s",irq,device->major,"message");
		acq200_incoming_message_isr(device);
	}else if (status&ACQ200_OISR_ODI){
		u32 doorbell = readl(CSR(device, ACQ200_ODR));

		dbg(1, "irq %d M:%d %s 0x%08x",
		    irq, device->major, "doorbell", doorbell);

		acq32_devGetStatus( device, NULL );
		writel(doorbell, CSR(device, ACQ200_ODR));
		acq32_doorbell_isr(device, doorbell);
	}else{
		dbg(1, "irq %d M:%d %s",irq,device->major,"bad");
		if ((bad_report&0xffff) == 0){
			err( "IRQ %d don't like this status 0x%08x",
			     irq, status );
		}
		writel(status,CSR(device, ACQ200_OISR));
		bad_report++;
		return IRQ_HANDLED;
	}

	bad_report = 0;
	return IRQ_HANDLED;
}


#include <linux/delay.h>

void _acq200_get_mailbox( 
    struct Acq32Device* dev, 
    int mailbox, 
    unsigned* pvalue )
/* do NOT call directly, always use device->get_mailbox() */
{
    ASSERT( IN_RANGE( mailbox, 0, 3 ) );
    ASSERT( pvalue != 0 );
    ASSERT( CSR(dev, 0x10) != 0 );

    *pvalue = readl( CSR( dev, 0x10)+mailbox );    

    {
	char buf[80];
	sprintf( buf, "%d == 0x%08x\n", mailbox, *pvalue );
	PRINTBP( "<pci", buf );
    }
}

#define PCI_BA_CSR  0 /* surely there's an official def ?? WORKTODO */
#define CSR_SIZE    0x80

static int acq32_makeIoMapping( 
	struct Acq32Device* device, int bar
	)
// return 0 on success
{
#define BA_MASK PCI_BASE_ADDRESS_MEM_MASK
	struct IoMapping* pim = &device->csr;
	const char* ident = "csr";    
	int rc = 0;

	sprintf( pim->name, "acq200.%d.%s", 
		 acq32_get_board_from_device( device ), ident );

	pim->pa = pci_resource_start(device->p_pci, bar)&BA_MASK;
	pim->len = CSR_SIZE;
    
	dbg(1, "request_mem_region 0x%08lx %d %s\n", 
		pim->pa, pim->len, pim->name );

	if ( request_mem_region( pim->pa, pim->len, pim->name ) != 0 ){
		pim->va = ioremap_nocache( pim->pa, pim->len );
	}else{
		dbg( 1, "request_mem_region( 0x%08lx, %d, %s ) failed\n",
			    pim->pa, pim->len, pim->name );
		rc = -ENODEV;
	}

	return rc;
}


static int map_target_pci_memory( struct Acq32Device* device )
{
    int rc = 0;                /* assume success until fail */
    
    if ( (rc = acq32_makeIoMapping( device, PCI_BA_CSR )) != 0 ){
        return rc;
    }
    device->rom.va  = device->ram.va = (void*)0xdeadbeef;


    PDEBUGL(2)(" %4s p 0x%08lx v %p\n", "csr",device->csr.pa,device->csr.va);
    return rc;
}

static int acq200_coreDevInit(struct Acq32Device *device)
{
#define BBLEN ACQ200_PCIWINSZ
#define name "name-goes-here"

	int rc;

        if ( acq32_big_buf_base ){
		rc = acq200_bb_alloc( &device->bigbuf.bb, BBLEN, 
				      device->devname );
		ASSERT(rc==0);
		acq200_bb_pool_create(device);
	}

	map_target_pci_memory(device);
	rc = acq32_genericCoreDevInit(device);

/* acq32 i2o buffers are acq32-side, but acq200 are host side,
 * this is for emulation.
 */
	device->ram.va = device->bigbuf.bb.va;

/*
 * acq200 MUST use bigbuf as the dma target 
 */
	device->dmabuf.va = device->bigbuf.bb.va;	
	device->dmabuf.pa = device->bigbuf.bb.pa;
	device->dmabuf.len = device->bigbuf.bb.len;

	return rc;

#undef name
#undef BBLEN
}


/* @@todo - select on subvendor as well as vendor ... */
#define IS_ACQ196(ent) ((ent)->vendor == PCI_VENDOR_ID_INTEL)

static int acq196_SetChannelMaskAll( 
    struct Acq32Path* path, 
    char* smask, 
    int boost 
)
/* smask is a string of ascii '0' - disable and '1' enable */
{
    SlaveIterState sis = {};
    struct Acq32Device* device;
    int finished = 0;

    while ( (device = acq32_getNextSlave( path, &sis )) != NULL ){

        unsigned* pmask = &device->m_dpd.channel_mask;
        int ichannel_board;

        *pmask = 0;

        for ( ichannel_board = 0; 
              ichannel_board != device->m_dpd.nchannels; 
              ++ichannel_board                             ){
            *pmask |= *smask++ == '1'? 1<<(ichannel_board/32): 0;

            if ( !*smask ){
                finished = 1;
                break;
            }
        }
        
        acq32_devSetChannelMask( device, *pmask, boost );
        if ( finished ){
            return 0;
        }      
    } 

    return -ENODEV;
}

static int acq196_devGetNumChannelsFromMask( struct Acq32Device* device )
{
    unsigned imask = 0x80000000;
    int nn = 0;

    for ( ; imask; imask >>= 1 ){
        if ( device->m_dpd.channel_mask&imask ){
            ++nn;
        }
    }

    return nn;
}

static int acq196_GetNumChannelsAll( struct Acq32Path* path )
{
    unsigned mask;
    struct Acq32Device* device = path->device;
    SlaveIterState sis = {};
    int nchannels = 0;

    while( (device = acq32_getNextSlave( path, &sis )) != NULL ){
        int ierror = acq32_devGetChannelMask( device, &mask );
   
        if ( ierror ){
            return ierror;
        }else{
            device->m_dpd.channel_mask = mask;

            nchannels += acq196_devGetNumChannelsFromMask( device ) * 32;
        }
    }

    return nchannels;
}



static void set_dev_type(
	struct Acq32Device* acq_device,
	const struct pci_device_id *ent)
{
	if (IS_ACQ196(ent)){
		acq_device->dev_type = DEV_TYPE_ACQ196;

		acq_device->p_md->SetChannelMask = acq196_SetChannelMaskAll;
		acq_device->p_md->GetNumChannels = acq196_GetNumChannelsAll;

		acq_device->m_dpd.nchannels           = 96;
		acq_device->m_dpd.channel_mask = 0x7;
	}else{
		acq_device->dev_type = DEV_TYPE_ACQ216;

		/** @@todo - how to read num channels */
		acq_device->m_dpd.nchannels           = 16;
		acq_device->m_dpd.channel_mask = 0xffff;

	}
}
static int __devinit
acq200_bridge_probe(struct pci_dev *dev, const struct pci_device_id *ent)
{
	int rc;
	struct Acq32Device* acq_device = acq32_create_device(dev);
	struct DevGlob* dg = KMALLOC(sizeof(struct DevGlob), GFP_KERNEL);

	ASSERT( acq_device != 0 );
	ASSERT( dg != 0 );

	set_dev_type(acq_device, ent);
	acq_device->getImagesDef = acq200_getImagesDef;
	acq_device->set_mailbox = _acq200_set_mailbox;
	acq_device->get_mailbox = _acq200_get_mailbox;
	acq_device->isr = acq200_isr;
	acq_device->coreDevInit = acq200_coreDevInit;
	acq_device->i2o_return_mfa = acq200_dev_i2o_return_mfa;
	acq_device->i2o_mfa2va = acq200_dev_i2o_mfa2va;
#ifdef PGMCOMOUT
	acq_device->fetchDataToLocalBuffer = acq200_fetchDataToLocalBuffer;
#endif
	memset(dg, 0, sizeof(struct DevGlob));

//	acq200_fixup_irqs(dev);

	dbg( 1,"enabling device irq was %d", dev->irq);
	rc = pci_enable_device(dev);	/* XXX check return */
	dbg( 1,"now dev->irq = %d  rc %d", dev->irq, rc);

	dg->idx = acq200_bridge_register_device(dg);

//	disable_parity_check(dev);
/*
	rc = pci_request_regions(dev, acq200_bridge_driver_name);
*/

	dg->acq32_device = acq_device;
	dg->pci_dev = dev;
/*
	if ( init_pci_resource(&dg.bridge.regs, dev, ACQ200_BRIDGE_REG_BAR) ){
		return -ENODEV;
	}
	if ( init_pci_resource(&dg.bridge.fifo, dev, ACQ200_BRIDGE_FIFO_BAR) ){
		return -ENODEV;
	}

	if (!IPC.is_bridge.requested){
		if ( request_irq( dev->irq, bridge_fifo_isr, 0, 
				  "bridge_fifo", (void*)&IPC.is_bridge ) != 0 ){
			err( "request_irq() failed for %d\n", dev->irq );
			return -ENODEV;
		}else{
			IPC.is_bridge.requested = 1;
		}
	}

	dbg( 1, "mapping done" );
*/

	return acq200_bridge_driver_init(dg);
}


static void acq200_bridge_remove (struct pci_dev *dev)
// remove DEVICE resources on device remove
{
	int idev;
	struct DevGlob *dg;

	for ( idev = 0; idev != GLOB.ndevs; ++idev ){
		info( "going down: %d", idev );
		dg = GLOB.devices[idev];

		if ( dg != 0 && dg->pci_dev == dev ){
			if ( WAVEGEN ){
				wavegen_driver_remove(dg);
			}
			MARK;
			unregister_chrdev(dg->bridge_dev.major, 
					  dg->bridge_dev.drv_name);
			MARK;
			pci_set_drvdata(dev, NULL);
			GLOB.devices[idev] = 0;
			KFREE(dg);
			return;
		}
	}

	err( "device %p not in table", dev );
}




#define PCI_VENDOR_ID_HINT 0x3388
/*
 *
 * { Vendor ID, Device ID, SubVendor ID, SubDevice ID,
 *   Class, Class Mask, String Index }
 */
static struct pci_device_id acq200_bridge_pci_tbl[] __devinitdata = {
	{PCI_VENDOR_ID_HINT, 0x0029, PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0},
	{PCI_VENDOR_ID_INTEL, 0x0318, PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0},
	{PCI_VENDOR_ID_INTEL, 0x0319, PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0},
/* some faulty boards show up thus ... */
	{0x1388, PCI_ANY_ID, PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0},
	{ }
};
static struct pci_driver acq200_bridge_driver = {
	.name     = acq200_bridge_driver_name,
	.id_table = acq200_bridge_pci_tbl,
	.probe    = acq200_bridge_probe,
	.remove   = __devexit_p(acq200_bridge_remove),
#ifdef PGMCOMOUT
	/* Power Managment Hooks */
#ifdef CONFIG_PM
	.suspend  = acq200_bridge_suspend,
	.resume   = acq200_bridge_resume
#endif
#endif
};


static int __init acq200_bridge_init( void )
{
	int rc;

	rc = pci_register_driver( &acq200_bridge_driver );
/*&&
	(rc = mk_sysfs( &acq200_bridge_driver.driver)) >= 0;
*/
	dbg(1, "pci_register_driver() returned %d", rc );
	return 0;
}


static void __exit
acq200_bridge_exit_module(void)
// Remove DRIVER resources on module exit
{
	pci_unregister_driver( &acq200_bridge_driver );
	delete_proc_entries();
}

int acq200_init_module(void)
{
	printk( "%s\n", acq200_bridge_driver_string );
	printk( "%s\n", acq200_bridge_driver_version );
	printk( "%s\n", acq200_bridge_copyright );
	return acq200_bridge_init();
}

void acq200_cleanup_module(void)
{
	acq200_bridge_exit_module();
}


void acq32_mmap_channel(void) 
{
/** pgmwashere 20050425 - solves link error. Dunno why it was there at all 
 *
 *  I really don't think this function will get called, but if it does, we
 *  will know about it!
 */
	BUG();
}
/*
 * The acq200-drv kernel module entry/exit are defined by acq32.c (compiled
 * via acq200_module.c with -DACQ200), which dispatches to
 * acq200_init_module() / acq200_cleanup_module() above.  The duplicate
 * module_init / module_exit / MODULE_LICENSE block that lived here in the
 * 2.4 source caused link-time symbol clashes on 2.6 and was redundant.
 */


