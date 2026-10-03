/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: acq32_mmap.c,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: 
 * allocate kernel buffers in response to mmap().
 * setup the vma such that the buffers which are non-contiguous in physical 
 * memory, form one contigous region of virtual memory in the vma, useable
 * by the calling application.
 *
 * WARNING: it was difficult to get this to work. Applications MUST follow
 * the following sequence or results are unpredictable
 * open() mmap() { process } munmap() close()
 *
 * $Id: acq32_mmap.c,v 1.15.4.5 2005/01/19 21:30:21 pgm Exp $
 * $Log: acq32_mmap.c,v $
 * Revision 1.15.4.5  2005/01/19 21:30:21  pgm
 * compile changes for 2.4.21-EL
 *
 * Revision 1.15.4.4  2004/10/21 09:10:56  pgm
 * clean compile, no warnings
 *
 * Revision 1.15.4.3  2003/10/27 20:38:57  pgm
 * fetch works to 2MS
 *
 * Revision 1.15.4.2  2003/10/26 21:19:47  pgm
 * dumdma emulation work - 1MS limit
 *
 * Revision 1.15.4.1  2003/09/16 11:03:20  pgm
 * compiles clean on 2.4.20 (RH9x)
 *
 * Revision 1.15  2002/06/16 14:05:59  pgm
 * RH62 back mods
 *
 * Revision 1.14  2002/07/28 17:02:30  pgm
 * add debugs marker to nopage
 *
 * Revision 1.13  2002/07/19 18:39:37  pgm
 * mod DUMDMA to avoid message overrun
 *
 * Revision 1.12  2002/07/02 18:25:13  pgm
 * acq32_mmap works on RH7.3
 *
 * Revision 1.11  2002/03/12 12:12:40  pgm
 * clean format rools OK
 *
 * Revision 1.10  2001/12/19 21:41:52  pgm
 * *** empty log message ***
 *
 * Revision 1.9  2001/12/19 21:32:49  pgm
 * llcontrol works on Linux 2.4 - SHIP IT QUICK
 *
 * Revision 1.8  2001/12/19 10:09:40  pgm
 * Builds on RH6.2 again
 *
 * Revision 1.7  2001/12/19 09:56:07  pgm
 * Linux 2.4 first cut runs, mapping dodgy
 *
 * Revision 1.6  2001/05/27 18:56:49  pgm
 * *** empty log message ***
 *
 * Revision 1.5  2001/05/27 14:42:54  pgm
 * nopage increments count of first page in buffer ONLY -
 * seems to work ...
 *
 * Revision 1.4  2001/03/31 20:58:24  pgm
 * DUMDMA works in slow mo
 *
 * Revision 1.3  2001/03/30 20:17:01  pgm
 * UDMA first cut working
 *
 * Revision 1.2  2001/03/25 17:28:40  pgm
 * i2o commands work
 *
 * Revision 1.1  2001/03/23 19:44:00  pgm
 * mmap() as kernel malloc to user mapping work
 *
 * Revision 1.2  2000/12/28 11:03:00  pgm
 * *** empty log message ***
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/

#include "local.h"

#ifndef __KERNEL__
#  define __KERNEL__
#endif

#include <linux/module.h>
#include <linux/mm.h>

#include <linux/kernel.h> /* printk() */
#ifdef LINUX_NEW_PCI
#include <linux/slab.h>
#else
#include <linux/malloc.h> /* kmalloc() */
#endif

#include <linux/fs.h>     /* everything... */
#include <linux/errno.h>  /* error codes */
#include <linux/types.h>  /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>        /* O_ACCMODE */
#include <linux/pci.h>
#include <linux/sched.h>

#include <asm/io.h>       /* ioremap()         */
#include <asm/uaccess.h>  /* VERIFY_READ|WRITE */
#include <asm/system.h>   /* cli(), *_flags */
#include <asm/segment.h>  /* memcpy and such */

#define DEBUG_MALLOC

#include "acq32.h"        /* local definitions */
#include "acq32ioctl.h"   /* APP interface defs */

#include "platform.h"

#include "arm_modules.h"


// VMA------------------------------------------------------------------VMA--//

/*
 * define a vma object that suits us.
 * Each buffer is one PAGE_ORDER allocation: physically contiguous because
 * the board DMAs into it, but not a compound page, so only its first page
 * carries a reference count.  The buffers are therefore mapped with
 * remap_pfn_range() at mmap() time: the VM then never takes or drops
 * references on these pages, and acq32_vma_release() frees each buffer
 * exactly as it was allocated.
 */
#ifdef LINUX_NEW_PCI
#define PDEBUG_PAGE( lvl, page ) \
    PDEBUGL(lvl)( "mapping page:%p count:%d\n", page, page_count( page ) )
#else
#define PDEBUG_MAPPING( lvl, map_nr ) \
    PDEBUGL(lvl)( "mem_map[%ld] %p flags:0x%08lx count:%d n:%p p:%p\n", \
        map_nr, &mem_map[map_nr], mem_map[map_nr].flags, \
	    atomic_read(&mem_map[map_nr].count),\
	    mem_map[map_nr].prev, mem_map[map_nr].next ) 
#endif	    
	    

static void storeFileInVma( 
    struct vm_area_struct* vma, 
    struct file* filp )
{
#ifdef LINUX_NEW_PCI
    vma->vm_private_data = PD(filp);
#else
    vma->vm_file = filp;    
#endif
}
static struct Acq32Path* getPathFromVma(
    struct vm_area_struct* vma
    )
{
#ifdef LINUX_NEW_PCI
    return (struct Acq32Path*)vma->vm_private_data;
#else
    return PD( vma->vm_file );
#endif
}
static int getChannelMapIndex(
    struct Acq32Path* path, 
    struct vm_area_struct* vma 
    )
{
    int imap;
    
    for ( imap = 0; imap != MAX_CONCURRENT_CHANNEL_MAPPINGS; ++imap ) { 
        if ( path->channel_maps[imap] && path->channel_maps[imap]->vma == vma ){
	    return imap;
	}
    }
    
    return -1;
} 


static void acq32_vma_open(struct vm_area_struct *vma)
{
    struct Acq32Device* device = getPathFromVma( vma )->device;
    
    device->vmas++;
    PDEBUGL(2)( "acq32_vma_open( %p ) device %p\n", vma, device );
}


static void acq32_vma_release(struct vm_area_struct* vma )
// free the kernel memory associated with this vma
{
//#define FN "acq32_vma_release() "
    struct Acq32Path* path = getPathFromVma( vma );
    struct Acq32Device* device = path->device;

    int imap = getChannelMapIndex( path, vma );
    
    PDEBUGL(2)(  "( %p ) device %p imap %d\n", vma, device, imap );

    if ( imap != - 1 ){ 
        ChannelMapping* cm = path->channel_maps[imap];       
        int ibuf;

        for ( ibuf = 0; ibuf != MAX_BUFFERS_IN_CHANNEL &&
		  cm->buffers[ibuf] != (void*)0;     ++ibuf ){
#ifdef LINUX_NEW_PCI
            PDEBUG_PAGE( 5, virt_to_page( cm->buffers[ibuf] ) );
#else

	    int count = atomic_read(
		&mem_map[MAP_NR( cm->buffers[ibuf])].count);

            if ( count != 1 ){
                atomic_set( &mem_map[MAP_NR(cm->buffers[ibuf])].count, 1 );
	
                PDEBUGL(1)( " BUFFER:%p %4lx count %d fixed to %d\n", 
			    cm->buffers[ibuf],
			    MAP_NR( cm->buffers[ibuf]),
			    count, 
			    atomic_read( 
				&mem_map[MAP_NR( cm->buffers[ibuf])].count) 
		    );
            }
            PDEBUG_MAPPING( 5, MAP_NR( cm->buffers[ibuf] ) );
            PDEBUG_MAPPING( 5, MAP_NR( cm->buffers[ibuf] )+1 );
#endif
            FREE_PAGES((unsigned long)cm->buffers[ibuf], PAGE_ORDER);
        }

        path->channel_maps[imap] = (void*)0;    
        KFREE( cm );
    }else{
        PDEBUG( "ERROR: acq32_vma_release - mapping not found ?? !!\n" );
    }
	
    device->vmas--;
//#undef FN
}

/*
 * Every page of the vma is mapped by acq32_mmap_channel(), so a fault here
 * is an access the mapping does not cover.  Without a fault handler the VM
 * would quietly back the address with an anonymous page.
 */
static int acq32_vma_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
	PDEBUGL(4)("%p 0x%08lx\n", vma, (unsigned long)vmf->virtual_address);

	return VM_FAULT_SIGBUS;
}



struct vm_operations_struct acq32_channel_vm_ops = {
	.open  = acq32_vma_open,
	.close = acq32_vma_release,
	.fault = acq32_vma_fault,
};

// VMA---------------------------------------------------------------------VMA--//         

static int getFirstFreeMmapBuffer( struct Acq32Path* path ) {
    int ibuf;
    
    for ( ibuf = 0; ibuf != MAX_CONCURRENT_CHANNEL_MAPPINGS; ++ibuf ) {
        if ( path->channel_maps[ibuf] == (void*)0 ){
	    path->channel_maps[ibuf] = 
		KMALLOC( sizeof(ChannelMapping), GFP_KERNEL );
	    if ( path->channel_maps[ibuf] == (void*)0 ){
		PDEBUG( "ERROR: failed to allocate channel map\n" );
		return -1;
	    }else{
		memset( path->channel_maps[ibuf], 0, sizeof(ChannelMapping) );
		return ibuf;
	    }
	}
    }

    PDEBUG( "ERROR: too many channel mappings" );	
    return -1;    
}

static void testFill( ChannelMapping* cm, int ichan ) 
// put a channel ident into the buffer
{
//#define FN "testFill() "
    int ibuf;

    PDEBUGL(2)( "channel %d\n", ichan );
    
    for ( ibuf = 0; ibuf != MAX_BUFFERS_IN_CHANNEL; ++ibuf ) {
        if ( cm->buffers[ibuf] ) {

	    PDEBUGL(4)( "%d %p %d %ld\n", 
			ibuf, cm->buffers[ibuf], ichan, DMABUF_SIZE );

            memset( cm->buffers[ibuf], ichan, DMABUF_SIZE );
            *(unsigned*)cm->buffers[ibuf] = ibuf*DMABUF_SIZE/2;
	}else{
	    break;
	}
    }

    PDUMPL( 5, , cm->buffers[0] );
//#undef FN
}

static void markPages( ChannelMapping* cm, unsigned long vsize )
// label the first page of each buffer (every page if acq32_fill_vma > 2)
{
    unsigned long offset;

    for ( offset = 0; offset < vsize; offset += PAGE_SIZE ){
        int ibuf = offset / DMABUF_SIZE;
        unsigned long page_offset_in_buffer = offset - ibuf * DMABUF_SIZE;
        char* page_addr = cm->buffers[ibuf] + page_offset_in_buffer;

        if ( page_offset_in_buffer == 0 || acq32_fill_vma > 2 ){
            sprintf( page_addr, "PAGE 0x%08lx offset 0x%08lx ibuf%d\n",
                     (unsigned long)page_addr, offset, ibuf );
        }
    }
}

int acq32_copyToChannelMap(
	ChannelMapping* cm, 
	void *src,
	unsigned offset,
	unsigned length )
{
	char *pbuf;

	while (length >0) {
		unsigned ibuf = offset/DMABUF_SIZE;
		unsigned obuf = offset%DMABUF_SIZE;
		unsigned ncpy = min(length, (unsigned)(DMABUF_SIZE-obuf));

		if (ibuf > MAX_BUFFERS_IN_CHANNEL ||
		    (pbuf = cm->buffers[ibuf]) == 0){
			return -ENOMEM;
		}else{
			memcpy(pbuf+obuf, src, ncpy);
			offset += ncpy;
			length -= ncpy;
		}
	}

	return 0;
}

/*
 * acq32_mmap_channel() - this is the module entry point
 */
 
int acq32_mmap_channel( struct file* filp, struct vm_area_struct* vma )
// allocate a series of 128k buffers and map them into the vma
{
//#define FN "acq32_mmap_channel() "
    struct Acq32Path* path = PD(filp);
    int imap = getFirstFreeMmapBuffer( path );
    unsigned long vsize     = vma->vm_end - vma->vm_start;
    unsigned long allocated = 0;
    int ibuf;    
    ChannelMapping* cm;
    int rc = 0;

    PDEBUGL(2)( "vma:%p channel:%d\n", vma, CHANNEL( path->minor ) );
    
    if ( imap == -1 ){
        return -ENODEV;
    }
    cm = path->channel_maps[imap];
    cm->vma = vma;
    vma->vm_ops = &acq32_channel_vm_ops;
    storeFileInVma( vma, filp );
#ifdef LINUX_NEW_PCI
    /* The buffers belong to this vma alone: a fork()ed copy would keep
     * mapping them after acq32_vma_release() has freed them. */
    vma->vm_flags |= VM_DONTCOPY;
#endif

    for (ibuf=allocated=0; allocated<vsize; ++ibuf, allocated += DMABUF_SIZE) {
	if ( ibuf >= MAX_BUFFERS_IN_CHANNEL ){
	    PDEBUG( "ERROR: ran out of buffers [%d]. ret ENODEV\n", ibuf );
	    rc = -ENODEV;
	    break;
	}
	/* GFP_DMA32: the board is a 32 bit bus master and is handed each
	 * buffer's address in the 32 bit HostRequestDataRecord.pci field. */
	cm->buffers[ibuf] = (void*)__GET_FREE_PAGES(
		GFP_KERNEL | GFP_DMA32, PAGE_ORDER );

	if ( cm->buffers[ibuf] == (void*)0 ){
	    PDEBUG( "ERROR __get_free_pages() failed Should set ENOMEM\n" );
	    rc = -ENOMEM;
	    break;
	}else{
#ifdef LINUX_NEW_PCI
	    unsigned long len = min_t( unsigned long,
				       vsize - allocated, DMABUF_SIZE );

	    if ( remap_pfn_range(
		     vma, vma->vm_start + allocated,
		     page_to_pfn( virt_to_page( cm->buffers[ibuf] ) ),
		     len, vma->vm_page_prot ) ){
		PDEBUG( "ERROR remap_pfn_range() failed\n" );
		rc = -EAGAIN;
		break;
	    }
#else
	    unsigned long map_nr = MAP_NR( cm->buffers[ibuf] );
	    int ipage;
		    
	    PDEBUG_MAPPING( 5, map_nr );
		    
	    for ( ipage = 1, map_nr += 1; 
                  ipage != 1<<PAGE_ORDER; ++ipage, ++map_nr ){
		PDEBUG_MAPPING( 6, map_nr );
	    }
#endif
	}
    }    

    acq32_vma_open( vma );

    if ( rc != 0 ){
        // unfortunately the vma is NFG so we have to close down again ..
	acq32_vma_release( vma );
    }else{
        if ( acq32_fill_vma ){
            testFill( cm, CHANNEL( path->minor ) );
	}
        if ( acq32_fill_vma > 1 ){
            markPages( cm, vsize );
	}
    }

    return rc;   
//#undef FN
}


