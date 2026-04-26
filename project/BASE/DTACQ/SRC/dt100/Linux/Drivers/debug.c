/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: debug.c,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: debug.c,v 1.9.4.1 2003/09/09 20:50:19 pgm Exp $
 * $Log: debug.c,v $
 * Revision 1.9.4.1  2003/09/09 20:50:19  pgm
 * *** empty log message ***
 *
 * Revision 1.9  2002/07/19 18:39:37  pgm
 * mod DUMDMA to avoid message overrun
 *
 * Revision 1.8  2002/07/02 10:59:49  pgm
 * add mmap testing
 *
 * Revision 1.7  2002/03/12 12:12:40  pgm
 * clean format rools OK
 *
 * Revision 1.6  2001/12/19 09:56:07  pgm
 * Linux 2.4 first cut runs, mapping dodgy
 *
 * Revision 1.5  2001/06/01 19:45:33  pgm
 * GUT WIP - hooking up core, compiles
 *
 * Revision 1.4  2001/05/27 18:56:49  pgm
 * *** empty log message ***
 *
 * Revision 1.3  2001/05/05 19:44:45  pgm
 * stub write(), fix bigdump dep, fault tolerant DUMDMA
 *
 * Revision 1.2  2001/03/31 20:58:24  pgm
 * DUMDMA works in slow mo
 *
 * Revision 1.1  2001/03/31 15:25:50  pgm
 * *** empty log message ***
 *
 *
\*****************************************************************************/


#include "local.h"

#ifndef __KERNEL__
#  define __KERNEL__
#endif

#include <linux/module.h>

#include <linux/kernel.h> /* printk() */
#ifdef LINUX_NEW_PCI
#include <linux/slab.h>
#else
#include <linux/malloc.h> /* kmalloc() */
#endif
#include <linux/mm.h>     /* mem_map[] etc */
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

#include "platform.h"

#include "arm_modules.h"

#include "acq200_debug.h"

extern int acq32_leak_detect;

void* dbg_kmalloc( size_t size, int flags, const char* file, int line ) {
    void* addr = kmalloc( size, flags );
    LEAK( ":%p ret by ( 0x%08x, 0x%08x ) [%s,%d]",
		addr, size, flags, file, line );
    return addr;
}
void __dbg_kfree( const void* addr, const char* file, int line ) {
    kfree( addr );
    LEAK( ":%p [%s,%d]", addr, file, line );
}

unsigned long dbg__get_free_pages( 
    int prio, int order, const char* file, int line 
    )
{
    unsigned long addr = __get_free_pages( prio, order );
    
    PDEBUGL(acq32_leak_detect)( "__get_free_pages( %d, %d ) returns 0x%08lx [%s,%d]\n",
                prio, order, addr, file, line );
		
    return addr;
}
void dbg_free_pages(
    unsigned long pages, int order, const char* file, int line
    )
{
#ifdef LINUX_NEW_PCI

#else
    unsigned long map_nr = MAP_NR( pages );
    atomic_t count = mem_map[map_nr].count;
    unsigned long flags = mem_map[map_nr].flags;
#endif    
    free_pages( pages, order );
    
#ifdef LINUX_NEW_PCI
    PDEBUGL(acq32_leak_detect)( "free_pages( 0x%08lx, %d )\n", pages, order );
#else        
    PDEBUGL(4)( "free_pages( 0x%08lx, %d ) mn:%x c:%d,%ld fl:0x%08lx [%s,%d]\n", 
		pages, order, map_nr, count, mem_map[map_nr].count, flags, file, line );
#endif	    
}


void acq32_dump( char* fmt, void* dp ) 
{
    unsigned * p = (unsigned*)dp;

    PDEBUG( "%s:%p: %08x %08x %08x %08x\n", 
	    fmt, p,
	    p[0], p[1], p[2], p[3] );
    
}
