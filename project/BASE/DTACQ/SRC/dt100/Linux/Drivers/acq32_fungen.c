/*****************************************************************************
 *
 * File: acq32_fungen.c
 *
 * $RCSfile: acq32_fungen.c,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: acq32_fungen.c,v 1.5.4.1 2003/09/16 11:03:20 pgm Exp $
 * $Log: acq32_fungen.c,v $
 * Revision 1.5.4.1  2003/09/16 11:03:20  pgm
 * compiles clean on 2.4.20 (RH9x)
 *
 * Revision 1.5  2002/03/12 12:12:40  pgm
 * clean format rools OK
 *
 * Revision 1.4  2001/12/19 09:56:07  pgm
 * Linux 2.4 first cut runs, mapping dodgy
 *
 * Revision 1.3  2001/05/15 17:56:09  pgm
 * AO/DO fungen implemented
 *
 * Revision 1.2  2001/05/05 21:04:41  pgm
 * try fix DUMDMA fault with tq_scheduler, not tq_immediate
 *
 * Revision 1.1  2001/05/05 19:44:45  pgm
 * stub write(), fix bigdump dep, fault tolerant DUMDMA
 *
 *
 *
\*****************************************************************************/

#include "local.h"

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


#include "acq32.h"        /* local definitions */
#include "acq32ioctl.h"   /* APP interface defs */
#include "acq32_fungen.h"

#include "acq32busprot.h"
#include "platform.h"


static ssize_t xO_immediate_write (
    struct file *filp, 
    const char *buf, 
    size_t nbytes, 
    loff_t *pos,
    unsigned funcode
    )
{
//#define FN    "xO_immediate_write() "

    struct Acq32Device* device = PDEV(filp);
    u32 sample;
    int n32 = nbytes/sizeof(u32);
    int isample;

    PDEBUGL(2)(  " send %d funcode 0x%08x %s\n", n32, funcode,
		funcode==BP_SET_FUNCODE(BP_FC_SET_DACS)? "BP_FC_SET_DACS":
		funcode==BP_SET_FUNCODE(BP_FC_SET_DO)? "BP_FC_SET_DO":
		"xxxx" );
    
    for ( isample = 0; isample != n32; ++isample ){
        copy_from_user( &sample, buf+4*isample, 4 );
	
	PDEBUGL(3)(  " send sample %d value 0x%08x\n", isample, sample );
	    
        acq32_devSendCommandA3A4( device, funcode, 1, &sample, 0 );
    }
    
    filp->f_pos += nbytes;
    return nbytes;
    
//#undef FN    
}

ssize_t acq32_AO_immediate_write (
    struct file *filp, 
    const char *buf, 
    size_t nbytes, 
    loff_t *pos
    )
{
    return xO_immediate_write(
	filp, buf, nbytes, pos, 
	BP_SET_FUNCODE(BP_FC_SET_DACS)
	);
}

ssize_t acq32_DO_immediate_write (
    struct file *filp, 
    const char *buf, 
    size_t nbytes, 
    loff_t *pos
    )
{
    return xO_immediate_write(
	filp, buf, nbytes, pos, 
	BP_SET_FUNCODE(BP_FC_SET_DO)
	);
}

static ssize_t xO_function_write (
    struct file *filp, 
    const char *buf, 
    size_t nbytes, 
    loff_t *pos,
    unsigned funcode
    )
// use i2o push ??
{
//#define FN    "xO_function_write() "

// quick hack ... use mbox for now

    return xO_immediate_write( filp, buf, nbytes, pos, funcode );
    
// get an ACQ32_PULL_OUTPUT_DATA_RECORD from the shared buffer

// populate it

// send it

// return without waiting for result
    return 0;
    
//#undef FN    
}

ssize_t acq32_AO_function_write (
    struct file *filp, 
    const char *buf, 
    size_t nbytes, 
    loff_t *pos
    )
// use i2o push ??
{
    return xO_function_write(
	filp, buf, nbytes, pos, 
	BP_SET_FUNCODE(BP_FC_SET_DACS)|
	BP_SET_A1( BP_FC_SET_XO_FUNCTION )|
	BP_SET_A2( BP_FC_SET_XO_DATA )
	);
}
ssize_t acq32_DO_function_write (
    struct file *filp, 
    const char *buf, 
    size_t nbytes, 
    loff_t *pos
    )
{
    return xO_function_write(
	filp, buf, nbytes, pos, 
	BP_SET_FUNCODE(BP_FC_SET_DO)|
	BP_SET_A1( BP_FC_SET_XO_FUNCTION )|
	BP_SET_A2( BP_FC_SET_XO_DATA )			
	);
}




int acq32_AO_fungen_open( struct inode* inode, struct file* filp )
{
    return acq32_devSendCommand( 
	PDEV(filp),
	BP_SET_FUNCODE(BP_FC_SET_DACS)|
	BP_SET_A1( BP_FC_SET_XO_FUNCTION )|
	BP_SET_A2( BP_FC_SET_XO_CLEAR )		   );    
}

int acq32_DO_fungen_open( struct inode* inode, struct file* filp )
{
    return acq32_devSendCommand( 
	PDEV(filp),
	BP_SET_FUNCODE(BP_FC_SET_DO)|
	BP_SET_A1( BP_FC_SET_XO_FUNCTION )|
	BP_SET_A2( BP_FC_SET_XO_CLEAR )		   );
}

int acq32_AO_fungen_release( struct inode* inode, struct file* filp )
{
    int recycle = AOFRDEV( inode->i_rdev );
    
    acq32_devSendCommand( 
	PDEV(filp),
	BP_SET_FUNCODE(BP_FC_SET_DACS)|
	BP_SET_A1( BP_FC_SET_XO_FUNCTION )|
	BP_SET_A2( BP_FC_SET_XO_END|(recycle?BP_FC_SET_XO_CYCLE:0) )
	);
    // send close command
    return acq32_release( inode, filp );
}

int acq32_DO_fungen_release( struct inode* inode, struct file* filp )
{
    int recycle = DOFRDEV( inode->i_rdev );
    
    
    acq32_devSendCommand( 
	PDEV(filp),
	BP_SET_FUNCODE(BP_FC_SET_DO)|
	BP_SET_A1( BP_FC_SET_XO_FUNCTION )|
	BP_SET_A2( BP_FC_SET_XO_END|(recycle?BP_FC_SET_XO_CYCLE:0) )
	);
    
    return acq32_release( inode, filp );
}


