/* ------------------------------------------------------------------------- */
/* file acq32_specific.c                                                     */
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





#include "local.h"

#include <linux/module.h>

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
#include <linux/time.h>

#include <asm/io.h>       /* ioremap()         */
#include <asm/uaccess.h>  /* VERIFY_READ|WRITE */
#include <asm/system.h>   /* cli(), *_flags */
#include <asm/segment.h>  /* memcpy and such */


#include "acq32.h"        /* local definitions */
#include "acq32ioctl.h"   /* APP interface defs */
#include "acq32_fungen.h"

#include "platform.h"

#include "arm_modules.h"  /* struct image_header */

#define ROM_SIZE    0x400000  /* 4MB from EBSA manual */
#define RAM_SIZE    0x400000  /* swag */
#define CSR_SIZE    0x80      /* 21285 table 7-2 */
/*
 * indices to memory map pci->base_address[PCI_BA_XXX]
 */

#define PCI_BA_CSR    0
#define PCI_BA_CSR_IO 1           /* not used */
#define PCI_BA_SDRAM  2

#define BADMAP 0x8beef000


#include "acq200_debug.h"


static int acq32_makeIoMapping( 
    struct Acq32Device* device, int bar
    )
// return 0 on success
{
    struct IoMapping* pim = bar==PCI_BA_CSR? &device->csr: &device->ram;
    const char* ident = bar==PCI_BA_CSR? "csr": "ram";    
    int rc = 0;

    sprintf( pim->name, "acq32.%d.%s", 
             acq32_get_board_from_device( device ), ident );

    pim->pa = pci_resource_start(device->p_pci, bar)&PCI_BASE_ADDRESS_MEM_MASK;
    pim->len = bar==PCI_BA_CSR? CSR_SIZE: RAM_SIZE;

    /* The whole device's BARs are already claimed via pci_request_regions()
     * in acq32_pci_probe(); just map this one. */
    pim->va = ioremap_nocache( pim->pa, pim->len );
    if ( !pim->va ){
        dev_err(&device->p_pci->dev,
                "ioremap_nocache 0x%08lx %d %s failed\n",
                pim->pa, pim->len, pim->name);
        rc = -ENOMEM;
    }

    PDEBUG( "ioremap 0x%08lx %d %s -> %p\n",
            pim->pa, pim->len, pim->name, pim->va );
    return rc;
}



extern ssize_t linear_buffer_read( 
    struct file *filp,
    char* buf,
    size_t count,
    char* pkmem,
    int kmax
	);

ssize_t 
acq32_rom_read ( 
    struct file* filp, char* buf, size_t count, loff_t* posp 
    )
{
    return linear_buffer_read( 
        filp, buf, count, (char*)PDEV(filp)->rom.va, ROM_SIZE );

}

ssize_t 
acq32_read ( struct file* filp, char* buf, size_t count, loff_t* posp )
{
    return linear_buffer_read( 
        filp, buf, count, (char*)PDEV(filp)->ram.va, RAM_SIZE );
}


void set_rom_word_A01( struct Acq32Device* dev, unsigned value )
{
    PDEBUGL(2)( "set_rom_word_A01 %d\n", value );
    writel( value, CSR( dev, ROM_BYTE_WRITE) );
}




static unsigned long getTargetP( 
    struct inode* inode, 
    struct Acq32Device* device, 
    unsigned long offset 
    )
{
    if ( _SPECIAL( inode->i_rdev ) ){
        if ( IS_ROM( inode->i_rdev ) ){ 

            // DEC21285OffsetFixup - don't ask me, ask DEC !!

            if ( IN_RANGE( offset, 0, 0x1f ) ){
                offset += 0x20;
            }else if ( IN_RANGE( offset, 0x20, 0x3f ) ){
                offset -= 0x20;
            }else{
                ;
            }

            return (unsigned long)device->rom.va + offset;
        }else if ( DEVRAW( inode->i_rdev ) ){
            return (unsigned long)device->ram.va + offset;
        }
        // else ... CSR's left out :-(
    }
    return 0;

}

int acq32_userTargetAccessRead16( 
    struct inode* inode, 
    struct Acq32Device* dev, 
    struct ACQ32_RW_DEF* caller_arg
    )
{
    unsigned long addr = getTargetP( inode, dev, caller_arg->offset );
    
    if ( addr ){
        u16 local;

        set_rom_word_A01( dev, addr&0x2 );
        local = readw(addr&=~0x2);
        put_user( local, &caller_arg->data.w );

        PDEBUGL(4)( "Read16 readw(%lx) %x\n", addr, local );
        return 0;
    }else{
        return -ENOTTY;
    }
}

int acq32_userTargetAccessWrite16( 
    struct inode* inode, 
    struct Acq32Device* dev, 
    struct ACQ32_RW_DEF* caller_arg
    )
{
    unsigned long addr = getTargetP( inode, dev, caller_arg->offset );
    
    if ( addr ){
        u16 local;
        
        get_user( local, &caller_arg->data.w ); 
        set_rom_word_A01( dev, addr&0x2 );    
        writew( local, addr&=~0x2 ); 
        PDEBUGL(4)( "Write16 writew( %x, %lx )\n", local, addr );
        return 0;
    }else{
        return -ENOTTY;
    }
}

int acq32_userTargetAccessRead32( 
    struct inode* inode, 
    struct Acq32Device* dev, 
    struct ACQ32_RW_DEF* caller_arg 
    )
{
    unsigned long addr = getTargetP( inode, dev, caller_arg->offset );
    
    if ( addr ){
        u32 value = readl(addr);

        put_user( value, &caller_arg->data.l );

        PDEBUGL(4)( "Read32 readl(%lx) %x\n", addr, value );
        return 0;
    }else{
        return -ENOTTY;
    }
}
int acq32_userTargetAccessWrite32( 
    struct inode* inode,
    struct Acq32Device* dev, 
    struct ACQ32_RW_DEF* caller_arg 
    )
{
    unsigned long addr = getTargetP( inode, dev, caller_arg->offset );
    
    if ( addr ){
        u32 local;

        get_user( local, &caller_arg->data.l );
        writel( local, addr ); 

        PDEBUGL(4)( "Write32 writel( %x %lx )\n", local, addr );
        return 0;
    }else{
        return -ENOTTY;
    }
}


/*
 * Mailbox control
 */





    
static int acq32_map_target_pci( struct Acq32Device* device )
{
    int rc = 0;                /* assume success until fail */
    
    if ( (rc = acq32_makeIoMapping( device, PCI_BA_CSR )) != 0 ||
         (rc = acq32_makeIoMapping( device, PCI_BA_SDRAM )) != 0  ){
        return rc;
    }
    /*
     * ROM area is mapped over RAM area, and driver controls access
     * this always works, not all BIOS' map our ROM
     */    
#ifdef LINUX_NEW_PCI    
    device->p_pci->resource[PCI_ROM_RESOURCE].start = 
#else
        device->p_pci->rom_address =   
#endif    
        device->ram.pa;
    device->rom.va  = device->ram.va;


    PDEBUGL(2)(" %4s p 0x%08lx v %p\n", "csr",device->csr.pa,device->csr.va);
    PDEBUGL(2)(" %4s p 0x%08lx v %p\n", "ram",device->ram.pa,device->ram.va);
    PDEBUGL(2)(" %4s p 0x%08lx v %p\n", "rom",device->rom.pa,device->rom.va);

    return rc;
}


int acq32_mmap_rom( struct file* filp, struct vm_area_struct* vma )
/* magic formulae thanks to Rubini pg 281 */
{
    struct Acq32Device* device     = PDEV(filp);
    unsigned long vsize     = vma->vm_end - vma->vm_start;
#ifdef LINUX_NEW_PCI
    int resource           = vsize<=0x1000? PCI_BA_CSR: PCI_ROM_RESOURCE;
    unsigned long physical = pci_resource_start( device->p_pci, resource );
    unsigned long off      = vma->vm_pgoff*PAGE_SIZE;
#else        
    unsigned long off       = vma->vm_offset;
    unsigned long physical  = (long)( vsize <= 0x1000?
                                      device->p_pci->base_address[PCI_BA_CSR]&PCI_BASE_ADDRESS_IO_MASK:
                                      device->p_pci->rom_address&PCI_ROM_ADDRESS_MASK
        );
#endif

    physical += off;

#if 1
    PDEBUGL(3)( 
        "acq32_mmap_rom(): start off:%ld physical:0x%08lx vsize:0x%lx\n",
        off, physical, vsize );

    PDEBUGL(3)( "device->p_pci->rom_address 0x%08lx\n", physical );

#if defined ( __alpha__ )                        
#warning "trying dense_mem hack for ALPHA"
    physical = __pa(physical + dense_mem(physical));
    
    PDEBUGL(3)( "acq32_mmap_rom after dense_mem() hack physical now %lx\n", 
                physical );
#endif
                         
#endif

#ifndef LINUX_NEW_PCI
    if ( off & (PAGE_SIZE-1) ){
        return -ENXIO;                          /* need aligned offsets */
    }else if ( vsize > ROM_SIZE ){
        return -EINVAL;                         /* spans too high */
    }
#endif    
    return remap_pfn_range(
        vma, vma->vm_start, physical >> PAGE_SHIFT, vsize, vma->vm_page_prot
        );
}


int acq32_mmap( struct file* filp, struct vm_area_struct* vma )
{
    unsigned long vsize     = vma->vm_end - vma->vm_start;

    struct Acq32Device* device     = PDEV(filp);
    unsigned long physical  = (long)
        (vsize <= 0x1000? device->csr.pa: device->ram.pa);
    

       
    unsigned long off =
#ifdef LINUX_NEW_PCI
        vma->vm_pgoff*PAGE_SIZE;
#else
    vma->vm_offset;                                 
#endif                  
    /*
     * frig to hack/map CSR's
     */
    PDEBUGL(2)( "acq32_mmap(): "
                "start off:%ld physical was :0x%08lx rounded:0x%08lx vsize:0x%lx\n",
                off,
                physical, physical&~(PAGE_SIZE-1), vsize );

    physical &= ~(PAGE_SIZE-1);
    physical += off;

    if ( physical & (PAGE_SIZE-1) ){
        return -ENXIO;                          /* need aligned offsets */
    }else if ( vsize > ROM_SIZE ){
        return -EINVAL;                         /* spans too high */
    }else{


#if defined(__i386__)
        if (boot_cpu_data.x86 > 3)
            pgprot_val(vma->vm_page_prot) |= _PAGE_PCD;
#else
#if defined (__alpha__ )
#warning "building for alpha"
#else
#warning "What have we here ??"
#endif
#endif

        if ( remap_pfn_range(
                vma, vma->vm_start,
                physical >> PAGE_SHIFT, vsize, vma->vm_page_prot ) ){
            return -EAGAIN;
        }else{
#ifdef PGMCOMOUT // Rubini does this p277 ??
            vma->vm_inode = filp->inode;
            filp->inode->i_count++;
#endif  
            return 0;
        }
    }
}


/*
 * ISR
 */

/*
 * some 21285 defs (really belong in platform.h
 */



static inline int duff_mfa( struct Acq32Device* device, u32 mfa, char* id )
// check for duff mfa (yes, there are some
{
#ifdef PGMCOMOUT
    if ( (mfa&0x1f) != 0 ){
	PDEBUGL(0)( "%s reject because not on even boundary 0x%08x\n",
		    id, mfa );
	return 1;
    }else{
	u32 mfa_va = (u32)phys_to_virt(mfa);

	if ( (mfa_va&~(device->dmabuf.nbytes-1)) != (u32)device->dmabuf.buf ){
	    PDEBUGL(0)( "%s ERROR: discarding duff mfa 0x%08x\n", 
			id, mfa );
	    return 1;
	}else{
	    return 0;
	}
    }
#else
    return 0;
#endif
}

static void _acq32_incoming_i2o_isr( struct Acq32Device* device )
{
    unsigned mfa;
    unsigned old_mfa = 0;
    
    for( ; (mfa = readl( CSR( device, PCI_I2O_OUTBOUND_FIFO ))) !=
           PCI_I2O_INVALID_MFA;
	 old_mfa = mfa ){

	if ( old_mfa == mfa ){
		PDEBUGL(2)(  "WARNING Matching mfa 0x%08x\n", mfa );
		continue;
	}
	PDEBUGL(1)(  " 0x%08x\n", mfa );

        if ( duff_mfa( device, mfa, "isr" ) ){

/*
 * gross hack to work around I2O / SDRAM unreliability
 * the MFA is stored in mbox 2 as well ...
 * WARNING: only compatible with Build 1532 and newer
 */
	    unsigned mfa2;
         
	    device->get_mailbox( device, 2, &mfa2 );
             
	    if ( mfa2 != mfa ){
		PDEBUG(  "mfas dont match, use mbox 0x%08x 0x%08x\n",
			mfa, mfa2 );
		if ( duff_mfa( device, mfa2, "mbox" ) ){
		    PDEBUG(  "bodged mbox mfa is duff too 0x%08x\n",
			    mfa2 );
		    return;
		}
		mfa = mfa2;
	    }         
	}

	if ( old_mfa == mfa ){
	    PDEBUGL(2)(  "WARNING Matching mfa 0x%08x\n", mfa );
	    continue;
	}

	acq32_incoming_i2o_isr(device, mfa);
    }
}

/*
 * 21285 always spits out a spurious int on enable 
 * so stub it
 */

#define SIGNATURE_21285 0x0000ffff

static irqreturn_t acq32_isr( int irq, void* dev_id )
{
	struct Acq32Device* device = (struct Acq32Device*)dev_id;
	unsigned status;


	PDEBUGL(4)(  "acq32_isr device %p\n", device );

	status = readl(CSR(device, PCI_OUT_INT_STATUS));

	if ( status != 0 ) {

		PDEBUGL(3)( "\n" );

		if ( (status&PCI_OUT_INT_STATUS_OUTPOST) != 0 ){
/* OUTBOUND data packet(s) .. Q it, start BH */
			_acq32_incoming_i2o_isr( device );
		}else if ( (status&PCI_OUT_INT_STATUS_DOORBELL) != 0 ){
			unsigned doorbell = readl( CSR( device, DOORBELL ) );

			acq32_devGetStatus( device, NULL );
			writel( doorbell, CSR( device, DOORBELL) );

			if ( doorbell == SIGNATURE_21285 ){
				PDEBUGL(1)(  "SIGNATURE_21285\n" );
			}
			acq32_doorbell_isr( device, doorbell );
		}else{
			PDEBUGL(2)( " baffled by PCI_OUT_INT_STATUS 0x%08x\n", status );
			return IRQ_NONE;
		}
		return IRQ_HANDLED;
	}
	return IRQ_NONE;
}



int acq32_coreDevInit( struct Acq32Device* device )
/* returns 0 on success */
{
    acq32_map_target_pci(device);
    return acq32_genericCoreDevInit(device);
}


// ctype.h macro valid for ASCII only (I guess we can live with that)


#define isprint( cc )  ( IN_RANGE( cc, ' ', '~' ) )


void _acq32_get_mailbox( 
    struct Acq32Device* dev, 
    int mailbox, 
    unsigned* pvalue )
/* do NOT call directly, always use device->get_mailbox() */
{
    ASSERT( IN_RANGE( mailbox, 0, 3 ) );

    *pvalue = readl( CSR( dev, MAILBOX_0)+mailbox );    

    {
	char buf[80];
	sprintf( buf, "%d == 0x%08x\n", mailbox, *pvalue );
	PRINTBP( "<pci", buf );
    }
}

static void _acq32_set_mailbox( 
	struct Acq32Device* dev, int mailbox, u32 value )
/* do NOT call directly, always use device->set_mailbox() */
{
    ASSERT( IN_RANGE( mailbox, 0, 3 ) );

    {
	char buf[80];

	sprintf( buf, "%d := 0x%08x\n", mailbox, value );
	PRINTBP( ">pci", buf );
    }

    writel( value, CSR( dev, MAILBOX_0)+mailbox );    
}

static void unprintable_as_dots( char string[] )
{
    /* size_t, not int — on 64-bit, an unterminated string could let
     * the index wrap past INT_MAX into UB. */
    size_t istring;

    for ( istring = 0; string[istring] != '\0'; ++istring ){
        if ( !isprint( string[istring] ) ){
            string[istring] = '.';
        }
    }
}


void acq32_enable_rom( struct Acq32Device* device, int enable )
{
#ifdef LINUX_NEW_PCI
    unsigned long rom_addr = device->p_pci->resource[PCI_ROM_RESOURCE].start;
#else
    unsigned long rom_addr = device->p_pci->rom_address;
#endif    
    unsigned long ram_addr;

    if ( (device->m_dpd.rom_is_enabled = enable) ){
        rom_addr |= PCI_ROM_ADDRESS_ENABLE;
        ram_addr = BADMAP;
    }else{
        rom_addr &= ~PCI_ROM_ADDRESS_ENABLE;
#ifdef LINUX_NEW_PCI    
        ram_addr = device->pci_stash.resource[PCI_BA_SDRAM].start;
#else
        ram_addr = device->pci_stash.base_address[PCI_BA_SDRAM];
#endif   
    }

    PDEBUGL(2)( " setting PCI_BA_SDRAM    to 0x%08lx (%s)\n",
                ram_addr, enable? "FARAWAY": "IN" );
    PDEBUGL(2)( " setting PCI_ROM_ADDRESS to 0x%08lx (%s)\n",
                rom_addr, enable? "ENABLE": "DISABLE" );
                        
    {
        /* pcibios_write_config_dword() was removed in 2.6; use the
         * modern pci_dev-based API. */
        unsigned long flags;
        local_irq_save(flags);
        pci_write_config_dword(device->p_pci, PCI_BASE_ADDRESS_2, ram_addr);
        pci_write_config_dword(device->p_pci, PCI_ROM_ADDRESS,    rom_addr);
        local_irq_restore(flags);
    }
    PDEBUGL(2) ( " it's done now ...\n" );
}


static int pgm_sscanf(
    char buf[], const char fmt[], char s1[], char s2[], char s3[] 
    )
// hardcode to "%s/%s/%s" AARGGHHH
{
    char* ps, * pd;

    for ( ps = buf, pd = s1; *ps && *ps != '/'; ){
	*pd++ = *ps++;
    }
    *pd = '\0'; if ( *ps ) ps++;

    for ( pd = s2; *ps && *ps != '/'; ){
	*pd++ = *ps++;
    }
    *pd = '\0'; if ( *ps ) ps++;

    for ( pd = s3; *ps && *ps != '/'; ){
	*pd++ = *ps++;
    }
    *pd = '\0'; if ( *ps ) ps++;

    return (strlen(s1)!=0) + (strlen(s2)!=0) + (strlen(s3)!=0);
}

static void trim( char buf[] )
{
    /* Original used int.  On x86_64 strlen() returns size_t (8 bytes);
     * "int ix = strlen(buf)-1" silently truncates the high bits, and
     * if strlen returned anything > INT_MAX, sign-extending ix into a
     * pointer offset walks buf into unmapped memory.  Use size_t. */
    size_t len = strlen(buf);

    while ( len > 1 ){
	--len;
	if ( buf[len] == ' ' ){
	    buf[len] = '\0';
	}
    }
}


static void
acq32_getFirmwareName( 
	struct Acq32Device* device, char client_buf[], int nbuf )
{
	char* rom_bytes = (char*)device->rom.va;

#ifdef PGMCOMOUT
	struct image_header* p_header;
#endif
	// cater for unlikely event that the rom device is already open

	int rom_was_enabled = device->m_dpd.rom_is_enabled;

	memset( client_buf, 0, nbuf );

	if ( rom_bytes == 0 ){
		strcpy( client_buf, "no-rom-mapping" );

		return;
	}
	if ( !rom_was_enabled ){
		acq32_enable_rom( device, TRUE );
	}
#ifdef PGMCOMOUT    // falls foul of strange 21285 Offset Fixup
	p_header = (struct image_header*)device->p_rom;
	strncpy( client_buf, p_header->Name, nbuf );
#else
	memcpy_fromio( client_buf, rom_bytes+0x34, nbuf );
#endif
	client_buf[nbuf] = '\0';	/* hard NUL terminator for unprintable_as_dots/trim */
	if ( !rom_was_enabled ){
		acq32_enable_rom( device, FALSE );
	}
	unprintable_as_dots( client_buf );
	trim( client_buf );
}

static void 
acq32_getLcaName( struct Acq32Device* device, char client_buf[], int nbuf )
{
    char* rom_bytes = (char*)device->rom.va;
    struct image_header* p_header;


    // cater for unlikely event that the rom device is already open

    int rom_was_enabled = device->m_dpd.rom_is_enabled;

    if ( rom_bytes == 0 ){
        strcpy( client_buf, "nfg" );
        return;
    }

    if ( !rom_was_enabled ){
        acq32_enable_rom( device, TRUE );
    }


    memset( client_buf, 0, nbuf );

// PGM: heavy Magic!!

    p_header = (struct image_header*)(rom_bytes+0x000b0000);
    memcpy_fromio( client_buf, p_header->Name, nbuf );
    client_buf[nbuf] = '\0';	/* hard NUL terminator */

    if ( !rom_was_enabled ){
        acq32_enable_rom( device, FALSE );
    }

    unprintable_as_dots( client_buf );
    trim( client_buf );
}
static void
acq32_getCalInfo( struct Acq32Device* device, char client_buf[], int nbuf )
{
    char* rom_bytes = (char*)device->rom.va;
    struct image_header* p_header;


    // cater for unlikely event that the rom device is already open

    int rom_was_enabled = device->m_dpd.rom_is_enabled;

    if ( rom_bytes == 0 ){
        strcpy( client_buf, "nfg" );
        return;
    }

    if ( !rom_was_enabled ){
        acq32_enable_rom( device, TRUE );
    }


    memset( client_buf, 0, nbuf );

// PGM: heavy Magic!!

    p_header = (struct image_header*)(rom_bytes+0x000f0000);
    memcpy_fromio( client_buf, p_header->Name, nbuf );
    client_buf[nbuf] = '\0';	/* hard NUL terminator */

    if ( !rom_was_enabled ){
        acq32_enable_rom( device, FALSE );
    }

    unprintable_as_dots( client_buf );
    trim( client_buf );
}

static int acq32_getBlankDef( 
	struct Acq32Device* device,
	struct Acq32ImagesDef* id
	)
{
#define NBLANK 16
	if ( acq32_simulate == 0 ){
		char* rom_bytes = (char*)device->rom.va;
		int rom_was_enabled = device->m_dpd.rom_is_enabled;
		char testbuf[NBLANK];
		char blankbuf[NBLANK];

		memset( blankbuf, 0xff, sizeof(blankbuf) );

		if ( !rom_was_enabled ){
			acq32_enable_rom( device, TRUE );
		}

		memcpy_fromio( testbuf, (char*)rom_bytes, NBLANK );
		if ( !rom_was_enabled ){
			acq32_enable_rom( device, FALSE );
		}

		if ( memcmp( testbuf, blankbuf, NBLANK ) == 0 ){
			PDEBUG( "acq32_getBlankDef: it's blank\n" );
			strcpy( id->model, "BLANK" );
			return 1;
		}else{
			return 0;
		}
	}else{
		return 1;
	}
}



static int acq32_getImagesDef(
	struct Acq32Device* device,
	struct Acq32ImagesDef* id
	)
{
	dbg(1, "device %p\n", device );

	if ( acq32_simulate == 0 && !acq32_getBlankDef(device, id)){

		char calbuf[80];

		memset( calbuf, 0, sizeof(calbuf) );
		acq32_getCalInfo( device, calbuf, sizeof(calbuf)-1 );
		pgm_sscanf( calbuf, "%s/%s/%s", 
			    id->serialnum, id->channels, id->caldate );
		acq32_getFirmwareName( device, id->armfw, sizeof(id->armfw)-1 );
		acq32_getLcaName( device, id->lcafw, sizeof(id->lcafw)-1 );

		if ( strstr( id->lcafw, "acq16" ) ){
			strcpy( id->model, "ACQ16" );
		}else if ( strstr( id->lcafw, "acq32" ) ){
			strcpy( id->model, "ACQ32" );
		}else{
			strcpy( id->model, "xxxxx" );
		}	
		return 0;  // OK
	}else{
		return 1;  // BAD
	}
}



static void acq32_dev_i2o_return_mfa(struct Acq32Device* device, unsigned mfa)
{
	writel( mfa, CSR( device, PCI_I2O_OUTBOUND_FIFO ) );
}

#if 0
static void* acq32_dev_i2o_mfa2va(struct Acq32Device *device, u32 mfa)
{
	return (void*)((unsigned)device->dmabuf.va+(mfa&ACQ200_PCIWINMSK));
}
#endif

static void* acq32_dev_i2o_mfa2va(struct Acq32Device *device, u32 mfa)
{
	/* ACQ32 mfa's come with an id tag in top byte */
	return (void*)phys_to_virt(mfa);
}


void acq32_device_init(struct Acq32Device* device)
{
	device->dev_type = DEV_TYPE_ACQ32;
	device->getImagesDef = acq32_getImagesDef;
	device->set_mailbox = _acq32_set_mailbox;
	device->get_mailbox = _acq32_get_mailbox;
	device->coreDevInit = acq32_coreDevInit;
	device->i2o_return_mfa = acq32_dev_i2o_return_mfa;
	device->i2o_mfa2va  = acq32_dev_i2o_mfa2va;
	device->isr = acq32_isr;
	device->fetchDataToLocalBuffer = acq32_fetchDataToLocalBuffer;
}
