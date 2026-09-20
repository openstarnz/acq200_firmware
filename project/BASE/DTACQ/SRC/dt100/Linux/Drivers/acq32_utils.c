/*****************************************************************************
 *
 * File: acq32_utils.c - Linux device driver for acq32 - utils funcs
 *
 * $RCSfile: acq32_utils.c,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: acq32_utils.c,v 1.12.4.1 2003/09/16 11:03:20 pgm Exp $
 * $Log: acq32_utils.c,v $
 * Revision 1.12.4.1  2003/09/16 11:03:20  pgm
 * compiles clean on 2.4.20 (RH9x)
 *
 * Revision 1.12  2002/08/26 13:10:00  pgm
 * impl status stream - compiles 2.4, needs tesst
 *
 * Revision 1.11  2002/06/16 14:05:59  pgm
 * RH62 back mods
 *
 * Revision 1.10  2002/07/19 18:39:37  pgm
 * mod DUMDMA to avoid message overrun
 *
 * Revision 1.9  2002/03/28 08:42:42  pgm
 * fixes isr to sep ack, cos, add waitevent, doubletap good
 *
 * Revision 1.8  2002/03/08 19:22:00  pgm
 * bigger readbuffer
 *
 * Revision 1.7  2001/03/28 21:18:30  pgm
 * i2o command handling for FETCH cmpiles - willit work??
 *
 * Revision 1.6  2001/01/14 17:54:11  pgm
 * multi thread safemake
 *
 * Revision 1.5  2001/01/02 12:11:08  pgm
 * slight cleanup
 *
 * Revision 1.4  2000/11/07 22:41:23  pgm
 * read() access works correctly on pre-samples
 *
 * Revision 1.3  2000/05/18 21:02:02  pgm
 * removed -lc from link (failed under RH6.2
 *
 * Revision 1.2  1999/11/13 20:31:48  pgm
 * error reporting in master, read funcs
 *
 * Revision 1.1  1999/10/12 10:23:47  pgm
 * basic master command/readback struct WORKS
 *
 *
\*****************************************************************************/

#include "local.h"
#include <stdarg.h>

#ifndef __KERNEL__
#  define __KERNEL__
#endif


#include <linux/module.h>

#include <linux/kernel.h> /* printk() */
#include <linux/fs.h>     /* everything... */
#include <linux/errno.h>  /* error codes */
#include <linux/types.h>  /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>        /* O_ACCMODE */
#include <linux/pci.h>
#include <linux/sched.h>


#include <asm/io.h>       /* ioremap()         */
#include <asm/irq.h>
#include <asm/uaccess.h>  /* VERIFY_READ|WRITE */
#include <asm/system.h>   /* cli(), *_flags */
#include <asm/segment.h>  /* memcpy and such */



#include "acq32.h"        /* local definitions */
#include "acq32ioctl.h"   /* APP interface defs */

#include "platform.h"

/*
 * the readbuffer is a preincrement put ring buffer
 */

#define IS_EMPTY( rb )     ((rb)->ibput==(rb)->ibget)
#define INC( ib )          (((ib)+1)&(READBUFFERLEN-1))
#define IS_FULL( rb )      (INC((rb)->ibput)==(rb)->ibget)

#define PUT( rb, cc ) \
   ((rb)->rbuffer[(rb)->ibput=INC((rb)->ibput)] = (cc))
#define GET( rb ) \
   ((rb)->rbuffer[(rb)->ibget=INC((rb)->ibget)])

/*
int acq32_readbuffer_printf( struct Acq32Device* p_dev, char* fmt, ... )
{
    char local[80];
    va_list ap;

    va_start( ap, fmt );

    vsprintf( local, fmt, ap );
    ASSERT( strlen( local ) < 80-1 );

    va_end( ap );

    return acq32_readbuffer_put( p_dev, local, strlen(local) );
}
*/

static int readbuffer_put( struct ReadBuffer* rb, char* cli_data, int nbytes )
{
    int ibyte;

    for ( ibyte = 0; ibyte < nbytes && !IS_FULL( rb ); ++ibyte ){
        PUT( rb, cli_data[ibyte] );
    }
    return ibyte;
}

static int readbuffer_get( struct ReadBuffer* rb, char* cli_data, int maxbytes)
// returns nbytes pulled from buffer
{
    
    int ibyte;

    for ( ibyte = 0; ibyte != maxbytes && !IS_EMPTY( rb ); ++ibyte ){
        cli_data[ibyte] = GET( rb );
    }
    return ibyte;
}


int acq32_device_readbuffer_put( 
    struct Acq32Device* p_dev, char* cli_data, int nbytes )
// returns nbytes put on buffer
{
    return readbuffer_put( &p_dev->mrb, cli_data, nbytes );
}

int acq32_device_readbuffer_get( 
    struct Acq32Device* p_dev, char* cli_data, int maxbytes )
// returns nbytes pulled from buffer
{
    int nget = readbuffer_get( &p_dev->mrb, cli_data, maxbytes );

    PDEBUGL(3) ( "acq32_device_readbuffer_get() returns %d\n", nget );
    return nget;
}

int acq32_path_readbuffer_put( struct Acq32Path* path, char* cli_data, int nbytes )
{
    return readbuffer_put( &path->prb, cli_data, nbytes );
}

/*
 * Staging buffer for ACQ32_PATH_READBUFFER_PRINTF()/APR_PRINTF().
 *
 * Sized from the worst-case formatted length over all current call sites. The
 * 92 command-parser call sites are in acq32_command.c; acq32_simul.c has nine
 * additional literal-only call sites. Twelve command-parser calls exceed
 * 80 bytes:
 *
 *   111  getChannelMask=%s      "getChannelMask=" + a 96 char ACQ196 mask
 *   119  getVoltsRange ...      eight %d, each worst case INT_MIN
 *   272  "ERROR: bad mode %s\n"                        ) ten sites that echo
 *   281  "ERROR:Incorrect dest arg %s\n"               ) an argv[] token back
 *   285  "ERROR:Incorrect function arg %s\n"           ) to the caller
 *   287  "ERROR:dont understand selector %s\n"         )
 *   292  "ERROR: failed: diX (%s) not recognised\n" x2 )
 *   295  "ERROR:getPhase property not recognised %s\n" )
 *   295  "ERROR:SetClock failed, unknown subject %s\n" )
 *   296  "ERROR:SetInternalClock <freq> [DOx] not %s\n")
 *   296  "ERROR: failed to decode EVENT CONDITION %s\n")
 *
 * The argv[] sites dominate: _write_command() clamps a write to
 * MAXCMDLINE-1 == 255 bytes and getargs() puts no cap on token length, so a
 * single token can be ~255 characters and any of these can be asked to format
 * ~296 bytes.  128 is therefore not enough, and neither is 256.  296 + NUL is
 * 297; 320 rounds that up with a little headroom.
 *
 * The other 80 sites are all under 80 bytes; the largest is 79
 * ("%s%d: %s, %s, %s, %s, %s\n" in do_GetSyncRoute_command()).
 *
 * READBUFFERLEN is 4096, so the readbuffer downstream takes 320 without
 * complaint.
 */
#define APR_PRINTF_BUFLEN 320

void acq32_path_readbuffer_printf( struct Acq32Path* path, const char* fmt, ... )
{
    char local[APR_PRINTF_BUFLEN];
    va_list ap;
    int len;

    va_start( ap, fmt );
    len = vsnprintf( local, sizeof(local), fmt, ap );
    va_end( ap );

    /*
     * vsnprintf() returns what it *would* have written, so an overrun is
     * detectable here rather than silently short-changing userspace the way
     * the inline snprintf() did.  Nothing should hit this - if something does,
     * the call site is new and APR_PRINTF_BUFLEN needs revisiting.
     */
    if ( len < 0 ){
        /* vsnprintf() failed - emit nothing rather than passing a negative
         * length down to readbuffer_put().
         */
        return;
    }
    if ( len >= (int)sizeof(local) ){
        printk( KERN_WARNING
                "acq32: APR_PRINTF truncated, needed %d of %d bytes: %s\n",
                len + 1, (int)sizeof(local), local );
        len = (int)sizeof(local) - 1;
    }

    PRINTCMD( "acq32:>", local );

    acq32_path_readbuffer_put( path, local, len );
}

int acq32_path_readbuffer_get( struct Acq32Path* path, char* cli_data, int maxbytes )
{
    int nget = readbuffer_get( &path->prb, cli_data, maxbytes );

    PDEBUGL(3)( "acq32_path_readbuffer_get() returns %d\n", nget );

    return nget;
}

int acq32_path_readbuffer_is_empty( struct Acq32Path* path ) {
    return IS_EMPTY( &path->prb );
}

int acq32_path_readbuffer_stash( struct Acq32Path* path )
// copy any data in path buffer to device buffer 
{
    int ncopy;

    for( ncopy = 0; !IS_EMPTY( &path->prb ); ++ncopy ){
	char achar = GET( &path->prb );
	if ( IS_FULL( &path->device->mrb ) ){
	    return -1;
	}else{
	    PUT( &path->device->mrb, achar );
	}
    }

    PDEBUGL(3)( "acq32_path_readbuffer_stash() returns %d\n", ncopy );

    return ncopy;
}

void acq32_path_readbuffer_create( struct Acq32Path* path )
{
    acq32_readbuffer_create( &path->prb );   
}
void acq32_path_readbuffer_delete( struct Acq32Path* path )
{
    acq32_readbuffer_delete( &path->prb );    
}

void acq32_device_readbuffer_create( struct Acq32Device* p_dev )
{
    acq32_readbuffer_create( &p_dev->mrb );    
}
void acq32_device_readbuffer_delete( struct Acq32Device* p_dev )
{
    acq32_readbuffer_delete( &p_dev->mrb );
}

struct MASK_ITERATOR {
    unsigned mask;
    unsigned icursor;
    enum state { MI_IDLE, MI_STARTED, MI_FINISHED } state;
};

int hasNext( struct MASK_ITERATOR* it ) {
    unsigned next_cursor = 0;

    switch( it->state ){
    case MI_IDLE:
	next_cursor = 0x80000000;
	it->icursor = 31;
	break;
    case MI_STARTED:
	next_cursor = (1 << it->icursor) >> 1;
	break;
    case MI_FINISHED:
	return 0;
    }
    for ( ; next_cursor != 0; next_cursor >>= 1 ){
	if ( it->mask & next_cursor ){
	    return 1;
	}
    }
    
    return 0;
}
int getNext( struct MASK_ITERATOR* it ) {
    unsigned next_cursor = 0;

    switch( it->state ){
    case MI_IDLE:
	next_cursor = 0x80000000;
	it->state = MI_STARTED;
	it->icursor = 31;
	break;
    case MI_STARTED:
	next_cursor = 1 << --it->icursor;
	break;
    case MI_FINISHED:
	return -1;
    }
    for ( ; next_cursor != 0; next_cursor >>= 1, --it->icursor ){
	if ( it->mask & next_cursor ){
	    return it->icursor;
	}
    }
    
    it->state = MI_FINISHED;
    return -1;
}
int acq32_intsDisable( unsigned irqs )
{
    struct MASK_ITERATOR it = { irqs };

    while ( hasNext( &it ) ){
	disable_irq_nosync( getNext( &it ) );
    }

    return 0;
}
int acq32_intsEnable( unsigned irqs )
{
    struct MASK_ITERATOR it = { irqs };

    while ( hasNext( &it ) ){
	enable_irq( getNext( &it ) );
    }

    return 0;
}

#define MAXBUF 1024

#ifdef LINUX_NEW_PCI
DECLARE_WAIT_QUEUE_HEAD(wq);
#else
// WORKTODO
#endif

int acq32_swallowKmem( unsigned orders )
{
//#define FN "acq32_swallowKmem() "
    int ibuf = 0;
    int nbuf;
    int totpages = 0;
    struct MASK_ITERATOR it = { orders };

    void** buffers = KMALLOC( MAXBUF*sizeof(void*), GFP_KERNEL );
    int*  porders   = KMALLOC( MAXBUF*sizeof(int), GFP_KERNEL );
    ASSERT( buffers );

    while ( hasNext( &it ) ){
	int order = getNext( &it );
	void *pbuf;

	PDEBUG(  "order %d\n", order );

	while ( ibuf < MAXBUF && 
                (pbuf = (void*)__GET_FREE_PAGES( GFP_KERNEL, order )) ){
	    buffers[ibuf] = pbuf;
	    porders[ibuf] = order;
	    ++ibuf;
	}
	PDEBUG(  "Allocating Order %d, total buffers %d\n",
		order, ibuf );
    }

    nbuf = ibuf;

    for ( ibuf = 0; ibuf != nbuf; ++ibuf ){
	totpages += 1<<porders[ibuf];
    }
    PDEBUG(  "now sleep until woken, total pages %d (%d MB)\n", 
	    totpages, totpages*4/1024 );
#ifdef LINUX_NEW_PCI
    interruptible_sleep_on( &wq );
#else
    ; // WORKTODO
#endif
    PDEBUG(  "rise and shaine!\n" );

    for ( ibuf = 0; ibuf != nbuf; ++ibuf ){
	FREE_PAGES( (unsigned long)buffers[ibuf], porders[ibuf] );
    }
    KFREE( buffers );
    KFREE( porders );

    PDEBUG( "klaaring out\n" );

    return 0;
}
