/*****************************************************************************
 *
 * File: acq32_core.c - core driver for acq32
 *
 * $RCSfile: acq32_core.c,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: *
 * $Id: acq32_core.c,v 1.150.4.11 2006/04/22 16:11:28 pgm Exp $
 * $Log: acq32_core.c,v $
 * Revision 1.150.4.11  2006/04/22 16:11:28  pgm
 * acq32_getDriver allocates fresh vtable per call so that clients can override methods with interference.
 * Fixes acq32 setChannelMask bug in mixed acq196/acq216 system
 *
 * Revision 1.150.4.10  2005/03/28 19:58:51  pgm
 * fixes acq196 channel mask
 *
 * Revision 1.150.4.9  2003/12/14 22:01:04  pgm
 * split acq32, acq200 drivers
 *
 * Revision 1.150.4.8  2003/11/03 12:04:42  pgm
 * lazy bigbuf init, dev swap for acq200
 *
 * Revision 1.150.4.7  2003/10/25 19:02:00  pgm
 * acq200_fetchData (NFG)
 *
 * Revision 1.150.4.6  2003/09/16 11:03:20  pgm
 * compiles clean on 2.4.20 (RH9x)
 *
 * Revision 1.150.4.5  2003/09/08 12:51:30  pgm
 * i2o file download works
 *
 * Revision 1.150.4.4  2003/09/07 14:38:23  pgm
 * stub wavegen intro
 *
 * Revision 1.150.4.3  2003/09/06 07:28:03  pgm
 * acq32 now works again after acq200 split
 *
 * Revision 1.150.4.2  2003/09/04 21:11:12  pgm
 * acq200 getFwrev works (sorta) x156
 *
 * Revision 1.150.4.1  2003/08/31 16:53:52  pgm
 * first acq200 testing
 *
 * Revision 1.150.2.1  2003/08/29 17:17:03  pgm
 * first stage acq200 support
 *
 * Revision 1.150  2003/04/05 09:16:55  pgm
 * fixes stride>1 fetch
 *
 * Revision 1.149  2002/11/03 20:14:52  pgm
 * get event
 *
 * Revision 1.148  2002/08/29 18:36:25  pgm
 * COS device tools OK
 *
 * Revision 1.147  2002/08/26 13:10:00  pgm
 * impl status stream - compiles 2.4, needs tesst
 *
 * Revision 1.146  2002/08/23 20:00:32  pgm
 * arm wdt rools OK
 *
 * Revision 1.145  2002/08/23 18:38:17  pgm
 * *** empty log message ***
 *
 * Revision 1.144  2002/08/23 08:05:05  pgm
 * restore previous armAll
 *
 * Revision 1.143  2002/08/20 10:07:42  pgm
 * clean globs, use mutex for IOREAD
 *
 * Revision 1.142  2002/08/10 16:10:11  pgm
 * gated fetch part success
 *
 * Revision 1.141  2002/08/09 13:52:53  pgm
 * ioread_fetch_mutex
 *
 * Revision 1.140  2002/08/04 12:44:41  pgm
 * better debug (but it bombs)
 *
 * Revision 1.139  2002/08/04 12:26:08  pgm
 * improve CYCLE test in setArm
 *
 * Revision 1.138  2002/08/04 12:17:30  pgm
 * remove CYCLE test in setArm
 *
 * Revision 1.137  2002/08/04 12:07:11  pgm
 * boost opt to setChannelMask
 *
 * Revision 1.136  2002/08/03 18:56:04  pgm
 * allow fetch local on any boundary
 *
 * Revision 1.135  2002/07/29 10:05:56  pgm
 * avoid spurious error report
 *
 * Revision 1.134  2002/07/29 09:20:10  pgm
 * ipbuf != ireq
 *
 * Revision 1.133  2002/07/29 08:06:53  pgm
 * acq32_fetchDataToLocalBuffer:calc user buffer offset in VMA
 *
 * Revision 1.132  2002/07/19 18:39:37  pgm
 * mod DUMDMA to avoid message overrun
 *
 * Revision 1.131  2002/07/02 10:59:49  pgm
 * add mmap testing
 *
 * Revision 1.130  2002/06/12 20:20:19  pgm
 * fix master device. gotta work now
 *
 * Revision 1.129  2002/06/11 14:49:32  pgm
 * streaming extra prams
 *
 * Revision 1.128  2002/05/30 19:36:36  pgm
 * move sysinclude for rh73
 *
 * Revision 1.127  2002/05/15 09:34:45  pgm
 * debug==8, debug==7 short circuits
 *
 * Revision 1.126  2002/05/15 09:17:41  pgm
 * *** empty log message ***
 *
 * Revision 1.125  2002/05/15 09:10:32  pgm
 * downstreambuf
 *
 * Revision 1.124  2002/05/15 08:57:40  pgm
 * *** empty log message ***
 *
 * Revision 1.123  2002/05/15 08:43:59  pgm
 * second duff_mfa check
 *
 * Revision 1.122  2002/05/15 08:27:26  pgm
 * second duff_mfa check
 *
 * Revision 1.121  2002/05/15 07:38:19  pgm
 * *** empty log message ***
 *
 * Revision 1.120  2002/04/20 13:07:51  pgm
 * makes multi master work again
 *
 * Revision 1.119  2002/04/09 12:27:58  pgm
 * better proto debug
 *
 * Revision 1.118  2002/03/28 18:57:29  pgm
 * status device works
 *
 * Revision 1.117  2002/03/28 11:23:09  pgm
 * oops, init 0xffff int IS significant
 *
 * Revision 1.116  2002/03/28 10:20:35  pgm
 * clean up isrs
 *
 * Revision 1.115  2002/03/28 09:26:38  pgm
 * remove redundant init waitq calls
 *
 * Revision 1.114  2002/03/28 08:42:42  pgm
 * fixes isr to sep ack, cos, add waitevent, doubletap good
 *
 * Revision 1.113  2002/03/27 15:48:39  pgm
 * clean up all handling
 *
 * Revision 1.112  2002/03/25 02:10:15  pgm
 * new style proc/acq32
 *
 * Revision 1.107  2002/03/07 12:25:19  pgm
 * acq32_double_tap
 *
 * Revision 1.106  2002/02/14 09:11:09  pgm
 * handa2:trigger_DIx on setModeTriggeredConinuous
 *
 * Revision 1.104  2002/02/10 18:15:25  pgm
 * FAST_MULTI opt
 *
 * Revision 1.103  2002/02/02 14:44:42  pgm
 * waitEvent (not complete), multimaster updates
 *
 * Revision 1.102  2002/01/06 12:14:52  pgm
 * data fetch now returns actual num samples available
 *
 * Revision 1.101  2001/12/21 21:20:09  pgm
 * fix acq32_GetHeartbeat() logic error - broke startup
 *
 * Revision 1.100  2001/12/20 21:02:03  pgm
 * limit excessive dbg msg
 *
 * Revision 1.99  2001/12/19 09:56:07  pgm
 * Linux 2.4 first cut runs, mapping dodgy
 *
 * Revision 1.98  2001/12/04 15:20:54  pgm
 * works for implicit self==slave
 *
 * Revision 1.97  2001/11/23 21:54:57  pgm
 * continuous stream speed demo done
 *
 * Revision 1.96  2001/10/21 09:08:40  pgm
 * selectThresholdDetector etc
 *
 * Revision 1.95  2001/10/20 20:42:30  pgm
 * selectThresholdDistributor opts
 *
 * Revision 1.94  2001/10/20 19:44:19  pgm
 * getDebug
 *
 * Revision 1.93  2001/10/20 15:52:11  pgm
 * clock redistribution opts included
 *
 * Revision 1.92  2001/10/13 19:56:47  pgm
 * IOCTL write finds flash, but prog bombs
 *
 * Revision 1.91  2001/10/01 21:27:16  pgm
 * DTACQ coding of header.id to avoid i2o clash
 *
 * Revision 1.90  2001/09/10 21:21:00  pgm
 * check heartbeat before init
 *
 * Revision 1.89  2001/09/03 15:53:06  pgm
 * threshold setting done
 *
 * Revision 1.88  2001/09/03 13:47:55  pgm
 * thresh trigger works (above, below right wya round
 *
 * Revision 1.87  2001/08/23 18:31:22  pgm
 * minor fix to setUser command handling
 *
 * Revision 1.86  2001/08/20 19:56:22  pgm
 * ALPHA:no direct deref io space
 *
 * Revision 1.85  2001/08/19 20:23:47  pgm
 * threshold commands safely on board
 *
 * Revision 1.84  2001/08/19 16:59:00  pgm
 * add threshold commands (untested)
 *
 * Revision 1.83  2001/08/19 08:52:55  pgm
 * resetGUT opt
 *
 * Revision 1.82  2001/06/29 11:32:18  pgm
 * pre-release
 *
 * Revision 1.81  2001/06/25 21:57:00  pgm
 * view commands added
 *
 * Revision 1.80  2001/06/23 11:22:02  pgm
 * BE opt
 *
 * Revision 1.79  2001/06/11 09:02:02  pgm
 * setClock, clockNow hooked up
 *
 * Revision 1.78  2001/06/02 19:57:57  pgm
 * reserveAO
 *
 * Revision 1.77  2001/06/01 20:10:21  pgm
 * GUT - hooking up core, compiles, willit work
 *
 * Revision 1.76  2001/06/01 19:45:32  pgm
 * GUT WIP - hooking up core, compiles
 *
 * Revision 1.75  2001/05/19 19:43:52  pgm
 * kludge for CSR mem mapping, getPhysicalAddress
 *
 * Revision 1.74  2001/05/17 18:51:40  pgm
 * ACQ32_IOREAD_GETPHYSICAL implemented
 *
 * Revision 1.73  2001/05/05 21:04:40  pgm
 * try fix DUMDMA fault with tq_scheduler, not tq_immediate
 *
 * Revision 1.72  2001/05/05 19:44:45  pgm
 * stub write(), fix bigdump dep, fault tolerant DUMDMA
 *
 * Revision 1.71  2001/04/28 08:01:43  pgm
 * add irq_req_mutex - avoid over registering isr
 *
 * Revision 1.70  2001/04/27 19:32:43  pgm
 * use mutex to guard mbox access - ship it quick
 *
 * Revision 1.69  2001/04/27 17:19:50  pgm
 * fix race in getFwrev
 *
 * Revision 1.68  2001/04/15 09:52:45  pgm
 * syncSetRoute() works
 *
 * Revision 1.67  2001/04/01 08:26:32  pgm
 * DUMDMA works - fix with post sync on acq32
 *
 * Revision 1.66  2001/03/31 20:58:24  pgm
 * DUMDMA works in slow mo
 *
 * Revision 1.65  2001/03/30 20:17:01  pgm
 * UDMA first cut working
 *
 * Revision 1.64  2001/03/28 21:18:29  pgm
 * i2o command handling for FETCH cmpiles - willit work??
 *
 * Revision 1.63  2001/03/25 19:19:04  pgm
 * first cut i2o command processing
 *
 * Revision 1.62  2001/03/25 17:28:40  pgm
 * i2o commands work
 *
 * Revision 1.61  2001/03/23 19:44:00  pgm
 * mmap() as kernel malloc to user mapping work
 *
 * Revision 1.60  2001/02/24 18:18:36  pgm
 * test device for lltest, also acqcmd
 *
 * Revision 1.59  2001/01/14 17:54:11  pgm
 * multi thread safemake
 *
 * Revision 1.58  2001/01/02 12:11:08  pgm
 * slight cleanup
 *
 * Revision 1.57  2000/11/12 15:13:43  pgm
 * read() from host memory buffer is perfect
 *
 * Revision 1.56  2000/11/07 22:41:22  pgm
 * read() access works correctly on pre-samples
 *
 * Revision 1.55  2000/10/28 21:18:59  pgm
 * getSetNumSamples
 *
 * Revision 1.54  2000/10/22 20:36:16  pgm
 * Get Internal Clock now returns actual
 *
 * Revision 1.53  2000/10/07 21:33:05  pgm
 * trigger continuous  etc commands impl
 *
 * Revision 1.52  2000/10/02 19:00:27  pgm
 * fix bug pasted int setChannel
 *
 * Revision 1.51  2000/10/01 21:38:34  pgm
 * bigdump, getFwrev features work
 *
 * Revision 1.50  2000/10/01 18:20:35  pgm
 * bigdump, getFwrev first cut
 *
 * Revision 1.49  2000/10/01 12:40:50  pgm
 * adjust fetch data to avoid negative starts
 *
 * Revision 1.48  2000/10/01 08:27:49  pgm
 * fix rowdev read bug - uses channel mask, ArmSpin flash prog feature
 *
 * Revision 1.47  2000/09/26 19:59:34  pgm
 * split SET_MODE_GC
 *
 * Revision 1.46  2000/09/26 19:46:52  pgm
 * GATED_CONTINUOUS pre/post, lseek, spin
 *
 * Revision 1.45  2000/09/22 13:59:45  pgm
 * min reads per sec timing implemented
 *
 * Revision 1.44  2000/09/18 13:29:34  pgm
 * slave iterator from low to hi to get multi board mask right
 *
 * Revision 1.43  2000/09/18 13:09:11  pgm
 * big buf prototype code in place, defualt:OFF
 *
 * Revision 1.42  2000/09/05 08:24:38  pgm
 * checks for duff mfa (PGMCOMOUT)
 *
 * Revision 1.41  2000/08/22 06:32:45  pgm
 * better instrumentation, diags, Q's work
 *
 * Revision 1.40  2000/08/16 19:08:34  pgm
 * streaming with multi sample messages: works (slowly)
 *
 * Revision 1.39  2000/08/13 06:13:42  pgm
 * stream.h
 *
 * Revision 1.38  2000/08/04 22:23:29  pgm
 * streaming workscp acq32-drv.o /mnt/barra/dt100/modules/
 *
 * Revision 1.37  2000/08/04 06:53:44  pgm
 * i2o diags added
 *
 * Revision 1.36  2000/07/01 06:31:31  pgm
 * SOFT_CONTINUOUS is valid mode, streams withno bh
 *
 * Revision 1.35  2000/06/27 21:19:31  pgm
 * this orta work
 *
 * Revision 1.34  2000/06/25 19:41:01  pgm
 * soft continuous and others
 *
 * Revision 1.33  2000/06/25 05:53:29  pgm
 * acq32 push works
 *
 * Revision 1.31  2000/06/04 18:41:00  pgm
 * Signal Conditioning protocol implemented
 *
 * Revision 1.30  2000/05/28 10:31:12  pgm
 * streaming works passibly in simulator
 *
 * Revision 1.29  2000/05/20 11:03:48  pgm
 * Rev 6 commands work + sim sample tagging
 *
 * Revision 1.28  2000/05/19 21:15:58  pgm
 * ICD rev6 command handling parsers in
 *
 * Revision 1.27  2000/03/26 22:52:27  pgm
 * correct nasty accident to int timing, status code
 *
 * Revision 1.26  2000/03/26 22:03:30  pgm
 * correct nasty accident to int timing, status code
 *
 * Revision 1.25  2000/03/26 20:59:05  pgm
 * non blocking, status changes work with ints
 *
 * Revision 1.24  2000/01/09 21:53:58  pgm
 * fix bug in setChannelMask command - input buf overflow
 *
 * Revision 1.23  2000/01/08 22:07:50  pgm
 * get num channels now reflects multi master mask
 *
 * Revision 1.22  2000/01/08 20:38:17  pgm
 * interrupt handling (not default)
 *
 * Revision 1.21  2000/01/04 10:53:49  pgm
 * eliminate slow poll bottleneck
 *
 * Revision 1.20  1999/11/29 21:10:34  pgm
 * fix row read for reduced channels
 *
 * Revision 1.19  1999/11/20 21:20:30  pgm
 * 991120 time for bed
 *
 * Revision 1.18  1999/11/13 20:31:48  pgm
 * error reporting in master, read funcs
 *
 * Revision 1.17  1999/11/12 22:15:20  pgm
 * lines and dos mode added
 *
 * Revision 1.16  1999/11/12 17:10:40  pgm
 * fix starange volts format bug; acqN.mN device
 *
 * Revision 1.15  1999/11/11 22:11:30  pgm
 * plug mem leak, fix buffer discon prob in ascii read
 *
 * Revision 1.14  1999/11/09 21:00:13  pgm
 * buffering fixed, works with any size buffer
 *
 * Revision 1.13  1999/11/07 09:35:06  pgm
 * data handling is a runner
 *
 * Revision 1.12  1999/11/03 21:49:55  pgm
 * stash for backup
 *
 * Revision 1.11  1999/11/02 21:35:53  pgm
 * bocking Arm is reliable - ship it
 *
 * Revision 1.10  1999/11/02 14:14:12  pgm
 * row formatting rools ok
 *
 * Revision 1.9  1999/11/02 12:01:25  pgm
 * rowdev reads too
 *
 * Revision 1.8  1999/11/02 10:43:35  pgm
 * channel read works
 *
 * Revision 1.7  1999/11/01 17:31:50  pgm
 * first stab at data fetch (not complete)
 *
 * Revision 1.6  1999/10/26 15:16:54  pgm
 * struct debug, mbox commands
 *
 * Revision 1.5  1999/10/14 21:10:28  pgm
 * master command handling works
 *
 * Revision 1.4  1999/10/12 21:24:20  pgm
 * master command handling top down connection OK
 *
 * Revision 1.3  1999/10/12 10:23:47  pgm
 * basic master command/readback struct WORKS
 *
 * Revision 1.2  1999/10/11 20:21:02  pgm
 * basic command handling and data format OK
 *
 * Revision 1.1  1999/10/09 21:40:59  pgm
 * args handling in write taped
 *
 *
\*****************************************************************************/



#include "local.h"


#define _ACQ32_CORE_C_

/* start kitchen sink ... one or more of these needed for <linux/interrupt.h>*/
#include <linux/module.h>
#include <linux/version.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/major.h>
#include <asm/segment.h>
#include <asm/io.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/signal.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/ioport.h>
#include <linux/timer.h>
#include <linux/wait.h>
#include <linux/isdnif.h>
#include <linux/tty.h>
#include <linux/serial_reg.h>
/* end kitchen sink */

#include <linux/interrupt.h>
#include <linux/module.h>

#include <linux/kernel.h> /* printk() */
#include <linux/fs.h>     /* everything... */
#include <linux/errno.h>  /* error codes */
#include <linux/types.h>  /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>        /* O_ACCMODE */
#include <linux/pci.h>
#include <linux/sched.h>  /* interruptable_sleep_on_timeout() */

#include <asm/io.h>       /* ioremap()         */
#include <asm/uaccess.h>  /* VERIFY_READ|WRITE */
#include <asm/system.h>   /* cli(), *_flags */
#include <asm/segment.h>  /* memcpy and such */


#include "acq32.h"        /* local definitions */
#include "acq32ioctl.h"   /* APP interface defs */
#include "acq32busprot.h" /* interface to phys board */

#include "dt100api.h"

#include "platform.h"



#define PCI_BA_CSR    0
#define PCI_BA_CSR_IO 1           /* not used */
#define PCI_BA_SDRAM  2

#include "acq200_debug.h"


char* acq32_acq32busprot_rev = BP_REV;


#define NULL_A1     NULL
#define NULL_A2     NULL
#define NULL_A3     NULL
#define NULL_A4     NULL


static int acq32_GetAvailableChannels(struct Acq32Path* path);
static int acq32_GetRange(struct Acq32Path* path);

/*
 * Proc filesystem
 * device bus fun IO SRAM ROM #CHANNELS [firmware] [modid] etc
 */



void rchInit( struct Acq32Device* device )
{
	struct ReturnCommandHandler *rch = &device->m_dpd.__first_handler;

        rch->next = rch->prev = rch;
	rch->handler = NULL;
	rch->lock = &device->m_dpd.__rch_lock;
        spin_lock_init(rch->lock);
	device->m_dpd.first_rch = rch;
}



/*
 * iterate the slaves
 */


static void fillSlaveMask( unsigned short* pmask ){
    int iboard;
    unsigned cursor = 0x1;

    for ( iboard = 0; iboard != acq32_getDeviceCount(); ++iboard, cursor <<= 1 ){
	    *pmask |= cursor;
    }
}

struct Acq32Device* acq32_getNextSlave( 
    struct Acq32Path* path, SlaveIterState* state 
    )
// call first time with *state == 0
// NB WONT WORK for boards > 7!
{
    PDEBUGL(4)( "acq32_getNextSlave() state->slaves 0x%04x cursor 0x%04x\n",
                state->slaves, state->iter_cursor                   );
 
    if ( state->slaves == 0 ){
        fillSlaveMask( &state->full_mask );
	if ( IS_MASTER_ALL( path->minor ) ){
	    fillSlaveMask( &state->slaves );
	}else{
	    state->slaves = SLAVES( path->minor );
	}
        if ( state->slaves <= 0 ){
            goto do_self;
        }
        state->iter_cursor = 0x1;         
    }else{
        state->iter_cursor <<= 1;
    }

    PDEBUGL(4)( "state->full 0x%04x state->slaves 0x%04x cursor 0x%04x\n",
                state->full_mask, state->slaves, state->iter_cursor         );

    for ( ; (state->iter_cursor&state->full_mask) != 0; state->iter_cursor <<= 1){
        if ( (state->iter_cursor&state->slaves) != 0 ){
            int iboard = 0;
            int isbit = 0x1;
            for ( ; (isbit&state->iter_cursor) == 0; isbit <<=1, ++iboard )
                ;

            PDEBUGL(4)( 
                "acq32_getNextSlave() state->slaves 0x%04x cursor 0x%04x board %d\n",
                state->slaves, state->iter_cursor, iboard           );

	    if ( iboard >= acq32_getDeviceCount() ){
		    PDEBUGL(4)(  "exceeded device count, return NULL\n" );
	        return NULL;
	    }else{
		struct Acq32Device* device = acq32_get_device_from_board( iboard );

		if ( device != path->device ){
			return device;
		}
	    }
        }
    }


 do_self:
    if ( !state->done_self ){
        state->done_self = 1;
        return path->device;
    }

    PDEBUGL(4)( 
        "acq32_getNextSlave() state->slaves 0x%04x cursor 0x%04x NULL\n",
        state->slaves, state->iter_cursor                   );

    return NULL;
}
/*
 * instantiate the master driver object
 */
#define DEV( minor )    acq32_get_device( minor )


static int num_enabled_channels( unsigned pmask )
{
    int nenabled = 0;
    unsigned mask;

    for ( mask = 0x80000000; mask; mask>>=1 ){
        if ( pmask & mask ){
            nenabled += 1;
        }
    }

    return nenabled;
}



//#define MAXTIMEOUT 100
#define MAXTIMEOUT   0x100000

static int acq32_devGetNumChannelsFromMask( struct Acq32Device* device )
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

static int acq32_devGetIndexFromMask( struct Acq32Device* device, int ichannel )
// returns index of channel in current mask, -1 is
// ichannel { 0..31 }
{
    int imatch = 1<<ichannel;
    int imask = 1;
    int ii = 0;

    for ( ii = 0; (imatch&imask) == 0; ++ii, imask <<= 1 ){
        ;
    }
    if ( (imatch&imask) != 0 ){
        return ii;
    }else{
        return -1;
    }
}

static int acq32_devPollAck( struct Acq32Device* device )
{
    unsigned itimeout;
    unsigned mask = BP_CI_ACK;
    unsigned ucommand;

    for ( itimeout = 0; itimeout != MAXTIMEOUT; ++itimeout ){
        device->get_mailbox( device, BP_MB_COMMAND, &ucommand );
        if ( ucommand&mask ){
            return 0;
        }else{
            if ( itimeout < 4 ){
                PDEBUGL(2)( "acq32_devPollAck() call to schedule STUBBED\n" );
            }
            if ( acq32_debug < 2 )
                schedule();
        }
    }
 
    return -ETIMEDOUT;
}

int acq32_waitInt( struct Acq32Device* device, int timeout )
{
    return interruptible_sleep_on_timeout( 
        &device->m_dpd.waitq, timeout   ) == 0L? -ETIMEDOUT: 0;
}

static int acq32_waitAuxMessage(  struct Acq32Device* device, int timeout )
{
    /* Original 2.4 used cli()/sti() to fence against the ISR; on 2.6 SMP
     * that doesn't actually exclude an ISR running on another CPU, but the
     * existing protocol (waitq + aux_mfa flag) behaves the same when this
     * pair is replaced literally.  TODO: convert to wait_event_*. */
    local_irq_disable();

    if ( device->m_dpd.aux_mfa != 0 ){
        local_irq_enable();
        return 0;
    }else{
        int rc;
                
        device->m_dpd.aux_task_waiting = 1;

        rc = interruptible_sleep_on_timeout( 
            &device->m_dpd.aux_waitq, timeout   ) == 0L? -ETIMEDOUT: 0;

        device->m_dpd.aux_task_waiting = 0;
        return rc;
    }
}

static int acq32_devAckInt( struct Acq32Device* device )
{
    unsigned long rv = acq32_waitInt( device, 100 );

    if ( rv < 0 ) {
        PDEBUGL(2)( "acq32_waitInt() timeout %ld\n", rv );
        return rv;
    }

    PDEBUGL(3)( "acq32_devAckInt rv %ld\n", rv );
    return acq32_devPollAck( device );
}

static int _acq32_devSendCommand(
    struct Acq32Device* device, 
    unsigned command,
    unsigned* a3,
    unsigned* a4
    )    
{
    int rv;
    
    MUTEX_DOWN( &device->m_dpd.mbox_mutex );
    
    command |= BP_CI_DONE+BP_CI_COMMAND;

    if ( a4 ){
        command |= BP_CI_A3;
        device->set_mailbox( device, BP_MB_A4, *a4 );
    }
    if ( a3 ){
        command |= BP_CI_A4;
        device->set_mailbox( device, BP_MB_A3, *a3 );
    }

    PDEBUGL(2)(  "device->use_interrupts %d\n", device->use_interrupts );
     
    if ( device->use_interrupts ){
        local_irq_disable();
        device->set_mailbox( device, BP_MB_COMMAND, command );
        PDEBUGL(2)(  "sent command, call acq32_devAckInt()\n" );
        rv = acq32_devAckInt( device );
    }else{
        device->set_mailbox( device, BP_MB_COMMAND, command );
        PDEBUGL(2)(  "sent command, call acq32_devPollAck()\n" );
        rv = acq32_devPollAck( device );
    }
    
    MUTEX_UP( &device->m_dpd.mbox_mutex );

    if (  acq32_busprot_debug ){
	char dbs[180], dbs2[80];

	sprintf( dbs, " M:%3d p:%p ", device->major, device->p_pci );
	sprintf( dbs2, "c:%08x", command );
		 strcat( dbs, dbs2 );
	if ( a3 ) {
	    sprintf( dbs2, " a3:%08x", *a3 );
	}else{
	    sprintf( dbs2, " a3:--------" );
	}
	strcat( dbs, dbs2 );
	if ( a4 ){
	    sprintf( dbs2, " a4:%08x", *a4 );
	}else{
	    sprintf( dbs2, " a4:--------" );
	}
	strcat( dbs, dbs2 );
	strcat( dbs, "\n" );
	PRINTBP( ">pci", dbs );
    }
    return rv;
}
      
int acq32_devSendCommand( struct Acq32Device* device, unsigned command )
// send command, return 0 on success
{
    return _acq32_devSendCommand( device, command, 0, 0 );
}


int acq32_setArmSpin( struct Acq32Device* device )
{
    unsigned command = BP_SET_FUNCODE(BP_FC_SET_SPIN)|BP_CI_DONE|BP_CI_COMMAND;
    device->set_mailbox( device, BP_MB_COMMAND, command );

    // give acq32 poller plenty of time to respond
    interruptible_sleep_on_timeout( &device->m_dpd.waitq, 20 );
    return 0;
}
 

int
acq32_devSendCommandA3A4(
    struct Acq32Device* device, 
    unsigned command,
    int nargs,                   // number of args's to send
    unsigned *a3,                // array of a3's to send
    unsigned *a4
    )
{
    int error = 0;

    for ( command |= BP_CI_A3; nargs; --nargs, ++a3, ++a4 ){
        if ( nargs == 1 ){
            command |= BP_CI_DONE;
        }           
        if ((error = _acq32_devSendCommand( device, command, a3, a4 )) != 0 ){
            break;
        }
    }

    return error;
}

static int acq32_devSetDistributor( 
    struct Acq32Device* device,
    int idist 
    )
{
    unsigned command = 
        BP_SET_FUNCODE(BP_FC_SET_DISTRIBUTOR) |
        BP_SET_A1( idist );
  
    return acq32_devSendCommand( device, command );
}

static int acq32_devSetRoute(
    struct Acq32Device* device,
    int route_id
    )
{
    unsigned command = 
        BP_SET_FUNCODE(BP_FC_SET_ROUTE) |
        BP_SET_A1( route_id );
  
    return acq32_devSendCommand( device, command );
}

#define NOPOLL 1

static int 
acq32_devSendQuery( 
    struct Acq32Device* device, 
    unsigned command, 
    unsigned* a1,
    unsigned* a2,
    unsigned* a3,
    unsigned* a4
    )
{
    int error = 0;

    PDEBUGL(3)( " command 0x%08x\n",  command|BP_CI_QUERY );

    MUTEX_DOWN( &device->m_dpd.mbox_mutex );
    
    if ( device->use_interrupts ){
        local_irq_disable();
        device->set_mailbox( device, BP_MB_COMMAND, command|BP_CI_QUERY );
        error = acq32_devAckInt( device );
    }else{
        device->set_mailbox( device, BP_MB_COMMAND, command|BP_CI_QUERY );
        error = acq32_devPollAck(device);
    }
    if ( !error ){
        if ( a1 || a2 ){
            unsigned response;
            
            device->get_mailbox( device, BP_MB_COMMAND, &response );
            if ( a1 ) *a1 = BP_GET_A1( response );
            if ( a2 ) *a2 = BP_GET_A2( response );
        }
        if ( a3 ) device->get_mailbox( device, BP_MB_A3, a3 );
        if ( a4 ) device->get_mailbox( device, BP_MB_A4, a4 );
    }
        
    MUTEX_UP( &device->m_dpd.mbox_mutex );

    
    return error;
}

static int acq32_devSendQueryWaitAuxMessage( 
    struct Acq32Device* device,
    unsigned command,
    int timeout
    )
// send the query, don't pollack, wait for aux message. error is timeout
{
    int error;
    
    if ( !device->use_interrupts ){
        return -ETIMEDOUT;
    }

    MUTEX_DOWN( &device->m_dpd.mbox_mutex );

    local_irq_disable();
    device->set_mailbox( device, BP_MB_COMMAND, command|BP_CI_QUERY );
    error = acq32_waitAuxMessage( device, timeout );
    
    MUTEX_UP( &device->m_dpd.mbox_mutex );
    
    return error;
}



enum STATE
acq32_devGetStatus( struct Acq32Device* device, enum MODE *mode )
{
    unsigned status;

    device->get_mailbox( device, BP_MB_STATUS, &status );

    if ( mode ){
        *mode = (enum MODE)BP_GET_MODE( status );

    }
    device->m_dpd.mode = (enum MODE)BP_GET_MODE(status);
    device->m_dpd.cycle = BP_GET_CYCLE(status);
    device->m_dpd.error = BP_GET_ERROR(status);
    return device->m_dpd.state = (enum STATE)BP_GET_STATE( status );
}


int 
acq32_devGetChannelMask( struct Acq32Device* device, unsigned* mask )
{
    return acq32_devSendQuery( 
        device,
        BP_SET_FUNCODE(BP_FC_GET_MASK),
        NULL, 
        NULL,
        mask,
        NULL
        );
}


int 
acq32_devSetChannelMask(struct Acq32Device* device, unsigned mask, int boost)
{
    unsigned actual_mask;
    int ierror =
        acq32_devSendCommandA3A4(
            device,
            BP_SET_FUNCODE(BP_FC_SET_MASK)+BP_FC_SET_MASK_ROW,
            1, &mask, &boost
            );

    if ( ierror ) return ierror;
    
    ierror = acq32_devGetChannelMask( device, &actual_mask );
    
    if ( !ierror ){
        if ( actual_mask != mask ){
            PDEBUGL(1)( "acq32_devSetChannelMask() mask not available: asked 0x%08x got 0x%08x\n",
                        mask, actual_mask );
        }
        device->m_dpd.channel_mask = actual_mask;
    }
    return ierror;
}


static int acq32_devSetStreaming( 
	struct Acq32Device* device, int npairs, int burst )
{
    unsigned mean = device->m_dpd.streaming&0x80000000? 1: 0;
    unsigned stride = device->m_dpd.streaming&0xffffU;
    unsigned a2 = BP_SET_A2(mean+BP_FC_SET_NPAIRS(npairs));
    unsigned command =
        BP_SET_FUNCODE(BP_FC_STREAM)|BP_SET_A1(stride)|a2;

    int ierror = acq32_devSendCommand( device, command );

    if ( ierror == 0 ){
        device->m_dpd.streaming_enabled = device->m_dpd.streaming != 0;
    }
    return ierror;
}


static int 
acq32_devFetchData( 
    struct Acq32Device* device, 
    long start, 
    void* buffer, int nsamples, int sample_size,
    unsigned ch_code,
    int is_pre_buffer
    )
// get required data from device, storing in buffer. return #samples buffered
    // this is HOST PULL technology
{
    unsigned command = BP_SET_FUNCODE(BP_FC_FETCH_DATA)|BP_SET_A2(ch_code);
    unsigned a3;
    int ierror;

    PDEBUGL(4)( "acq32_devFetchData start:0x%08lx %s\n", start,
                is_pre_buffer? "PREBUFFER": "" );

    if ( is_pre_buffer ){
        command |= BP_SET_A1(BP_A1_FETCH_PRETRIGGER);
    }
    if ( start > MAX_START_CODING ){
        command |= BP_SET_A1(BP_A1_START_BS_1K);
        start /= BS_1K;
    }

    ASSERT( start < MAX_START_CODING );         // WORKTODO
    ASSERT( nsamples < 0x10000 );

    TIMESTAMP( device, 0, "acq32_devFetchData( %d )", nsamples );
    
    a3 = (start<<16) | nsamples;

    PDEBUGL(2)( 
        "acq32_devFetchData: SendCommand c:0x%08x a3: 0x%08x\n",
        command, a3 );

    ierror = acq32_devSendCommandA3A4( device, command, 1, &a3, NULL_A4 );

    if ( ierror == 0 ){
        
        unsigned target_offset;
        unsigned maxsamples;
        int nbytes;
        
        TIMESTAMP( device, 0, "acq32_devFetchData - sendCommand done" );

        device->get_mailbox( device, BP_MB_A3, &target_offset );
        device->get_mailbox( device, BP_MB_A4, &maxsamples );

        PDEBUGL(4)( "acq32_devFetchData: ram:%p t:0x%08x m:0x%x\n", 
                    device->ram.va, target_offset, maxsamples );

        maxsamples = MIN( maxsamples, nsamples );
        nbytes = maxsamples*sample_size;
        
        memcpy_fromio( buffer, (char*)device->ram.va+target_offset, nbytes );

        TIMESTAMP( device, 0, "acq32_devFetchData - done" );

        PDEBUGL(4)( "acq32_devFetchData() returns %d\n", nsamples );

        return nsamples;
    }else{
        return ierror;
    }        
}

static int 
acq32_FetchDataFromDevice( 
    struct Acq32Path* path, 
    long start_sample,
    int cli_max_samples, 
    unsigned ch_code 
    )
{
    struct Acq32Device* device = path->device;

    
    int sample_size =  sizeof(short) *
        (ch_code == 0xff? acq32_devGetNumChannelsFromMask( device ): 1);
    int nsamples = path->buffer.max_data/sample_size;

    int prelen = path->device->m_dpd.nsamples_actual.pre;
    int postlen= path->device->m_dpd.nsamples_actual.post; 
    int is_pre_buffer = prelen > start_sample;

    long cli_last_sample;
    int iput;

    /*
     * special case of pre-trigger
     */

    if ( is_pre_buffer ){
        cli_last_sample = start_sample + cli_max_samples;
        cli_last_sample = MIN( cli_last_sample, prelen );
        nsamples        = MIN( nsamples, prelen-start_sample );
    }else{
        start_sample -= prelen;
        cli_last_sample = start_sample + cli_max_samples;
        cli_last_sample = MIN( cli_last_sample, postlen );
        nsamples        = MIN( nsamples, postlen-start_sample );
    }  

    PDEBUGL(4)( "acq32_FetchData s:%08lx max:%d nsamples:%d sz:%d "
                "BUF:%d, s:%08lx %d %d\n",
                start_sample, cli_max_samples, nsamples, sample_size, 
                path->buffer.max_data,
                path->buffer.start_sample,
                path->buffer.iget,
                path->buffer.iput
        );

    // if the data is already in the bag, return


    iput = (cli_last_sample - path->buffer.start_sample)*sample_size;

    if ( is_pre_buffer == path->buffer.is_pre_buffer  &&
         start_sample   >= path->buffer.start_sample   && 
         iput           <= path->buffer.iput                 ){

        nsamples = MIN( nsamples, cli_max_samples );

        PDEBUGL(4)( "acq32_FetchData - reusing buffer, ret %d\n", nsamples );

        return nsamples;
    }else{

        /*
         * set up an optimal size buffer 
         */
        if ( path->buffer.max_data >= PAGE_SIZE ){

            // this is normal - large buffer case ... dma compatible
             
            long start_bytes = start_sample*sample_size;
            
            start_bytes &= PAGE_MASK;

            path->buffer.start_sample = start_bytes/sample_size;
            path->buffer.iget = 
                (start_sample - path->buffer.start_sample)*sample_size;
        }else{

            // app may use a small buffer for low latency ...

            path->buffer.start_sample = start_sample;
            path->buffer.iget = 0;
        }
    }

    nsamples = acq32_devFetchData( 
        device, 
        path->buffer.start_sample,
        path->buffer.data, 
        nsamples, sample_size, ch_code ,
        is_pre_buffer
        );

    path->buffer.is_pre_buffer = is_pre_buffer;

    path->buffer.iput = nsamples*sample_size;

    PDEBUGL(4)( "acq32_FetchData() returns %d\n", nsamples );    

    return MIN( nsamples, cli_max_samples );
}

static int
acq32_FetchDataFromHostBuffer(
    struct Acq32Path* path, 
    long start_sample,
    int cli_max_samples, 
    unsigned ch_code 
    )
{
    struct Acq32Device* device = path->device;
    int totlen = device->m_dpd.nsamples_actual.pre +
        device->m_dpd.nsamples_actual.post;
    int rowlen = acq32_devGetNumChannelsFromMask( device );
    int sample_size =  sizeof(short) *  (ch_code == 0xff? rowlen: 1);

    int cli_last_sample = MIN( start_sample+cli_max_samples, totlen );
    int nsamples = path->buffer.max_data/sample_size;

    int iput = (cli_last_sample - path->buffer.start_sample)*sample_size;
    nsamples = MIN( nsamples, totlen-start_sample );

    // is it in the buffer already ??

    // if it's not in the buffer already , fetch it

    if ( !(start_sample >= path->buffer.start_sample &&
           iput         <= path->buffer.iput           )  ){


        // not in buffer, build up a buffer full 

        path->buffer.start_sample = start_sample;

        PDEBUGL(2)( "acq32_FetchDataHB s:%08lx max:%d nsamples:%d sz:%d " 
                    "BUF:%d, s:%08lx %d %d\n",
                    start_sample, cli_max_samples, nsamples, sample_size, 
                    path->buffer.max_data,
                    path->buffer.start_sample,
                    path->buffer.iget,
                    path->buffer.iput
            );


        if ( ch_code == 0xff ){
            int nlongs = nsamples*sample_size/sizeof(u32);
            u32* psrc = (u32*)&
                ((char*)device->bigbuf.bb.va)[sample_size*start_sample];
            u32* pdst = (u32*)path->buffer.data;

            while ( nlongs-- ){
                *pdst++ = *psrc++;
            }

        }else{
            int ncopy = nsamples;
            int stride = rowlen;
            int sample_offset = start_sample*stride;
            int chan_offset   = acq32_devGetIndexFromMask( device, ch_code );

            u16*psrc = 
                &((u16*)device->bigbuf.bb.va)[sample_offset+chan_offset];
            u16* pdst= (u16*)path->buffer.data;

            for( ; ncopy--; psrc += stride ){
                *pdst++ = *psrc;
            }
        }       

        path->buffer.iput = nsamples*sample_size;           
        path->buffer.iget = 0;

    }

    PDEBUGL(4)( "acq32FetchDataHB() returns %d\n", 
                MIN( cli_max_samples, nsamples ) );

    return MIN( cli_max_samples, nsamples );
}


static int 
acq32_FetchData( 
    struct Acq32Path* path, 
    long start_sample,
    int cli_max_samples, 
    unsigned ch_code 
    )
{
    if ( ch_code != 0xff ){
        if ( acq32_devGetIndexFromMask( path->device, ch_code ) == -1 ){
            return 0;
        }
    }
        
    if ( path->device->bigbuf.bb.len != 0 &&
         path->device->bigbuf.contains_valid_data == 1 ){
        return acq32_FetchDataFromHostBuffer(
            path, start_sample, cli_max_samples, ch_code 
            );
    }else{
        return acq32_FetchDataFromDevice(
            path, start_sample, cli_max_samples, ch_code 
            );
    }
}



static int
acq32_GetNextData( struct Acq32Path* path, void* buffer, int maxbuf )
// attempt to pull maxbuffer bytes from path buffer. return positive on success
{
    int itest = path->buffer.iget+maxbuf;

    if ( itest <= path->buffer.iput ){
        memcpy( buffer, &path->buffer.data[path->buffer.iget], maxbuf );
        path->buffer.iget = itest;

        PDEBUGL(5)( "acq32_GetNextData() returns %d\n", maxbuf );

        return maxbuf;
    }else{
        return 0;
    }
}

static void
acq32_discardLastData( struct Acq32Path* path, int data_len )
{
    path->buffer.iget -= data_len;
}

static enum STATE acq32_GetState( struct Acq32Path* path )
{
    if ( path->device->use_interrupts ){
        return path->device->m_dpd.state;
    }else{
        return acq32_devGetStatus( path->device, NULL );
    }
}

static int acq32_GetMemoryDepth( struct Acq32Path* path, int channel )
{
    int ichan = 0;
    int slaves = SLAVES( path->minor);
    int iboard;

    for ( iboard = 0;
          slaves != 0;
          ++iboard, slaves >>= 1                    ){
        if ( slaves&0x01 ){
  
            struct Acq32Device* p_dev = acq32_get_device_from_board( iboard );
            int ichannel_board;
            unsigned* pmask =  &p_dev->m_dpd.channel_mask;
            
            for ( ichannel_board = 0; 
                  ichannel_board!=p_dev->m_dpd.nchannels && ichan!=channel; 
                  ++ichannel_board, ++ichan ){
                if ( ichan == channel ){                    if ( *pmask&(1<<ichannel_board ) ){
                        int nenabled = num_enabled_channels( *pmask );

                        ASSERT( nenabled>0 );

                        return  p_dev->m_dpd.total_sample_length/nenabled;
                    }else{
                        return 0;
                    }
                }
            }
        }
    } 
    return -1;
}

static Message* getMessageBuffer( struct Acq32Path* path ) 
{
    ASSERT( sizeof( struct Acq32Path ) < 4096 );    // keep it less than one page
    ASSERT( sizeof( path->scratch ) > sizeof(Message) );
    
    PDEBUGL(2)("getMessageBuffer() sizeof struct Acq32Path:%d Message:%d\n", 
               sizeof(struct Acq32Path), sizeof(Message) );
                    
    return (Message*)path->scratch;
}

#define MFA_MASK 0x00ffffff

int acq32_sendI2O_kbuf( struct Acq32Path* path, Message* message )
{
    struct Acq32Device* device = path->device;
    unsigned mfa = readl( CSR( device, PCI_I2O_INBOUND_FIFO ) );

    
    if ( mfa == PCI_I2O_INVALID_MFA ) {
        PDEBUG( "ERROR: no i2o buffers available" );
        return -EACQ32_NO_INCOMING_I2O;
    }else{
        char* pdst = (char*)device->ram.va + (mfa&=MFA_MASK);

	PDEBUGL(2)(  "write Message %d to mfa 0x%08x\n", 
                path->device->m_dpd.i2o_last_id_out+1, mfa );
          
        message->header.id = MAKE_ID_DTACQ( ++device->m_dpd.i2o_last_id_out );

        PDEBUGL(3)( " message %4d %d 0x%08x 0x%04x 0x%04x\n",
                    message->header.id, 
                    message->header.length,
                    message->header.type,
                    message->payload.sdata[0], 
		    message->payload.sdata[1] );
                                                                                        
        memcpy_toio( 
            pdst, message, sizeof(MessageHeader)+message->header.length );

        writel( mfa, CSR( device, PCI_I2O_INBOUND_FIFO ) );
        device->m_dpd.i2o_packets_outgoing++;
        return 0;
    }
}



int acq32_sendI2O( struct Acq32Path* path, struct MESSAGE* arg )
{
    Message* message = getMessageBuffer( path );

    PDEBUGL(2)( " call copy_from_user( %p %p %d )\n", 
                &message->header, arg, sizeof(Message) );
    
    copy_from_user( &message->header, arg, sizeof(MessageHeader) );
    
    ASSERT( message->header.length < sizeof(Message)-sizeof(MessageHeader) );

        
    copy_from_user( 
	    &message->payload.cdata[0], 
	    &arg->payload.cdata[0], 
	    message->header.length );
    
    return acq32_sendI2O_kbuf( path, message );
}



static int acq32_SetModeAll( 
    struct Acq32Path* path, 
    enum MODE mode, 
    int samples, 
    int presamples,
    int trigger_DIx )
{
    SlaveIterState sis = {};
    struct Acq32Device* device;
    int error;

    unsigned command;

    ASSERT( mode==M_GATED_TRANSIENT  || 
            mode==M_GATED_CONTINUOUS ||
            mode==M_SOFT_TRANSIENT   ||
            mode==M_SOFT_CONTINUOUS  ||
            mode==M_TRIGGERED_CONTINUOUS     );


    if ( mode==M_GATED_CONTINUOUS && presamples!=-1 ){
        command = BP_SET_FUNCODE(BP_FC_SET_MODE_GC);
    }else if ( mode==M_TRIGGERED_CONTINUOUS && presamples!=-1 ){
        command = BP_SET_FUNCODE(BP_FC_SET_MODE_TC);
    }else{
        command = BP_SET_FUNCODE(BP_FC_SET_MODE);
    }

    if ( acq32_debounce_clk ){
	trigger_DIx |= BP_FC_SM_FLAGS_DEB;
    }
    if ( acq32_enable_emtrig_bit == 0 ){
	// NB reversing logic
	trigger_DIx |= BP_FC_SM_FLAGS_NEM;
    }

    while( (device = acq32_getNextSlave( path, &sis )) != NULL ){

        TIMESTAMP( device, 0, "acq32_setMode()" );
        device->m_dpd.mode = mode;
        device->m_dpd.nsamples_requested.post = samples;
        device->m_dpd.nsamples_requested.pre = presamples;


        
        error = acq32_devSendCommandA3A4( 
            device, 
            command|BP_SET_A1(mode)|BP_SET_A2(trigger_DIx),
            1, &samples, &presamples
            );

        TIMESTAMP( device, 0, "acq32_setMode() done" );
        if ( error ){
            return error;
        }
    }


    return 0;
}

void acq32_finish_with_engines( struct DriverPrivate* dp )
// common cleanup on finishing TimerTask
{
    dp->work_task.routine = 0;
    dp->state = ST_STOP;
    wake_up( &dp->waitq );
}

void acq32_restart_timer_task( struct DriverPrivate* dp )
// common timer restart func
{
    if ( MOD_IN_USE ){
#ifdef LINUX_NEW_PCI
        INIT_LIST_HEAD(&dp->work_task.list);
#else
        dp->work_task.next = 0; 
        dp->work_task.sync = 0;
#endif  
        queue_task( &dp->work_task, &tq_timer );
    }else{
        PDEBUGL(2)( "acq32_restart_timer_task - no apps, better quit\n" );
    }
}

static void acq32_BlockTillStoppedTask( void* args )
// polls for ST_STOP
{
    struct Acq32Device* device = (struct Acq32Device*)args;
    struct DriverPrivate* dp = &device->m_dpd;

    if ( acq32_devGetStatus( device, NULL ) == ST_STOP ){
        acq32_finish_with_engines( dp );
    }else{
        acq32_restart_timer_task( dp );
    }
}
        
static int acq32_BlockTillStopped( struct Acq32Device* device )
{
    struct DriverPrivate* dp = &device->m_dpd;

    INIT_TQUEUE( &dp->work_task, acq32_BlockTillStoppedTask, device );

    queue_task( &dp->work_task, &tq_timer );
    interruptible_sleep_on( &dp->waitq ); 
    return 0;
}







static int i2o_return_message( struct Acq32Device* device, unsigned mfa )
{
	device->i2o_return_mfa(device, mfa);
	device->m_dpd.i2o_packets_returned++;       
	return 0;
}

#ifdef PGMCOMOUT   // enable for strict mfa checking
#define i2o_return_message( device, mfa ) \
   duff_mfa( device, mfa, "ret" ) || i2o_return_message( device, mfa )
#endif

/*
 * Bottom Halves - ISR task level processing
 */

static void acq32_processCommandMessageReturn(
    struct Acq32Device* device,
    Message* message
    )
// called at bh (ISR-TASK) priority - make it quick
{
    struct ReturnCommandHandler* rch = device->m_dpd.first_rch;

    if ( rch == NULL ){
        PDEBUG(  " no handlers queued, discard\n" );
        return;
    }
        
    PDEBUGL(2)(  " locating handler for id %d\n", message->header.id );
        
    do {
        if ( rch->command_id == message->header.id && rch->handler ){
            PDEBUGL(2)(  " calling handler for id %d\n", message->header.id );
            rch->handler( rch, message );
            return;
        }
        rch = rch->next;
    }
    while( rch != device->m_dpd.first_rch );

/*
 * here if not taken by any handler ...
 */
    device->m_dpd.i2o_packets_discarded++;
    PDEBUG(  "ERROR - no handler available, discard\n" );
}



static void acq32_streaming_isr_bh( void* clidata )
// check the message and distribute to interested parties
// ideally we would have multiple buffers WORKTODO
{
	struct Acq32Device* device = (struct Acq32Device*)clidata;
	u32 mfa;

	TIMESTAMP( device, 'i', "bh" );

	PDEBUGL(4)(  "entry\n" );
    
	while ( (mfa = streamGet( &device->streambuf )) != 
		STREAM_MFA_INVALID ){
		Message *message = (Message*)device->i2o_mfa2va(device, mfa);

		PDEBUGL(2)(  " mfa 0x%08x type 0x%08x id %04d len %d\n", 
			    mfa, MT_ID(message->header.type), 
			    message->header.id,
			    message->header.length);

		if ( acq32_debug ==7 ){
			PDEBUGL(2)(  "SHORT CIRCUIT\n" );
			i2o_return_message( device, mfa );
			continue;
		}

		switch( MT_ID(message->header.type) ){
		case MT_STREAM:
			if ( device->m_dpd.streaming_enabled ){
				PDEBUGL(1)( " MT_STREAM:\n" );
				streamPut( &device->downstreambuf, mfa );
// user code dQ's data
				wake_up_interruptible( &device->m_dpd.waitq ); 
				return;
			}else{
				break;
			}

		case MT_COMMAND_PACKET:
			acq32_processCommandMessageReturn( device, message );
			break;
		default:
			PDEBUGL(1)( " default: id 0x%08x\n", 
				    MT_ID(message->header.type) );

            
			if ( device->m_dpd.aux_task_waiting ){
				device->m_dpd.aux_mfa = mfa;
				device->m_dpd.aux_task_waiting = 0;    
// no race, rather discard
				wake_up_interruptible(&device->m_dpd.aux_waitq);
				return;
			}else{
				device->m_dpd.i2o_packets_discarded++;
				PDEBUGL(1) (  "discard %x\n", mfa );
			}
		}
		i2o_return_message( device, mfa );              
	}
}


void acq32_incoming_i2o_isr( struct Acq32Device* device, u32 mfa )
{
        PDEBUGL(4)( " OUTBOUND FIFO 0x%08x\n", mfa );

        device->m_dpd.i2o_packets_incoming++;

	if ( acq32_debug ==8 ){
		PDEBUGL(2)(  "SHORT CIRCUIT\n" );
		i2o_return_message( device, mfa );
	}else if ( streamPut( &device->streambuf, mfa ) < 0 ){
		i2o_return_message( device, mfa );
		device->m_dpd.i2o_packets_discarded++;
		PDEBUGL(2)( " discarding buffer  0x%08x\n", mfa );

		TIMESTAMP( device, 'i', "disc" );
        }else{
		device->m_dpd.i2o_last_in = (void*)mfa;

		device->m_dpd.streaming_message_delta_usecs =
			acq32_getDeltaTime( 
				&device->m_dpd.tv_last_streaming_message,
				&device->m_dpd.tv_last_streaming_message  );
		TIMESTAMP( device, 'i', "put" );

		PDEBUGL(4)( " queued mfa 0x%08x\n", mfa );
            
		/* mark_bh(IMMEDIATE_BH) — 2.4 immediate-BH trigger; under the
		 * 2.6 shim queue_task() already maps to schedule_work, which
		 * runs from process context, so this line is redundant. */
		queue_task( &device->m_dpd.isr_bh, &tq_immediate );


		PDEBUGL(5)( " queue_task %s\n",
			    device->m_dpd.isr_bh.routine==
			    acq32_streaming_isr_bh?
			    "acq32_streaming_isr_bh": "dunno guv" 
			);      
        }
}


void acq32_doorbell_isr( struct Acq32Device* device, u32 doorbell )
// NB doorbell active LOW!
{
    if ( doorbell&BP_INT_COMMAND_ACK ){
	PDEBUGL(3)(  "BP_INT_COMMAND_ACK waitq %p\n", 
		    &device->m_dpd.waitq );

	wake_up_interruptible( &device->m_dpd.waitq );
    }
    if ( (doorbell&BP_INT_STATUS_CHANGE) && 
	 device->m_dpd.cos.enable && !device->m_dpd.cos.inhibit ){

	unsigned status = STATBUF_SET( jiffies, 
				       device->m_dpd.error, 
				       device->m_dpd.state );
	PDEBUGL(3)(  "BP_INT_STATUS_CHANGE task %p\n",
		    &device->m_dpd.cos.task );

	statbufPut( &device->statusbuf, status );
/*
 * WORKTODO ... need to Q the state
 */
	schedule_task( &device->m_dpd.cos.task );
    }

    PDEBUGL(2)( " was here! 0x%08x\n", doorbell );
}




void acq32_validate_bigbuf( 
    struct Acq32Device* device, unsigned set_valid )
{
    if ( set_valid > 1 ){
	// giant frig to let clients get at the buffer data
	device->m_dpd.nsamples_actual.post = set_valid;
    }
    device->bigbuf.contains_valid_data = set_valid!=0;
    
}

static int acq32_SetArmAll( struct Acq32Path* path )
{
    SlaveIterState sis = {};
    struct Acq32Device* device;
    int error = 0;

    while( (device = acq32_getNextSlave( path, &sis )) != NULL ){
        error = acq32_devSendCommand( device, BP_SET_FUNCODE(BP_FC_SET_ARM));
       
        if ( error ){
            return error;
        }
        acq32_validate_bigbuf( device, 0 );
    }

    return error;
}

static int acq32_SetAbortAll( struct Acq32Path* path )
{
    SlaveIterState sis = {};
    struct Acq32Device* device;
    int error = 0;

    while( (device = acq32_getNextSlave( path, &sis )) != NULL ){
        error = acq32_devSendCommand( device, BP_SET_FUNCODE(BP_FC_SET_ABORT));
       
        if ( error ){
	    break;
        }
    }

    return error;
}

static int acq32_BlockTillStoppedAll( struct Acq32Path* path )
{
    struct Acq32Device* device;
    SlaveIterState sis = {};
    int error = 0;

    while( (device = acq32_getNextSlave( path, &sis )) != NULL ){
	if ( acq32_devGetStatus( device, NULL ) != ST_STOP ){
	    error = acq32_BlockTillStopped( device );
	    if ( error != 0 ){
		PDEBUG(  " blockTillStopped failed\n" );
		break;
	    }        
	}
    }
    return error;
}


static int acq32_devGetCycle( struct Acq32Device* device )
{
    acq32_devGetStatus( device, 0 );
    return device->m_dpd.cycle;
}

#define WDT_POLL_JIFFIES (HZ/10)
#define WDT_DONE_JIFFIES (HZ)

static void set_arm_wdt( void* arg ) 
/*
 * If time to poll, poll
 * If error, kick and drop out
 * Else if past time, drop out
 * Else restart
 */
{
    struct Acq32Device* device = (struct Acq32Device*)arg;
    struct ArmWdtStruct* wdt = &device->m_dpd.set_arm;
    
    if ( jiffies- wdt->last_poll_jiffies > WDT_POLL_JIFFIES ){
	
	acq32_devGetStatus( device, 0 );

	if ( device->m_dpd.error ){
	    int e;
	    // WRKTODO - use MUTEX??
	    e = acq32_devSendCommand( device, BP_SET_FUNCODE(BP_FC_SET_ARM));
	    PDEBUGL(1)( "kickit after %ld ok? %d\n", 
		    jiffies - wdt->start_jiffies, e );
	    wdt->busy = 0;
	    return;
	}else if ( jiffies - wdt->start_jiffies > WDT_DONE_JIFFIES ){
	    PDEBUGL(1)(  "signing off\n" );
	    wdt->busy = 0;
	    return;
	}else{
	    PDEBUGL(2)(  "poll\n" );
	    wdt->last_poll_jiffies = jiffies;
	}
    }
/*
 * default: restart
 */
    schedule_task( &wdt->watchdog_task );    
}

static void set_arm_wdt_launch( struct Acq32Device* device ) {
    struct ArmWdtStruct* wdt = &device->m_dpd.set_arm;

    INIT_TQUEUE( &wdt->watchdog_task, set_arm_wdt, device );
    wdt->last_poll_jiffies = wdt->start_jiffies = jiffies;
    schedule_task( &wdt->watchdog_task );
}


#define CYCINC( n ) ((n+1)&0xf)
static int acq32_GoAll( struct Acq32Path* path )
{
    int error = 0;
    struct Acq32Device* device = path->device;
    int major = device->major;
    int c0, c1, c2;             // false start debug NB !ALL compatible
    int retry = 0;
    int startok = 0;


    // WORKTODO - use a spinlock
    if ( device->m_dpd.set_arm.busy ){
	return -EBUSY;
    }else{
	device->m_dpd.set_arm.busy = 1;
    }

    c0 = acq32_devGetCycle( device );

    if ( acq32_double_tap ){
	device->m_dpd.cos.inhibit = 1;
    }
    if ( (error = acq32_SetArmAll( path )) != 0 ){
        PDEBUG(  " first setArm failed\n" );
        return error;
    }

    c1 = acq32_devGetCycle( device );


    while ( !startok && retry<2 && (acq32_double_tap&1) ){
        if ( (error = acq32_SetAbortAll( path )) != 0 ){
            PDEBUG(  " setAbort failed\n" );
            break;;
        }else{
	    PDEBUGL(1)(  "setAbort() done %d\n", major );
	}
	if ( (error = acq32_BlockTillStoppedAll( path )) != 0 ){
	    PDEBUG(  " blockTillStopped (1) failed\n" );
	    break;
	}else{
	    PDEBUGL(1)(  "blockTillStopped (1) OK %d\n", major );
	}

	device->m_dpd.cos.inhibit = 0;

        if ( (error = acq32_SetArmAll( path )) != 0 ){
            PDEBUG(  " second setArm failed\n" );
            break;    
        }else{
	    PDEBUGL(1)(  "setArm() (2) OK %d\n", major );
	}

	c2 = acq32_devGetCycle( device );

	if ( !(startok = CYCINC(c0)==c1 && CYCINC(c1)==c2) ){
	    PDEBUG(  "cycle mismatch %d %d %d\n", c0, c1, c2 );
	    if ( ++retry < 2 ){
		PDEBUG(  "retry\n" );
	    }else{
		PDEBUG(  "retry failed, dop out\n" );
	    }
	}
    }
    
    device->m_dpd.cos.inhibit = 0;

    if ( acq32_double_tap&2 ){
	set_arm_wdt_launch( device );
    }else{
	device->m_dpd.set_arm.busy = 0;
    }
    if ( path->is_blocking ){     
	if ( (error = acq32_BlockTillStoppedAll( path )) != 0 ){
	    PDEBUG(  " blockTillStopped (2) failed\n" );
	    return error;
	}	
    }

    return error;
}

static int acq32_GetNumSamples( struct Acq32Path* path )
{
    acq32_devSendQuery( 
        path->device, 
        BP_CI_DONE|BP_SET_FUNCODE(BP_FC_GET_NUMSAMPLES ),
        NULL_A1, 
        NULL_A2,
        &path->device->m_dpd.nsamples_actual.post,
        &path->device->m_dpd.nsamples_actual.pre
        );

    path->device->m_dpd.itrigger = path->device->m_dpd.nsamples_actual.pre;
    return path->device->m_dpd.nsamples_actual.post+
        path->device->m_dpd.nsamples_actual.pre;
}

static int acq32_SetChannelMaskAll( 
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
            *pmask |= *smask++ == '1'? 1<<ichannel_board: 0;

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

static int acq32_GetNumChannelsAll( struct Acq32Path* path )
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

            nchannels += acq32_devGetNumChannelsFromMask( device );
        }
    }

    return nchannels;
}


static int acq32_OnOpen( struct Acq32Path* path )
{
	struct Acq32Device *device = path->device;

	PDEBUGL(2)( "acq32_OnOpen() %d\n", device->use_interrupts );

	if (device->use_interrupts){
		acq32_devSendCommand( 
			device, 
			BP_SET_FUNCODE(BP_FC_SET_INTERRUPTS)+BP_SET_A1(3)
			);

		/* this need not happen every open, but must happen for
		 * every instance of acq200control up top ... 
		 * so this is ott but safe
		 */
		/* MUST re-use the DMABUF as it is in the window.
		 * wont' get passed to get_free_pages as dmabuf.len = 0
		 * this is all fairly dodgy => WORKTODO
		 */

		/* BUG WORKTODO: assumes always use interrupts! 
                 * given use of DMA, probably safe assumption...
                 */

		acq32_devSendCommandA3A4( 
			device,
			BP_SET_FUNCODE(BP_FC_SET_HOST_DMABUF),
			1,
			(unsigned*)&device->dmabuf.pa,
			(unsigned*)&device->dmabuf.len );

		/*
                 * # channels won't change from code to code
                 * hmmm, but VRange might!!
                 */
		if (device->input_channels.nchannels == 0 &&
		    device->output_channels.nchannels == 0   ){
			acq32_GetAvailableChannels(path);
			acq32_GetRange(path);
		}

		acq32_devGetStatus(device, NULL);
	}

	return 0;
}

static int acq32_OnRelease( struct Acq32Path* path )
{
    struct Acq32Device* device = path->device;

    PDEBUGL(2)( "acq32_OnRelease() %d\n", path->device->use_interrupts );

    if ( device->m_dpd.streaming_enabled ){
        device->m_dpd.streaming = 0;
        acq32_devSetStreaming( device, 0, 0 );
    }
    if ( device->use_interrupts ){
        device->use_interrupts = 0;     // answer will be polled!
        acq32_devSendCommand( 
            path->device, 
            BP_SET_FUNCODE(BP_FC_SET_INTERRUPTS)+BP_SET_A1(0));
    }

    return 0;
}

static int acq32_SetInternalClockAll( struct Acq32Path* path )
{
    SlaveIterState sis = {};
    struct Acq32Device* device;
    int error;
    unsigned freq = path->device->m_dpd.clock.internal;
    u8 doX = path->device->m_dpd.clock.doX;

    while( (device = acq32_getNextSlave( path, &sis )) != NULL ){
        error = acq32_devSendCommandA3A4( 
            device, 
            BP_SET_FUNCODE(BP_FC_SET_INTCLOCK)+BP_SET_A2(doX),
            1, &freq, NULL_A4                              
            );
        if ( error ){
            return error;
        }

        error = acq32_devSendQuery(
            device,
            BP_CI_DONE|BP_SET_FUNCODE(BP_FC_GET_INTCLOCK),
            NULL_A1, 
            NULL_A2,
            &device->m_dpd.clock.internal,
            NULL_A4    
            );
        if ( error ){
            return error;
        }
    }

    return 0;
}
static int acq32_SetExternalClockAll( struct Acq32Path* path )
{
    SlaveIterState sis = {};
    struct Acq32Device* device;
    int error;
    unsigned divisor = path->device->m_dpd.clock.divisor;
    u8 diX = path->device->m_dpd.clock.diX;
    u8 doX = path->device->m_dpd.clock.doX;

    while( (device = acq32_getNextSlave( path, &sis )) != NULL ){
        error = acq32_devSendCommandA3A4( 
            device, 
            BP_SET_FUNCODE(BP_FC_SET_EXTCLOCK)+BP_SET_A1(diX)+BP_SET_A2(doX),
            1, &divisor, NULL_A4                           
            );
        if ( error ){
            return error;
        }
    }

    return 0;
}                                 

static int acq32_SetSampleTaggingAll( struct Acq32Path* path )
{
    SlaveIterState sis = {};
    struct Acq32Device* device;
    int error;

    while( (device = acq32_getNextSlave( path, &sis )) != NULL ){
        error = acq32_devSendCommand( 
            device, 
            BP_SET_FUNCODE(BP_FC_SET_TAGGING)|
            BP_SET_A1(path->device->m_dpd.sample_tagging) 
            );
       
        if ( error ){
            return error;
        }
    }

    return 0;
}

static int acq32_SetDIO( struct Acq32Path* path )
    // valid for single board only
{
    int error = acq32_devSendCommandA3A4( 
        path->device, 
        BP_SET_FUNCODE(BP_FC_SC_SET_DIO),
        1, 
        &path->device->m_dpd.dio_dir,
        &path->device->m_dpd.dio_outputs 
        );
    return error;
}

        
static int acq32_GetDIO( struct Acq32Path* path )
// WORKTODO ... valid for one board ONLY
{
	acq32_devSendQuery(
		path->device,
		BP_CI_DONE|BP_SET_FUNCODE(BP_FC_SC_GET_DIO),
		NULL_A1,
		NULL_A2,
		&path->device->m_dpd.dio_inputs,
		NULL_A4
		);
    
	return 0;
}

static int dev_getAvailableChannels( struct Acq32Device* device )
{
	return acq32_devSendQuery(
		device,
		BP_CI_DONE|BP_SET_FUNCODE(BPFC_GET_NCHANNELS),
		&device->input_channels.nchannels, 
		&device->output_channels.nchannels,
		NULL_A3,
		NULL_A4
		);

}
static int acq32_GetAvailableChannels( struct Acq32Path* path )
{
    return dev_getAvailableChannels( path->device );
}

static void code2v10( 
    struct CHANNEL_INFO* ci,
    enum BPFC_RANGE code
    )   
{
#define V10( vv ) ((int)((vv)*10))
    switch( code ) {
    case BPFC_RANGE_UNKNOWN:
    default:
        ci->vmax10 = 
            ci->vmin10 = 0;
        break;
    case BPFC_RANGE_10_10:
        ci->vmax10 = V10( 10 );
        ci->vmin10 = V10( -10 );
        break;
    case BPFC_RANGE_05_05:       
        ci->vmax10 = V10( 5 );
        ci->vmin10 = V10( -5 );
        break;
    case BPFC_RANGE_02_02:
        ci->vmax10 = V10( 2 );
        ci->vmin10 = V10( -2 );
        break;
    case BPFC_RANGE_01_01:
        ci->vmax10 = V10( 1 );
        ci->vmin10 = V10( -1 );
        break;
    case BPFC_RANGE_2p5_2p5:
        ci->vmax10 = V10( 2.5 );
        ci->vmin10 = V10( -2.5 );
        break;
    }
    
    PDEBUGL(2)(  " code %d min:%d max:%d\n", code, ci->vmax10, ci->vmin10 );
}

static int dev_getRange( struct Acq32Device* device )
{
    unsigned vin_code, vout_code;
    
    int ierrno = acq32_devSendQuery(
        device,
        BP_CI_DONE|BP_SET_FUNCODE(BPFC_GET_RANGE),
        &vin_code, 
        &vout_code,
        NULL_A3,
        NULL_A4
        );
        
    code2v10( &device->input_channels,  vin_code ); 
    code2v10( &device->output_channels, vout_code );
    return ierrno;    
}

static int acq32_GetRange( struct Acq32Path* path )
{
    return dev_getRange( path->device );    
}
static int acq32_SetStreaming( struct Acq32Path* path, int npairs, int burst )
{
    return acq32_devSetStreaming( path->device, npairs, burst );
}

static int acq32_SetDistributor( struct Acq32Path* path, int idist )
{
    return acq32_devSetDistributor( path->device, idist );
}

static int acq32_SetRoute( struct Acq32Path* path, int iroute )
{
    int routecode;

    switch( iroute ){
    case 0:
        routecode = BP_FC_SET_MASK_RAW;     break;
    case 1:
    default:
        routecode = BP_FC_SET_MASK_ROW;     break;
    case 2:
        routecode = BP_FC_SET_MASK_CHAN;    break;
    }
    return acq32_devSetRoute( path->device, routecode );
}

static int acq32_SetChannel( 
    struct Acq32Path* path,
    int ichan, int g1, int g2, int vex )
    // valid for one board only
{
    unsigned cc = 
        BYTE_ENCODE_BIT( BP_FC_SC_SET_CHANNEL_CH_LSB,    ichan )|
        BYTE_ENCODE_BIT( BP_FC_SC_SET_CHANNEL_G1_LSB,    g1    )|
        BYTE_ENCODE_BIT( BP_FC_SC_SET_CHANNEL_G2_LSB,    g2    )|
        BYTE_ENCODE_BIT(  BP_FC_SC_SET_CHANNEL_V_EX_LSB, vex   );

    return acq32_devSendCommandA3A4(
        path->device,
        BP_SET_FUNCODE(BP_FC_SC_SET_CHANNEL),
        1, &cc, NULL_A4                               );
}

static int acq32_printAuxReply( struct Acq32Path* path )
{
    struct Acq32Device* device = path->device;
    unsigned mfa     = device->m_dpd.aux_mfa;
    Message* message = (Message*)device->i2o_mfa2va(device, mfa);
    char* printable  = message->payload.cdata;
    int   len        = MIN( strlen( printable ), MESSAGE_LEN );
  
    acq32_path_readbuffer_put( path, printable, len );

    printable[len] = '\0';
    PDEBUGL(1)( "acq32_printAuxReply() \"%s\"\n", printable );

    i2o_return_message( device, mfa );
    device->m_dpd.aux_mfa = 0;
    return 0;
}

static int acq32_GetInfo( struct Acq32Path* path,  enum StatusSelect selector )
{
    int error;
    int func;
    
    switch ( selector ) {
    case SS_FW_REV:
    default:
        func = BP_FC_GET_FWREV;
        break;
    case SS_CONFIG:
        func = BP_FC_GET_CONFIG;
        break;
    case SS_CALINFO:
        func = BP_FC_GET_CALINFO;
        break;
    case SS_CAPTURESTATS:
        func = BP_FC_GET_CAPTURESTATS;
        break;
    case SS_DEBUG:
        func = BP_FC_DEBUG_GET_REGS;
        break;
    }
    error = acq32_devSendQueryWaitAuxMessage(
        path->device, 
        BP_CI_DONE|BP_SET_FUNCODE(func),
        200 
        );

    if ( error < 0 ){
        return error;
    }else{
        return acq32_printAuxReply( path );
    }
}

static int acq32_DoBigdump( struct Acq32Path* path )
{
    if ( acq32_big_buf_len == 0 ){
        return -1;
    }else{
        int error = acq32_devSendCommandA3A4(
            path->device,
            BP_SET_FUNCODE(BP_FC_BIG_DUMP),
            1,
            (unsigned*)&path->device->bigbuf.bb.pa,
            (unsigned*)&path->device->bigbuf.bb.len  );
        if ( error != 0 ){
            return error;
        }else{
            error = acq32_waitAuxMessage( path->device, 1000 );

            if ( error < 0 ){
                return error;
            }else{
                if ( path->device->bigbuf.bb.len != 0 ){
		    acq32_validate_bigbuf( path->device, 1 );
                }
                return acq32_printAuxReply( path );
            }
        }
    }
}


static int acq32cpci_SetSyncRoute(
    struct Acq32Path* path,
    int func,
    unsigned dest_mask
    )
{
    unsigned command = BP_SET_FUNCODE(BP_FC_CPCI_SET_ROUTE)|
        BP_SET_A1( func ) |
        BP_SET_A2( dest_mask );
                                
    PDEBUGL(1)( "acq32cpci_SetSyncRoute( %d, %02x )\n", func, dest_mask );
    
    return acq32_devSendCommand( path->device, command );
}


static int acq32_SetCal( THIS, int ical )
{
    unsigned command = BP_SET_FUNCODE(BP_FC_SELECT_CALSET)+BP_SET_A1(ical);
                                
    PDEBUGL(1)( "acq32_SetCal( %d ) STUB\n", ical );
    
    return acq32_devSendCommand( path->device, command );
}

static int acq32_WaitState( THIS, int state )
{
    PDEBUGL(1)( "acq32_WaitState( %d ) STUB\n" , state );
    return 0;
}


static unsigned make_gut_cmd( unsigned cmd, enum FX fx, enum SUBJECT subject )
{
    unsigned a2 = 0;
    
    switch( fx ){
    case FX_AI:
        a2 = BP_GUT_FC_FX_AI; break;
    case FX_AO:
        a2 = BP_GUT_FC_FX_AO; break;
    case FX_DO:
        a2 = BP_GUT_FC_FX_DO; break;
    default:
        ASSERT( 0 );
    }
    if ( subject&PH_REQUEST ){
        a2 |= subject & ~PH_REQUEST;
    }else if ( subject&EV_REQUEST ){
        a2 |= BP_GUT_FC_EV + (subject & ~EV_REQUEST);
    }else{
        ;
    }
    return BP_SET_FUNCODE(cmd)+BP_SET_A2( a2 );
}

static int acq32_SetPhase( THIS, enum FX fx, enum SUBJECT ph, int nsamples )
{
    unsigned command = make_gut_cmd( BP_GUT_SETPHASE, fx, ph );
                       
    int error = acq32_devSendCommandA3A4( 
        path->device, 
        command,
        1, 
        &nsamples,
        NULL 
        );
                       
    PDEBUGL(1)( "acq32_SetPhase( %d, 0x%02xm %d ) returns %d\n", 
                fx, ph, nsamples, error );
                                
    return error;
}

static int acq32_GetPhase( 
    THIS, enum FX fx, enum SUBJECT ph, enum PHASE_PROPERTY property )
{
    unsigned query = BP_CI_DONE +
        make_gut_cmd( BP_GUT_GETPHASE, fx, ph )+
        BP_SET_A1(property);
                             
    unsigned value;
    int error = acq32_devSendQuery( 
        path->device, query,
        NULL, NULL, &value, NULL );

    PDEBUGL(1)( "acq32_GetPhase( %d, 0x%02x, %d ) value %d error %d\n", 
                fx, ph, property, value, error );
    return value;
}

static int acq32_SetEvent(
    THIS, enum FX fx, enum SUBJECT ev, struct EventConditionDef* ecd )
{
    unsigned command = make_gut_cmd( 0, fx, ev );
    unsigned a3 = 0, a4 = 0;
    int error;
    
    switch ( ecd->ec ){
    case EC_TRIGGER_RISING:
    case EC_TRIGGER_FALLING:
        a3 = ecd->prams.trigger_bit;    // fall thru
    default:
        command += BP_SET_FUNCODE(BP_GUT_SETEVENT)+BP_SET_A1(ecd->ec);
        break;
    case EC_DATA_ABOVE:
    case EC_DATA_BELOW:
                
        //     threshold is not a GUT command at all ...
        // so how to put it in phases ???
        // answer - it is ONLY valid at E2
        command = BP_SET_FUNCODE(BP_FC_SET_DATA_THRESHOLD)+
            BP_SET_A1(ecd->prams.th.channel);
        a3 = ecd->prams.th.below;
        a4 = ecd->prams.th.above;
        break;
    }
                
    error = acq32_devSendCommandA3A4( 
        path->device, 
        command,
        1, 
        &a3,
        &a4 
        );
                   
    PDEBUGL(1)( "acq32_SetEvent( %d, 0x%02x, %d ) returns %d\n" , 
                fx, ev, ecd->ec, error );
                            
    return error;
}     

static int acq32_GetEvent( 
    THIS, enum FX fx, enum SUBJECT event
    )
{
    unsigned query = BP_CI_DONE +
        make_gut_cmd( BP_GUT_GETEVENT, fx, event )+
        BP_SET_A1(0);
             
    unsigned type;
    unsigned value;
    acq32_devSendQuery( 
        path->device, query,
        &type, NULL, &value, NULL );

    return type<<8|value;
}

static int acq32_FireEvent(THIS, enum FX fx, enum SUBJECT ev )
{
    unsigned command = make_gut_cmd( BP_GUT_FIREEVENT, fx, ev );
                       
    int error = acq32_devSendCommand( path->device, command );
                       
    PDEBUGL(1)( "acq32_FireEvent( %d, 0x%02x ) returns %d\n", 
                fx, ev, error );
                                
    return error;
}

static int acq32_SetClock( THIS, enum FX fx, int clk_source )
{
    unsigned command = make_gut_cmd(BP_GUT_SETCLOCK, fx, 0 ) + 
        BP_SET_A1(clk_source);       
    
    int error = acq32_devSendCommand( path->device, command );
                                
    PDEBUGL(1)( "acq32_SetClock( %d, %d ) returns %d\n", fx, clk_source, error );
    
    return error;
}

static int acq32_ClockNow( THIS )
{
    unsigned command = BP_SET_FUNCODE(BP_GUT_CLOCKNOW);      
    int error = acq32_devSendCommand( path->device, command );
                                
    return error;
}

static int acq32_ResetGut( THIS )
{
    unsigned command = BP_SET_FUNCODE(BP_GUT_RESET);
    int error = acq32_devSendCommand( path->device, command );
    
    return error;
}
static int acq32_ReserveAO( THIS, int nreserve )
{
    int error = acq32_devSendCommandA3A4( 
        path->device, 
        BP_SET_FUNCODE(BP_FC_RESERVE_AO),
        1, 
        &nreserve,
        NULL
        );
                                
    PDEBUGL(1)( "acq32_ReserveAO( %d ) returns %d\n", nreserve, error );
    
    return error;
}


static int acq32_SetThreshold( 
    THIS, 
    enum ThresholdSelect channel, 
    short trigger_above, short trigger_below 
    )
{
    int error =-1;
    u32 ta, tb;
    int distsel = 0;
    
    switch( channel ){
    case THS_ARM_DATA_SINGLE:
        distsel = BP_FC_SET_DATA_TRIGGER; 
        break;
    case THS_ARM_DATA_MULTI:
        distsel = BP_FC_SET_DATA_TRIGGER_MULTI;
        break;
    case THS_ARM_EDGE_SINGLE:
        distsel = BP_FC_SET_DATA_EDGE_TRIGGER;
        break;
    case THS_ARM_EDGE_MULTI:
        distsel = BP_FC_SET_DATA_EDGE_TRIGGER_MULTI;
        break;
    case THS_ARM_FAST_MULTI:
        distsel = BP_FC_SET_DATA_FAST_TRIGGER_MULTI;
        break;
    case THS_ALL:
        channel = 'X';            // fall thru
    default:
        ta = trigger_above;
        tb = trigger_below;
                    
        PDEBUGL(1)( "acq32_SetThreshold( %d, %d ) set 0x%08x 0x%08x\n",
                trigger_above, trigger_below, ta, tb );
                                
        error = acq32_devSendCommandA3A4(
            path->device,
            BP_SET_FUNCODE(BP_FC_SET_DATA_THRESHOLD)+
            BP_SET_A1(channel),
            1, &tb, &ta
            );
        break;
    }
        
    if ( distsel ){
        error = acq32_devSendCommand( 
            path->device,
            BP_SET_FUNCODE(BP_FC_SET_DISTRIBUTOR)+BP_SET_A1(distsel)
            );
    }
    return error;
}       
    

static int acq32_SetUser( THIS, int user_led, enum USERLED mode )
{
    u32 a3;
    int error;
#define ENCODE( ul, mode ) ((ul)*10+mode)

    switch( ENCODE( user_led, mode ) ){
    case ENCODE( 3, UL_OFF ):
        a3 = BP_FC_USER_LED3_OFF; break; 
    case ENCODE( 3, UL_ON ):
        a3 = BP_FC_USER_LED3_ON; break;
    case ENCODE( 3, UL_FLASH ):
        a3 = BP_FC_USER_LED3_FLIP; break;
    case ENCODE( 4, UL_OFF ):
        a3 = BP_FC_USER_LED4_OFF; break;
    case ENCODE( 4, UL_ON ):
        a3 = BP_FC_USER_LED4_ON; break;
    case ENCODE( 4, UL_FLASH ):   
        a3 = BP_FC_USER_LED4_FLIP; break;
    default:
        PDEBUGL(1)( "acq32_SetUser() bad opts %d %d\n", user_led, mode );
        return -1;
    }
    error = acq32_devSendCommandA3A4(
        path->device,
        BP_SET_FUNCODE(BP_FC_USER),
        1,
        &a3,
        NULL
        );
                                
    PDEBUG( "acq32_setUser( %d, %d ) cmd:0x%08x a3:0x%08x returns %d\n", 
            user_led, mode,BP_SET_FUNCODE(BP_FC_USER), a3, error );
    return error;
}


int acq32_WaitEvent( THIS, struct ACQ32_WAIT_DEF* wait_def )
{
    PDEBUG( "acq32_WaitEvent( %p, %p ) STUB\n", path, wait_def );
    
    // build an rch, queue the command and sleep
    return -1; // WORKTODO!!!
}


struct Acq32MasterDriver* acq32_getDriver()
{
    static struct Acq32MasterDriver _driver = {
        acq32_GetState,                 // GetState
        acq32_SetChannelMaskAll,        // SetChannelMask
        acq32_GetMemoryDepth,           // GetMemoryDepth
        acq32_SetModeAll,               // SetMode
        acq32_GoAll,              // SetArmed
        acq32_SetAbortAll,              // SetAbort
        acq32_GetNumSamples,            // GetNumSamples
        acq32_GetNumChannelsAll,        // GetNumChannels
        acq32_OnOpen,                   // int (*OnOpen)( struct Acq32Path* path );
        acq32_OnRelease,                // int (*OnRelease)( struct Acq32Path* path );

        acq32_SetInternalClockAll,        
        acq32_SetExternalClockAll,
        acq32_SetSampleTaggingAll,

        acq32_SetDIO,
        acq32_GetDIO,
        acq32_SetChannel,
        acq32_SetStreaming,

        acq32_GetInfo,
        acq32_DoBigdump,

        acq32_SetDistributor,
        acq32_SetRoute,
        
        acq32cpci_SetSyncRoute,
            
        acq32_SetCal,
        acq32_WaitState,
            
        acq32_SetPhase,
        acq32_GetPhase,
        acq32_SetEvent,
        acq32_GetEvent,
        acq32_FireEvent,
        acq32_SetClock,
        acq32_ClockNow,
        acq32_ResetGut,
               
        acq32_ReserveAO,
        acq32_SetUser,
            
        acq32_SetThreshold,
            
        acq32_GetAvailableChannels,
        acq32_GetRange,
    };
    
    struct Acq32MasterDriver* new_vtable = 
	    KMALLOC(sizeof(struct Acq32MasterDriver), GFP_KERNEL );
    memcpy(new_vtable, &_driver, sizeof(struct Acq32MasterDriver));

    return new_vtable;
}


static void beFix( char* buf, int nshorts )
{
    int ishort;
    unsigned char* ubuf = (unsigned char*)buf;
    unsigned char utmp;
    
    for ( ishort = 0; ishort != nshorts; ++ishort ){
        SWAP( ubuf[ishort*2], ubuf[ishort*2+1], utmp );
    }
}

static ssize_t bin_buf_update( 
    struct Acq32Path* path,
    char* buf,
    int max_samples,
    int sample_size
    )
// binary update of client buffer buf. returns #chars added to buf
{
    ssize_t tcount = max_samples*sample_size;

    TIMESTAMP( path->device, 1, "bin_buf_update( %d )", tcount );    
    copy_to_user( buf, path->buffer.data+path->buffer.iget, tcount );
    path->buffer.iget += tcount;

    TIMESTAMP( path->device, 1, "bin_buf_update() - done" );
    return tcount;
}

ssize_t acq32_channel_read ( 
    struct file* filp, char* buf, size_t count, loff_t* posp 
    )
{
    int channel = CHANNEL( PD(filp)->minor );
    struct Acq32Path* path = PD(filp);
    int max_samples = count/sizeof(short);
    unsigned long f_pos = (unsigned long)filp->f_pos;
    ssize_t tcount = 0;    

    // get the number of samples from cache or board is necessary 
    

    if ( !path->nsamples ){
        path->nsamples = acq32_GetNumSamples( path );
    }
    if ( f_pos >= path->nsamples ){
        PDEBUGL(3)( "assume EOF f_pos %ld num_samples %d\n",
                    f_pos,path->nsamples);
        return 0;
    }

    PDEBUGL(3)( "acq32_channel_read count: %d %ld\n", count, f_pos );

    max_samples = acq32_FetchData( path, f_pos, max_samples, channel );

    if ( max_samples <= 0 ){
        return max_samples;
    }

    if ( (ACFI_MODE&CM_EXCLUSIVE_OPTS) == CM_BINARY ||
         (ACFI_MODE&CM_EXCLUSIVE_OPTS) == CM_BIGEND    ){
        tcount = bin_buf_update( path, buf, max_samples, sizeof(short) );
        f_pos += max_samples;
        
        if ( (ACFI_MODE&CM_EXCLUSIVE_OPTS) == CM_BIGEND ){
            beFix( buf, tcount/sizeof(short) );
        }
    }else{

/*
 * formatting ... assume single ASCII format at present
 */
        int isample;
        int last_len;
        char* term = ACFI_MODE&CM_DOS_LINES? "\r\n": "\n";
        int termlen = strlen( term );

        for ( isample = 0; isample < max_samples; ++isample, ++f_pos ){

            int add_chars = 0;
            unsigned short data;

            char kdata[30];
  
            if ( (last_len = acq32_GetNextData( 
                path, &data, sizeof(data) ))  <= 0 ){
                break;
            }

            if ( ACFI_MODE&CM_LINENUMS ){
                long adj_pos = f_pos - path->device->m_dpd.nsamples_actual.pre;

                add_chars += sprintf( kdata+add_chars, "%8ld,", adj_pos  ); 
            }
 
            PDEBUGL(6)( "add_chars:%d kdata %s\n", add_chars, kdata );

            switch( ACFI_MODE&CM_EXCLUSIVE_OPTS ){
            case CM_VOLTS:
            {
                char buf[10];
                acq32_raw2mV10( data, buf );

                add_chars += sprintf( kdata+add_chars, ACFI_FORMAT, buf );
            }
            break;
            default:

                PDEBUGL(6) ( "f_pos %ld data %d", f_pos, data );

                add_chars += sprintf( kdata+add_chars, ACFI_FORMAT, data );
            }
            strcat( kdata+add_chars, term );
            add_chars += termlen;

            if ( tcount+add_chars < count ){
                copy_to_user( buf+tcount, kdata, add_chars );
                tcount += add_chars;
               
                PDEBUGL(6)( " copied, tcount now %d\n", tcount );

            }else{
                acq32_discardLastData( path, last_len );
                PDEBUGL(6) ( " space-chucking...\n" );
                break;
            }
        }
    }

    filp->f_pos = f_pos;

    PDEBUGL(3)( "acq32_channel_read returning %d f_pos %ld\n", 
                tcount, f_pos );

    return tcount;
}


#define MAXTIME_WITH_DATA 250000
#define MAXTIME_NO_DATA   1000000

ssize_t acq32_streaming_rowdev_read_workfunc( struct Acq32Device* device )
/* handle consumer end of streambuf.
 * turns out this doesn't work as a back end as copy_to_user_fails,
 * so it has to be just a regular fxn call from app side code
 */
{
    int ibuf = 0;
    u32 mfa;
    struct timeval tv_prev;
        
    PDEBUGL(3)(  " STARTS\n" );

    if ( device->appbuf.count_max <= 0 || device->appbuf.count_actual != 0 ){
        // bale out if no appbuf
        return 0;  
    }
    acq32_getDeltaTime( &tv_prev, &tv_prev );

    TIMESTAMP( device, 'b', "bh1" );

    // HACK begins here - try to force a min buffer size!
    while ( ibuf < 4096 ){

        while ( (mfa = streamGet( &device->downstreambuf )) == 
		                                     STREAM_MFA_INVALID ){

            if ( ibuf && 
		 device->m_dpd.streaming_message_delta_usecs > SLOW_ACQ ){
                goto copy_done;
            }else{
                unsigned delta_time =  acq32_getDeltaTime( &tv_prev, NULL );

                if ( ibuf && delta_time > MAXTIME_WITH_DATA ){
                    goto copy_done;
                }else{
                    if ( !ibuf && delta_time > MAXTIME_NO_DATA ){
                        return -ETIMEDOUT;
                    }else{
                        acq32_waitInt( device, 100 );
                    }
                }
            }
        }  //    while ( (mfa = streamGet( &device->streambuf )) ...
        
#ifdef PGMCOMOUT
        if ( duff_mfa( device, mfa, "bh" ) ) continue;
#endif
        { //XXXX (told you it was a hack!
            Message* message = (Message*)phys_to_virt( mfa );

            u16 id    = message->header.id;
            u16 mylen = message->header.length;
            u32 mtype = message->header.type;

            if ( (id&MID_NOT_VALID) != 0 ){

                PDEBUGL(0)( "discard garbled message id:0x%04x %d 0x%08x\n",
                            id, mylen, mtype );
                i2o_return_message( device, mfa );
                continue;
            }else if ( MT_ID(mtype)  != MT_STREAM ){
                i2o_return_message( device, mfa );
                PDEBUGL(0)( "ERROR!! MT_ID(0x%08x) not supported\n", 
			    MT_ID(mtype) );
                continue;
            }else if ( ibuf+mylen > device->appbuf.count_max ){

                /* here with streaming data ... copy to user buffer
                 * NB user buffer HAS to be big enough for entire message or we stall
                 */
                TIMESTAMP( device, 'b', "putback" );

                streamPutBack( &device->downstreambuf );
                PDEBUGL(3)( "end of user buffer PutBack for next time\n" );
                break;
            }else{
                // now we are ready to copy .... pass tag+samples, skip filla

                int isample  = 0;
                int nsamples = MTP_STREAM_NSAM(mtype);
                int slenw    = MTP_STREAM_SLEN(mtype)/sizeof(u16);
                int clenb    = MTP_STREAM_SLEN(mtype)-2;        // skip filla
                u16* psrc    = message->payload.sdata;

                PDEBUGL(2)( 
                    " id:0x%04x len:0x%04x mtype 0x%08x\n",
                    id, mylen, mtype 
                    );
                PDEBUGL(2)(
                    " isample:%d nsamples:%d slenw:%d clenb:%d\n", 
                    isample, nsamples, slenw, clenb 
                    );
                TIMESTAMP( device,'b',"cp n:%d",nsamples);

                for (; isample != nsamples; 
		         ++isample, psrc+=slenw, ibuf+=clenb ){
                    PDEBUGL(2)( " ib:%d MFA 0x%08x tag 0x%04x "
                                "copy_to_user( %p, %p, %d\n",
                                ibuf, mfa, psrc[1], 
                                device->appbuf.buf+ibuf, psrc+1, clenb  );

                    copy_to_user( device->appbuf.buf+ibuf, psrc+1, clenb );
                }       

                acq32_timestamp( device, 'b', "cp ends" );
                i2o_return_message( device, mfa );

                TIMESTAMP( device, 'b', "i2o" );
            }
        } //XXXX
    } // while()

 copy_done:
    if ( ibuf ){
        device->appbuf.count_actual = ibuf;
    }
    PDEBUGL(3)( "acq32_streaming_rowdev_read_workfunc() DONE\n" );

    acq32_timestamp( device, 'b', "bh99" );

    return ibuf;
}

static ssize_t 
streaming_rowdev_read( 
    struct file* filp,
    char* buf,
    size_t count,
    loff_t* posp
    )
{
    struct Acq32Device *device = PDEV( filp );
    size_t return_count;


    // do some stats on streambuf 

    {
        int nq = streamNumEntries( &device->streambuf );

        if ( nq > device->streambuf.def.hitide ){
            device->streambuf.def.hitide = nq;
        }
    }

    PDEBUGL(2)(  " %p buf count:%d\n", buf, count );

    /*
     * setup up application buffer to be used by "Bottom Half"
     */

    // GUARD
    //    cli();  
    device->appbuf.buf = buf;
    device->appbuf.count_max = count;
    device->appbuf.count_actual = 0;
    //    sti();

    return_count = acq32_streaming_rowdev_read_workfunc( device );

    if ( return_count < 0 ) return return_count;

    /*
     * now retrieve the filled application buffer
     */

    return_count = device->appbuf.count_actual;
    device->appbuf.buf = (char*)0;
    device->appbuf.count_max = 
        device->appbuf.count_actual = 0;

    PDEBUGL(2)(  " returning %d\n", return_count );

    filp->f_pos += return_count;         /* fpos in bytes not samples ??? */

    if ( return_count < 450 ) {
        PDEBUGL(0)(  " rtn %d buf %d available %d\n",
                    return_count, count, streamNumEntries( &device->streambuf ) );
    }
    return return_count;
}
    

ssize_t acq32_row_read ( 
    struct file* filp, char* buf, size_t count, loff_t* posp 
    )
{
    int channel = 0xff;          // magic # PGMWORKTODO
    struct Acq32Path* path = PD(filp);
    int max_samples = count/sizeof(short)/ACQ32_MAX_CHANNELS;
    unsigned long f_pos = (unsigned long)filp->f_pos;
    int nchannels_in_mask = acq32_devGetNumChannelsFromMask( PDEV(filp) );
    ssize_t tcount = 0;    

    if ( PDEV(filp)->m_dpd.streaming != 0 ){
        return streaming_rowdev_read( filp, buf, count, posp );
    }    
    // get the number of samples from cache or board is necessary 
    
    if ( !path->nsamples ){
        path->nsamples = acq32_GetNumSamples( path );
    }
    if ( f_pos >= path->nsamples ){
        PDEBUGL(3)( "assume EOF f_pos %ld num_samples %d\n",
                    f_pos,path->nsamples);
        return 0;
    }

    PDEBUGL(2)( "acq32_row_read pos:%ld count: %d\n", f_pos, count );

    max_samples = acq32_FetchData( path, f_pos, max_samples, channel );

    if ( max_samples <= 0 ){
        return max_samples;
    }

    if ( (ACFI_MODE&CM_EXCLUSIVE_OPTS) == CM_BINARY ){
        tcount = bin_buf_update( 
            path, buf, max_samples, 
            sizeof(short)*nchannels_in_mask );
        f_pos += max_samples;
    }else{

/*
 * formatting ... assume single ASCII format at present
 */
        int isample;
        int last_len = 0;
        int data_len = nchannels_in_mask*sizeof(unsigned short);

        for ( isample = 0; isample < max_samples; ++isample, ++f_pos ){

            unsigned short data[ACQ32_MAX_CHANNELS];
            char kstring[ACQ32_MAX_CHANNELS*32];
            int add_chars = 0;
            int channel;
            int this_len;
  
            if ( (this_len = acq32_GetNextData(path, &data, data_len)) <= 0 ){
                PDEBUGL(4)( "acq32_row_read - data's run out %d\n", isample );
                break;
            }else{
                last_len += this_len; 
            }

            if ( ACFI_MODE&CM_LINENUMS ){
                long adj_pos = f_pos - path->device->m_dpd.nsamples_actual.pre;

                add_chars += sprintf( kstring+add_chars, "%8ld,", adj_pos ); 
            }

            for ( channel = 0; channel != nchannels_in_mask; ++channel ){
                switch( ACFI_MODE&CM_EXCLUSIVE_OPTS ){
                case CM_VOLTS:
                {
                    char buf[10];
                    acq32_raw2mV10( data[channel], buf );

                    add_chars += sprintf(kstring+add_chars,ACFI_FORMAT,buf);
                }
                break;
                default:
                    add_chars += sprintf( 
                        kstring+add_chars, 
                        ACFI_FORMAT, 
                        data[channel]
                        );
                }
                strcat( kstring+add_chars, "," );
                add_chars += strlen( "," );
            }
            
            if ( ACFI_MODE&CM_DOS_LINES ){
                kstring[add_chars-1] = '\r';
                ++add_chars;
            }
            kstring[add_chars-1] = '\n';             // overwrite last comma

            ASSERT( add_chars < sizeof(kstring) );

            if ( tcount+add_chars <= count ){
                PDEBUGL(4)( "acq32_row_read copy %d\n", add_chars );
                copy_to_user( buf+tcount, kstring, add_chars );
                tcount += add_chars;
            }else{
                acq32_discardLastData( path, last_len );
                PDEBUGL(4)( "acq32_row_read full tcount:%d addc:%d\n",
                            tcount, add_chars );                
                break;
            }
        }
    }

    filp->f_pos = f_pos;

    PDEBUGL(3)( "acq32_row_read pos %ld returning %d\n", f_pos, tcount );

    return tcount;
}

int acq32_getCycle( struct Acq32Device* device )
{
    unsigned cycle;

    device->get_mailbox( device, BP_MB_STATUS, &cycle );
    return BP_GET_CYCLE( cycle );
}
static unsigned _getHeartbeat( struct Acq32Device* device )
{
    unsigned v;

    device->get_mailbox( device, BP_MB_STATUS,  &v );
    return BP_GET_HEARTBEAT( v );
}

int acq32_getHeartbeat( struct Acq32Device* device )
// return heartbeat
{
    WAIT_QUEUE waitq;
    unsigned v1, v2;
    
    v1 = _getHeartbeat( device );

    init_waitqueue_head( &waitq );    
    PDEBUGL(2)(  " 1 waitq %p, timeout %d\n", &waitq, HZ/10  );
    interruptible_sleep_on_timeout( &waitq, HZ/10 );
    PDEBUGL(2)(  " 2\n" );
    v2 = _getHeartbeat( device );
 
    if ( v2 != v1 ){
        if ( v2 > v1 ){
            PDEBUGL(2)( "acq32_heartbeat %d ticks/sec\n", 
                        (v2-v1)*10 );
            return device->heartbeat_detected = v2 - v1;
        }else{
	    PDEBUGL(2)(  " 3\n" );
	    init_waitqueue_head( &waitq );    
	    interruptible_sleep_on_timeout( &waitq, HZ/10 );
	    PDEBUGL(2)(  " 4\n" );
            v1 = _getHeartbeat( device );
	    
	    PDEBUGL(2)( " second try %d ticks/sec\n",
			(v1-v2)*10 );

            return device->heartbeat_detected = v1 - v2;
        }
    }else{
        unsigned m1, m2, m3, m4;
            
        device->get_mailbox( device, 0, &m1 );
        device->get_mailbox( device, 1, &m2 );
        device->get_mailbox( device, 2, &m3 );
        device->get_mailbox( device, 3, &m4 );
        PDEBUGL(1)( "acq32_heartbeat ERROR:mailboxes are\n"
		    "0x%08x\n0x%08x\n0x%08x\n0x%08x\n",
                    m1, m2, m3, m4 );
        return device->heartbeat_detected = 0;
    }
}


int acq32_genericCoreDevInit( struct Acq32Device* device )
/* returns 0 on success */
{
	int rc = 0;               

	PDEBUGL(2)(  "1 \n" );

	rchInit( device );

        /* set up control struct for streaming isr bh */

        INIT_TQUEUE( &device->m_dpd.isr_bh, acq32_streaming_isr_bh, device ); 
        
        PDEBUGL(3)( " queue_task %s\n",
                    device->m_dpd.isr_bh.routine==acq32_streaming_isr_bh?
                    "acq32_streaming_isr_bh": "dunno guv" 
		);

	PDEBUGL(2)(  "99\n" );
        
	return rc;
}


/*
 * here is the return command handler. WARNING: runs at BH level
 */
 
void fetchDataReturnHandler(
    struct ReturnCommandHandler* self,
    struct MESSAGE* response
    )
{
    struct Acq32Path* path   = (struct Acq32Path*)self->path;
    Message* command  = getMessageBuffer( path );
    int total_records = command->header.length/sizeof(HostRequestDataRecord);
    HostRequestDataRecord* this_response = response->payload.hrdr;
    HostRequestDataRecord* requests = command->payload.hrdr;
    int ireq;
    int zeros = 0;
    int not_accepted = 1;
    
    
    for ( ireq = 0; ireq != total_records; ++ireq ){
        if ( requests[ireq].pci ){
            if ( this_response->start == requests[ireq].start ){
            
                PDEBUGL(2)(  " clearing response %d start %x\n", 
                            ireq, this_response->start );
                    
                path->return_samples += this_response->nsamples;
                         
                memset( &requests[ireq], 0, sizeof(HostRequestDataRecord) );
                ++zeros;
                not_accepted = 0;
            }
        }else{
            ++zeros;
        }
    }
    if ( not_accepted ){
        PDEBUG(  " ERROR: did not accept this_reponse start %x\n", 
                this_response->start );
                                            
        for ( ireq = 0; ireq != total_records; ++ireq ){
            PDEBUG( "%d : %x %s\n", 
                    ireq, requests[ireq].start, 
                    requests[ireq].pci? "":"CLEAR" );
        }
    }
    if ( zeros == total_records ){
        // we're done, wake up time
            
        rchRemove( self );
        memset( self, 0, sizeof(struct ReturnCommandHandler) );
        wake_up( &path->return_waitq );
            
        PDEBUGL(2) (  " wakey wakey!\n" );
    }
}

static void dumpRH( struct ReturnCommandHandler* self )
{
    struct Acq32Path* path   = (struct Acq32Path*)self->path;
    Message* command  = getMessageBuffer( path );
    int total_records = command->header.length/sizeof(HostRequestDataRecord);
    HostRequestDataRecord* requests = command->payload.hrdr;
    int ireq;

    for ( ireq = 0; ireq != total_records; ++ireq ){
        PDEBUG( "dumPRH %2d dst 0x%08x start 0x%08x\n", 
                ireq, requests[ireq].pci, requests[ireq].start );
    }
}
static void sendCommandMessage( void* clidata )
// this func sends the message, deferred until main task is already
// waiting for int 
// so NO CHANCE of a RACE. SO THERE
{
    struct Acq32Path* path = (struct Acq32Path*)clidata;
    acq32_sendI2O_kbuf( path, getMessageBuffer( path ) );
}    


#define NUMCHANNELS( channel )     ((channel)==ALLCHANNELS? 32: 1)    // WORKTODO
#define SAMPLES_PER_BUFFER(channel) \
        (DMABUF_SIZE/(sizeof(short)*NUMCHANNELS(channel)))
        
#define TIMEOUT MAX(((2+2*acq32_debug*acq32_debug)*100),acq32_dumdma_to)

static int _acq32_fetchDataToLocalBuffer(         
    struct Acq32Path* path,
    int channel, 
    short* buffer,
    int nsamples,
    int start,
    int stride
    )
/*
 * new style streamed fetch
 * buffer MUST be pre mapped using mmap
 * returns actual num samples fetched
 * MESSAGE_HRDR_LEN restricts the number of HRDR records possible
 */
{
    Message* message = getMessageBuffer( path );
    ChannelMapping* mapping = path->channel_maps[0];
    HostRequestDataRecord* requests = message->payload.hrdr;
    struct ReturnCommandHandler* rch = &path->returnCommandHandler;

    int ireq = 0;    // Buffer request index in packet
    int ipbuf = 0;   // Corresponding buffer index in VMA (!= ireq)
    int ntotal;
    int rv;
    unsigned long vm_ubuf = (unsigned long)buffer;

    /* some booleans to diagnose endpoint */
    int max_mapping = 0;
    int mapping_too_small = 0;
    int end_of_message = 0;
    int complete = 0;
    int rv_numsamples = -1;

    // find the right map - WORKTODO - ONE only assumed   


    PDEBUGL(1)(  "ch %d buf %p ns %d start %d stride %d\n",
		channel, buffer, nsamples, start, stride );

    if ( mapping == NULL ){
        return -EACQ32_NO_MAPPING;
    }else if ( !IN_RANGE( vm_ubuf, 
			  mapping->vma->vm_start, 
			  mapping->vma->vm_end ) ){
	return -EACQ32_OUT_OF_MAPPING_RANGE;
    }else if ( vm_ubuf+nsamples > mapping->vma->vm_end ){
	return -EACQ32_OUT_OF_MAPPING_RANGE;
    }else{
	int sample_offset_in_vma = (vm_ubuf - mapping->vma->vm_start)/2;
	int buf_start_sample;

	ipbuf = sample_offset_in_vma/SAMPLES_PER_BUFFER(channel);
	buf_start_sample = ipbuf*SAMPLES_PER_BUFFER(channel);

	if (  buf_start_sample != sample_offset_in_vma ){
	    int buf_delta = sample_offset_in_vma - buf_start_sample;

	    PDEBUGL(1)(  "buf_start_sample 0x%06x buf_delta 0x%06x\n",
			buf_start_sample, buf_delta );

	    nsamples += buf_delta;
	    path->return_samples = -buf_delta;
	}else{
	    path->return_samples = 0;
	}
    }

    PDEBUGL(1)( "vm_start 0x%08lx vm_ubuf 0x%08lx vm_end 0x%08lx ipbuf %d\n",
		mapping->vma->vm_start, vm_ubuf, mapping->vma->vm_end, ipbuf );
    /*
     * here with valid buffer request, ireq set to offset in phys buf
     * now assemble buffer requests for transmission to acq32
     */
    for ( ntotal = 0, ireq = 0; 
          !((complete = ntotal >= nsamples                   ) ||
	    (max_mapping = ireq >= MAX_BUFFERS_IN_CHANNEL    ) ||
            (mapping_too_small = mapping->buffers[ipbuf]==NULL)||
            (end_of_message = ireq >= MESSAGE_HRDR_LEN       )     ); 
	  ++ireq, ++ipbuf ){
              
        requests[ireq].pci         = virt_to_bus( mapping->buffers[ipbuf] );
        requests[ireq].start       = start;
        
        requests[ireq].nsamples    = MIN( nsamples-ntotal, 
					  SAMPLES_PER_BUFFER(channel)  );

        requests[ireq].chan        = channel;
        requests[ireq].stride      = stride;
            
        ntotal += requests[ireq].nsamples;
        start  += requests[ireq].nsamples * stride;
            
        if ( channel == ALLCHANNELS ){
            REQUESTMAXBYTES( requests[ireq] ) = DMABUF_SIZE; // WORKTODO!!
        }
            
            
        PDEBUGL(3)(  "ch %d build me a buffer, make her %d samples long\n",
                    requests[ireq].chan, requests[ireq].nsamples );
        PDEBUGL(4)(  "pci:0x%08x istart:%d stride:%d\n",
                    requests[ireq].pci, 
		    requests[ireq].start, 
		    requests[ireq].stride );
    }

    if ( max_mapping ){
	PDEBUG("ERROR - request greater than max mapping\n" );
    }
    if ( mapping_too_small ){
	PDEBUG("ERROR - request too big for mapping\n" );
    }
    if ( end_of_message ){
	PDEBUG("WARNING - end of message [%d], app should retry at offset\n",
	       MESSAGE_HRDR_LEN );
    }

    message->header.type = MAKE_MT_COMMAND( channel==ALLCHANNELS?
                                            MTC_HOST_REQUEST_X_DATA:
                                            MTC_HOST_REQUEST_DATA    );
    message->header.length = ireq*sizeof(HostRequestDataRecord);
    
    // register message with device for future wakeup

    if ( rch->next != NULL ){
        PDEBUG( " ERROR: there's a command already queued!!!\n" );
        return -1;          
    }


    rch->handler = fetchDataReturnHandler;
    rch->path = (struct Acq32Path*)path;
    rch->command_id = MAKE_ID_DTACQ(path->device->m_dpd.i2o_last_id_out + 1);

/*
 * enter the gate - only one DUMDMA fetch at a time.
 * WORKTODO - when we split MARSHALL/fetch, this can move
 * first gate is per device
 * second gate is global
 */
    MUTEX_DOWN( &path->device->m_dpd.ioread_fetch_mutex );
    if ( acq32_globalIoreadFetchMutexDown() ){
	MUTEX_UP( &path->device->m_dpd.ioread_fetch_mutex );
	return -ERESTARTSYS;
    }

    PDEBUGL(1)(  " q send command task and goto sleep %d\n", path->minor );
    rchInsert( path->device->m_dpd.first_rch, rch );
 
    INIT_TQUEUE( &path->defer, sendCommandMessage, path );   
    schedule_task( &path->defer );

    rv = interruptible_sleep_on_timeout( &path->return_waitq, TIMEOUT );

    if ( acq32_debug ){
	    char buf[20];
	    copy_from_user( buf, buffer, 20 );
	    buf[19] = '\0';

	    PDEBUGL(3)( "Contents of buffer now %s\n", buf );
    }

    if ( rv != 0 ){
        PDEBUGL(1)(  " returns SUCCESS\n" );
        rv_numsamples = path->return_samples;
    }else{
	PDEBUGL(1)(  "WARNING: timeout, resetting driver (could be dodgy)\n" );
        dumpRH( rch );
        rchRemove( rch );
        memset( rch, 0, sizeof(struct ReturnCommandHandler) );
	path->device->coreDevInit(path->device);
        rv_numsamples = -ETIMEDOUT;
    }

    PDEBUGL(1)(  " leaving mutex %d\n", path->minor );

    acq32_globalIoreadFetchMutexUp();
    MUTEX_UP( &path->device->m_dpd.ioread_fetch_mutex );

    return rv_numsamples;
}


int acq32_fetchDataToLocalBuffer(         
    struct file* file,
    int channel, 
    short* buffer,
    int nsamples,
    int start,
    int stride
    )
{
	return _acq32_fetchDataToLocalBuffer(
		PD(file), channel, buffer, nsamples, start, stride );
}




extern int acq32_getBufferPhysicalAddr(
    struct Acq32Path* path,
    int channel,
    void* buffer
    )
// outputs array u32[nsegs*2] in buffer
// buffer[0] = paddr, buffer[1] = seg len (DMABUF_SIZE)
{
    u32* paddrs = (u32*)getMessageBuffer( path );
    ChannelMapping* mapping = path->channel_maps[0];
    int ireq;
    
    PDEBUGL(2)(  "Start\n" );
    // find the right map - WORKTODO - ONE only assumed   
    
    if ( mapping == NULL ){
        return -EACQ32_NO_MAPPING;
    }
    
    
    for ( ireq = 0; 
          ireq != MAX_BUFFERS_IN_CHANNEL && mapping->buffers[ireq]; ++ireq ){
              
        paddrs[ireq*2] = virt_to_bus( mapping->buffers[ireq] );
        paddrs[ireq*2+1] = DMABUF_SIZE;
            
        PDEBUGL(3)(  "[%d] bus:0x%08x len:0x%08x\n",
                    ireq, paddrs[ireq*2], paddrs[ireq*2+1] ); 
    }
    
    if ( ireq == 0 ){
        return -ENOMEM;
    }
        
    paddrs[ireq*2] = 0;
    paddrs[ireq*2+1] = 0;
   
/*
 * copy_to_user() doesn't work !!???
 * never mind, it's all kernel mapped memory anyway,
 * so just copy to first base
 */
//  copy_to_user( buffer, paddrs, (ireq*2+2)*sizeof(u32) );
    memcpy( mapping->buffers[0], paddrs, (ireq*2+2)*sizeof(u32) );
    
    PDEBUGL(2)(  "Done - %d areas user %p len:%d\n", 
                ireq, buffer, (ireq*2+2)*sizeof(u32) );
    
    return 0;
}


    
