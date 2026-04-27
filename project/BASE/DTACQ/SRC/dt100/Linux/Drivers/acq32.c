/*****************************************************************************
 *
 * File: acq32.c - Linux device driver for acq32
 *
 * $RCSfile: acq32.c,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: acq32.c,v 1.128.4.16 2006/04/22 16:11:27 pgm Exp $
 * $Log: acq32.c,v $
 * Revision 1.128.4.16  2006/04/22 16:11:27  pgm
 * acq32_getDriver allocates fresh vtable per call so that clients can override methods with interference.
 * Fixes acq32 setChannelMask bug in mixed acq196/acq216 system
 *
 * Revision 1.128.4.15  2004/09/26 15:27:18  pgm
 * correct host buf map, read ident behaviour
 *
 * Revision 1.128.4.14  2004/08/10 07:37:05  pgm
 * *** empty log message ***
 *
 * Revision 1.128.4.13  2003/12/14 22:01:04  pgm
 * split acq32, acq200 drivers
 *
 * Revision 1.128.4.12  2003/12/11 19:28:22  pgm
 * no bb hog on 1G
 *
 * Revision 1.128.4.11  2003/10/25 19:02:00  pgm
 * acq200_fetchData (NFG)
 *
 * Revision 1.128.4.10  2003/10/04 17:32:25  pgm
 * compiles RH73
 *
 * Revision 1.128.4.9  2003/09/16 11:03:20  pgm
 * compiles clean on 2.4.20 (RH9x)
 *
 * Revision 1.128.4.8  2003/09/09 20:50:18  pgm
 * *** empty log message ***
 *
 * Revision 1.128.4.7  2003/09/08 20:48:35  pgm
 * file upload almost works (but not quite)
 *
 * Revision 1.128.4.6  2003/09/08 12:51:30  pgm
 * i2o file download works
 *
 * Revision 1.128.4.5  2003/09/07 14:38:23  pgm
 * stub wavegen intro
 *
 * Revision 1.128.4.4  2003/09/06 07:28:03  pgm
 * acq32 now works again after acq200 split
 *
 * Revision 1.128.4.3  2003/09/04 21:11:12  pgm
 * acq200 getFwrev works (sorta) x156
 *
 * Revision 1.128.4.2  2003/08/31 21:23:42  pgm
 * *** empty log message ***
 *
 * Revision 1.128.4.1  2003/08/31 16:53:52  pgm
 * first acq200 testing
 *
 * Revision 1.128.2.1  2003/08/29 17:17:03  pgm
 * first stage acq200 support
 *
 * Revision 1.128  2002/11/03 21:12:02  pgm
 * merge from hols simul
 *
 * Revision 1.126  2002/08/29 18:36:25  pgm
 * COS device tools OK
 *
 * Revision 1.125  2002/08/26 13:09:59  pgm
 * impl status stream - compiles 2.4, needs tesst
 *
 * Revision 1.124  2002/08/20 10:07:42  pgm
 * clean globs, use mutex for IOREAD
 *
 * Revision 1.123  2002/08/10 16:10:11  pgm
 * gated fetch part success
 *
 * Revision 1.122  2002/08/09 13:52:52  pgm
 * ioread_fetch_mutex
 *
 * Revision 1.121  2002/08/03 18:56:14  pgm
 * trace fault in bucket
 *
 * Revision 1.120  2002/07/30 12:06:58  pgm
 * avoid problems with partially blank flash modules
 *
 * Revision 1.119  2002/07/30 10:16:13  pgm
 * attempt to handle BLANK boards correctly
 *
 * Revision 1.118  2002/07/29 18:02:53  pgm
 * try to make /proc/acq32 resistant to blank device
 *
 * Revision 1.117  2002/07/28 17:42:36  pgm
 * all globs in .globs
 *
 * Revision 1.116  2002/07/19 18:39:37  pgm
 * mod DUMDMA to avoid message overrun
 *
 * Revision 1.115  2002/07/02 13:31:58  pgm
 * bodge 2.4.7
 *
 * Revision 1.114  2002/07/02 10:59:49  pgm
 * add mmap testing
 *
 * Revision 1.113  2002/06/12 20:20:18  pgm
 * fix master device. gotta work now
 *
 * Revision 1.112  2002/05/15 09:10:32  pgm
 * downstreambuf
 *
 * Revision 1.111  2002/05/15 08:05:57  pgm
 * busprot debug
 *
 * Revision 1.110  2002/04/20 13:07:51  pgm
 * makes multi master work again
 *
 * Revision 1.109  2002/03/28 18:57:28  pgm
 * status device works
 *
 * Revision 1.108  2002/03/28 09:26:38  pgm
 * remove redundant init waitq calls
 *
 * Revision 1.107  2002/03/28 08:42:42  pgm
 * fixes isr to sep ack, cos, add waitevent, doubletap good
 *
 * Revision 1.106  2002/03/27 15:33:31  pgm
 * revert to mode, num proc
 *
 * Revision 1.105  2002/03/25 02:10:14  pgm
 * new style proc/acq32
 *
 * Revision 1.104  2002/03/24 23:46:19  pgm
 * new proc/acq32
 *
 * Revision 1.103  2002/03/25 09:28:49  pgm
 * checks cycle
 *
 * Revision 1.102  2002/03/12 12:12:40  pgm
 * clean format rools OK
 *
 * Revision 1.101  2002/03/08 19:21:59  pgm
 * bigger readbuffer
 *
 * Revision 1.100  2002/03/08 16:17:55  pgm
 * *** empty log message ***
 *
 * Revision 1.99  2002/03/07 13:32:26  pgm
 * double tap rools OK
 *
 * Revision 1.98  2002/03/07 12:25:19  pgm
 * acq32_double_tap
 *
 * Revision 1.97  2002/02/26 16:43:24  pgm
 * command debug stream
 *
 * Revision 1.96  2002/02/02 14:44:42  pgm
 * waitEvent (not complete), multimaster updates
 *
 * Revision 1.95  2002/01/02 12:14:15  pgm
 * back prot to 2.2.x works
 *
 * Revision 1.94  2001/12/26 21:15:02  pgm
 * ROM too
 *
 * Revision 1.93  2001/12/26 21:09:38  pgm
 * fix offset in acq32_mmap()
 *
 * Revision 1.92  2001/12/21 21:20:09  pgm
 * fix acq32_GetHeartbeat() logic error - broke startup
 *
 * Revision 1.91  2001/12/21 16:08:59  pgm
 * *** empty log message ***
 *
 * Revision 1.90  2001/12/19 21:40:07  pgm
 * remove KMALLOC DEBUG
 *
 * Revision 1.89  2001/12/19 21:32:49  pgm
 * llcontrol works on Linux 2.4 - SHIP IT QUICK
 *
 * Revision 1.88  2001/12/19 10:09:40  pgm
 * Builds on RH6.2 again
 *
 * Revision 1.87  2001/12/19 09:56:06  pgm
 * Linux 2.4 first cut runs, mapping dodgy
 *
 * Revision 1.86  2001/12/14 12:22:52  pgm
 * debug channel num
 *
 * Revision 1.85  2001/11/28 21:03:25  pgm
 * minor encode supports more than 4 boards
 *
 * Revision 1.84  2001/11/23 21:54:57  pgm
 * continuous stream speed demo done
 *
 * Revision 1.83  2001/11/05 21:37:50  pgm
 * stubs for test write interface
 *
 * Revision 1.82  2001/10/21 09:08:40  pgm
 * selectThresholdDetector etc
 *
 * Revision 1.81  2001/10/20 15:52:11  pgm
 * clock redistribution opts included
 *
 * Revision 1.80  2001/10/13 19:56:47  pgm
 * IOCTL write finds flash, but prog bombs
 *
 * Revision 1.78  2001/10/06 22:21:53  pgm
 * raw, flash devs no longer use ints
 *
 * Revision 1.77  2001/10/01 07:46:09  pgm
 * change csr_ba mapping to work with big CSR range
 *
 * Revision 1.76  2001/09/08 17:52:38  pgm
 * debug [sg]et_mailbox()
 *
 * Revision 1.75  2001/08/23 18:53:49  pgm
 * alpha warning modifed
 *
 * Revision 1.74  2001/08/23 18:44:43  pgm
 * and compiles on alpha too
 *
 * Revision 1.73  2001/08/23 18:39:22  pgm
 * alpha hack comiles on x86
 *
 * Revision 1.72  2001/08/23 18:37:31  pgm
 * try dense_mem() hack for ALPHA
 *
 * Revision 1.71  2001/08/21 18:09:58  pgm
 * ALPHA: remove all derefs of p_rom, p_sram
 *
 * Revision 1.70  2001/08/20 19:56:22  pgm
 * ALPHA:no direct deref io space
 *
 * Revision 1.69  2001/05/27 18:56:49  pgm
 * *** empty log message ***
 *
 * Revision 1.68  2001/05/26 07:20:22  pgm
 * remove CPCI test hack, this was probably causing faulty op
 *
 * Revision 1.67  2001/05/19 19:43:51  pgm
 * kludge for CSR mem mapping, getPhysicalAddress
 *
 * Revision 1.66  2001/05/17 18:51:40  pgm
 * ACQ32_IOREAD_GETPHYSICAL implemented
 *
 * Revision 1.65  2001/05/15 17:56:09  pgm
 * AO/DO fungen implemented
 *
 * Revision 1.64  2001/05/05 21:04:40  pgm
 * try fix DUMDMA fault with tq_scheduler, not tq_immediate
 *
 * Revision 1.63  2001/05/05 19:44:45  pgm
 * stub write(), fix bigdump dep, fault tolerant DUMDMA
 *
 * Revision 1.62  2001/04/29 09:25:50  pgm
 * now supports >4 devices. whew
 *
 * Revision 1.61  2001/04/29 08:23:18  pgm
 * use one chrdev per board, prep for >4 boards
 *
 * Revision 1.60  2001/04/28 08:01:43  pgm
 * add irq_req_mutex - avoid over registering isr
 *
 * Revision 1.59  2001/04/27 19:32:43  pgm
 * use mutex to guard mbox access - ship it quick
 *
 * Revision 1.58  2001/04/27 17:19:50  pgm
 * fix race in getFwrev
 *
 * Revision 1.57  2001/04/27 13:45:44  pgm
 * maps ROM in SRAM as required as per strict PCI BIOS reqs
 *
 * Revision 1.56  2001/04/27 09:20:24  pgm
 * handles no rom mapping
 *
 * Revision 1.55  2001/04/01 12:17:57  pgm
 * channel number _is_ frm 0
 *
 * Revision 1.54  2001/04/01 08:26:32  pgm
 * DUMDMA works - fix with post sync on acq32
 *
 * Revision 1.53  2001/03/31 20:58:23  pgm
 * DUMDMA works in slow mo
 *
 * Revision 1.52  2001/03/30 20:17:00  pgm
 * UDMA first cut working
 *
 * Revision 1.51  2001/03/28 21:18:29  pgm
 * i2o command handling for FETCH cmpiles - willit work??
 *
 * Revision 1.50  2001/03/25 17:28:40  pgm
 * i2o commands work
 *
 * Revision 1.49  2001/03/23 19:44:00  pgm
 * mmap() as kernel malloc to user mapping work
 *
 * Revision 1.48  2001/03/11 21:30:11  pgm
 * fixed indentation with snavigator
 *
 * Revision 1.47  2001/02/24 18:18:36  pgm
 * test device for lltest, also acqcmd
 *
 * Revision 1.46  2001/01/14 17:54:11  pgm
 * multi thread safemake
 *
 * Revision 1.45  2001/01/02 12:10:28  pgm
 * add trace to kmalloc/kfree, fix host mem leak
 *
 * Revision 1.44  2000/12/09 10:59:43  pgm
 * fixes pfmu hang in blnak prog mode
 *
 * Revision 1.43  2000/11/12 15:13:43  pgm
 * read() from host memory buffer is perfect
 *
 * Revision 1.42  2000/10/28 21:18:59  pgm
 * getSetNumSamples
 *
 * Revision 1.41  2000/10/06 21:06:36  pgm
 * bigdump mmaps to .host device
 *
 * Revision 1.40  2000/10/01 08:27:49  pgm
 * fix rowdev read bug - uses channel mask, ArmSpin flash prog feature
 *
 * Revision 1.39  2000/09/26 19:46:52  pgm
 * GATED_CONTINUOUS pre/post, lseek, spin
 *
 * Revision 1.38  2000/09/22 13:59:45  pgm
 * min reads per sec timing implemented
 *
 * Revision 1.37  2000/09/18 13:09:11  pgm
 * big buf prototype code in place, defualt:OFF
 *
 * Revision 1.36  2000/08/22 06:32:45  pgm
 * better instrumentation, diags, Q's work
 *
 * Revision 1.35  2000/08/04 06:53:44  pgm
 * i2o diags added
 *
 * Revision 1.34  2000/07/01 08:56:03  pgm
 * defaults to using interrupts
 *
 * Revision 1.33  2000/06/25 05:53:28  pgm
 * acq32 push works
 *
 * Revision 1.32  2000/06/24 10:57:46  pgm
 * i2o round trip works
 *
 * Revision 1.31  2000/06/04 11:20:39  pgm
 * better version info
 *
 * Revision 1.30  2000/06/04 11:12:53  pgm
 * *** empty log message ***
 *
 * Revision 1.29  2000/05/28 10:31:12  pgm
 * streaming works passibly in simulator
 *
 * Revision 1.28  2000/05/20 11:03:47  pgm
 * Rev 6 commands work + sim sample tagging
 *
 * Revision 1.27  2000/05/08 19:13:09  pgm
 * simulate mode /proc/acq32 works properly
 *
 * Revision 1.26  2000/03/26 20:58:18  pgm
 * acq32_is_nonblocking is default
 *
 * Revision 1.25  2000/01/08 20:38:17  pgm
 * interrupt handling (not default)
 *
 * Revision 1.24  2000/01/04 10:53:49  pgm
 * eliminate slow poll bottleneck
 *
 * Revision 1.23  1999/11/20 21:20:30  pgm
 * 991120 time for bed
 *
 * Revision 1.22  1999/11/13 23:03:14  pgm
 * device info in /proc/acq32
 *
 * Revision 1.21  1999/11/11 22:11:30  pgm
 * plug mem leak, fix buffer discon prob in ascii read
 *
 * Revision 1.20  1999/11/09 21:00:13  pgm
 * buffering fixed, works with any size buffer
 *
 * Revision 1.19  1999/11/07 09:35:06  pgm
 * data handling is a runner
 *
 * Revision 1.18  1999/11/02 21:35:53  pgm
 * bocking Arm is reliable - ship it
 *
 * Revision 1.17  1999/11/02 10:43:35  pgm
 * channel read works
 *
 * Revision 1.16  1999/11/01 17:31:49  pgm
 * first stab at data fetch (not complete)
 *
 * Revision 1.15  1999/10/27 22:07:02  pgm
 * acq32-drv.o replaces ebsa-drv
 *
 * Revision 1.14  1999/10/26 15:18:14  pgm
 * revision added
 *
 * Revision 1.13  1999/10/26 15:16:54  pgm
 * struct debug, mbox commands
 *
 * Revision 1.12  1999/10/20 19:57:16  pgm
 * mixed in ebsa driver funcs + new rom offset hakker
 *
 * Revision 1.11  1999/10/15 18:47:00  pgm
 * extclock, gate_hi prams in, GATED_CONTINUOUS
 *
 * Revision 1.10  1999/10/15 15:10:11  pgm
 * GATE input to SM workscvs status | grep Status
 *
 * Revision 1.9  1999/10/14 22:51:50  pgm
 * simul engine runs - SHIP IT QUICKcvs status | grep Status
 *
 * Revision 1.8  1999/10/14 21:10:28  pgm
 * master command handling works
 *
 * Revision 1.7  1999/10/12 21:24:20  pgm
 * master command handling top down connection OK
 *
 * Revision 1.6  1999/10/12 10:23:47  pgm
 * basic master command/readback struct WORKS
 *
 * Revision 1.5  1999/10/11 20:21:02  pgm
 * basic command handling and data format OK
 *
 * Revision 1.4  1999/10/09 21:40:59  pgm
 * args handling in write taped
 *
 * Revision 1.3  1999/10/03 06:22:42  pgm
 * readproc now works
 *
 * Revision 1.2  1999/10/02 21:27:14  pgm
 * first pass compile OK. /proc/acq32 stuffed
 *
 * Revision 1.1  1999/10/02 19:33:46  pgm
 * lets go
 *
 *
\*****************************************************************************/



#include "local.h"

#ifndef __KERNEL__
#  define __KERNEL__
#endif
#ifndef MODULE
#  define MODULE
#endif

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


#ifndef PROCFILENAME
#define PROCFILENAME "acq32"
#endif

/* 2.4-only __global_cli/__global_sti stubs and tqueue_lock removed for 2.6 port */


//#define DEBUG_MALLOC
#ifdef DEBUG_MALLOC
#define KMALLOC( size, flags ) dbg_kmalloc( size, flags, __FILE__, __LINE__ )
#define KFREE( addr )          __dbg_kfree( addr, __FILE__, __LINE__ )
#else
#define KMALLOC( size,flags )  kmalloc( size, flags )
#define KFREE( addr )          kfree( addr )
#endif

//#define FN __FUNCTION__

    
#define VENDOR_ID       PCI_VENDOR_ID_DEC
#define DEVICE_ID       PCI_DEVICE_ID_DEC_21285



/*
 * MODULE PARMS
 */

#include "acq32_releasetag.c"

#define MPI( name, def, descr ) \
int name = def; \
module_param( name, int, 0644 ); \
MODULE_PARM_DESC( name, descr );

#define MPL( name, def, descr ) \
long name = def; \
module_param( name, long, 0644 ); \
MODULE_PARM_DESC( name, descr );

MODULE_AUTHOR("Peter.Milne@d-tacq.com");
MODULE_DESCRIPTION("D-TACQ Solutions Ltd acq32pci/acq16pci/acq32cpci driver");
MODULE_LICENSE("GPL");
            
MPI( acq32_use_interrupts,   1, "enable/disable interrupts      (1)" );
MPI( acq32_simulate,         0, "set to 1 to enable simulation  (0)" );
MPI( acq32_gate_hi,          0, "set 1 for gate active high     (0)" );
MPI( acq32_extclock,         1, "set to 0 for internal clock    (1)" );
MPI( acq32_debug,            0, " pump up the debugs with this  (0)" );
MPI( acq32_command_debug,    0, "debug command sequence         (0)" );
MPI( acq32_busprot_debug,    0, "debug bus protocol             (0)" );
MPI( acq32_path_buffer_size, 0x4000,  "default path buffer size " );
MPI( acq32_instrument_buf_len, 0, "set length of inst buffer, if rqd" );

MPI( acq32_is_nonblocking,   1, "set TRUE for non blocking IO (1)" );

MPL( acq32_big_buf_base,     0, "set TRUE for dma->bigbuf (deprecated) (0) " );
MPL( acq32_big_buf_len,      0, "set LENGTH of dma->bigbuf (deprecated)(0) " );

MPI( acq32_dumdma_to,      200, "DUMDMA timout - turn up if debugs on (200) ");
MPI( acq32_fill_vma,         0, "fill vma with channel id data (for debug) " );
MPI( acq32_max_channels,     ACQ32_MAX_CHANNELS, "default channnel count" );
//WORKTODO - should come from board
                                        
MPI( acq32_double_tap, 2, "arm-abort-arm fills the I-cache (2) " );
MPI( acq32_debounce_clk, 0, "debounce clock (0) " );
MPI( acq32_enable_emtrig_bit, 0, "enable embedded trigger bit (0) " );
MPI( acq200_debug, 0, "enable acq200 debugs (0)" );
MPI( acq32_leak_detect, 0, "set to to enable leak detection" );

/*
 * Global Variables. Kept to the minimum (2)
 */


#define ABS_MAX_DEVICES 32

static struct {
    int ndevs;                            /* update on dev scan            */
	int idev;
    struct Acq32Device* 
    devices[ABS_MAX_DEVICES];            /* acq32_devices[0..acq32_ndevs] */
    struct semaphore ioread_fetch_mutex; /* global fetch mutex */
}
S_acq32;

extern char* acq32_acq32busprot_rev;
static void init_cos_task( struct Acq32Device* device );

struct Acq32Device* acq32_allocate_device(void)
{
	struct Acq32Device* device;

	ASSERT( S_acq32.idev < ABS_MAX_DEVICES-1 );

	device = KMALLOC( sizeof(struct Acq32Device), GFP_KERNEL );

	memset( device, 0, sizeof(struct Acq32Device));

	S_acq32.devices[S_acq32.idev] = device;

	S_acq32.idev++;

	return device;
}

extern int acq32_globalIoreadFetchMutexDown()
{
    MUTEX_DOWN( &S_acq32.ioread_fetch_mutex ); // returns on signal/error
    return 0;
}
extern void acq32_globalIoreadFetchMutexUp()
{
    MUTEX_UP( &S_acq32.ioread_fetch_mutex );
}


#ifndef ACQ200

static void allocateDmaBuffer( struct Acq32Device* device )
{

    PDEBUGL(4)(  " calling __get_free_pages( GFP_KERNEL, %d )\n",PAGE_ORDER);
    
    device->dmabuf.va = 
        (u32*)__get_free_pages( GFP_KERNEL, PAGE_ORDER );

    PDEBUGL(4)(  " back\n" );
    
    ASSERT( device->dmabuf.va );

    device->dmabuf.len = DMABUF_SIZE;

    PDEBUGL(2)( "dmabuf vaddr %p\n", device->dmabuf.va );
    device->dmabuf.pa = virt_to_phys( device->dmabuf.va );
    PDEBUGL(2)( "dmabuf paddr 0x%08x\n", device->dmabuf.pa);

    {
        int ii;

        for ( ii = 0; ii != DMABUF_SIZE/4; ++ii ){
            device->dmabuf.va[ii] = 0xdeadbeef;
        }
    }
}

static void freeDmaBuffer( struct Acq32Device* device )
{
    /* Pre-existing bug: this used to pass dmabuf.len (a size) as the
     * address to free_pages(), which panics on rmmod once free_pages
     * actually walks the page tables.  Pass the virtual address. */
    if ( device->dmabuf.len && device->dmabuf.va ){
        free_pages( (unsigned long)device->dmabuf.va, PAGE_ORDER );
        device->dmabuf.va = NULL;
        device->dmabuf.len = 0;
    }
}

#endif


int acq32_getDeviceCount() {
	return S_acq32.ndevs;
}
/*
 * Lookups on acq32_devices - 
 * use linear search of max 8 element array - ... at 300+MIPS overhead small!
 */
 
struct Acq32Device* acq32_get_device( dev_t i_rdev )
{
    int major = MAJOR( i_rdev );
    int isearch;
    
    for ( isearch = 0; isearch != S_acq32.ndevs; ++isearch ){
        struct Acq32Device* device = S_acq32.devices[isearch];
        
        if ( device->major == major ){
            return device;
        }
    }
    return 0;
}

int acq32_device_exists( dev_t i_rdev )
{
    return acq32_get_device( i_rdev ) != 0;
}

int acq32_get_board( dev_t i_rdev )
{
    int major = MAJOR( i_rdev );
    int isearch;
    
    for ( isearch = 0; isearch != S_acq32.ndevs; ++isearch ){
        struct Acq32Device* device = S_acq32.devices[isearch];
        
        if ( device->major == major ){
            return isearch;
        }
    }
    return -1;    
}

struct Acq32Device* acq32_get_device_from_filp( struct file* filp )
{
    return acq32_get_device(  filp->f_dentry->d_inode->i_rdev );
}

struct Acq32Device* acq32_get_device_from_board( int board )
{
    ASSERT( IN_RANGE( board, 0, S_acq32.ndevs-1 ) );

    return S_acq32.devices[board];
}

int acq32_get_board_from_device( struct Acq32Device* device )
{
    int isearch;
    
    for ( isearch = 0; isearch != S_acq32.ndevs; ++isearch ){
        struct Acq32Device* cursor = S_acq32.devices[isearch];
        
        if ( device == cursor ){
            return isearch;
        }
    }
    return -1;    
}

/* 
 * Timestamping
 */
 
static void normalize( struct timeval* tv )
{
    while ( tv->tv_usec > 1000000 ){
        tv->tv_usec -= 1000000;
        tv->tv_sec  += 1;
    }
}
int acq32_timestamp( struct Acq32Device* device, int id, const char* fmt, ... )
{
    struct timeval tv;
    va_list ap;
    va_start( ap, fmt );

    do_gettimeofday( &tv );

    if ( device->instrument_buf.cur-device->instrument_buf.buf < 
         acq32_instrument_buf_len ){
        device->instrument_buf.cur->id = id;

        vsprintf( device->instrument_buf.cur->ids, fmt, ap );
        ASSERT( strlen( device->instrument_buf.cur->ids ) < 
                sizeof( device->instrument_buf.cur->ids )   );

        normalize( &tv );
        device->instrument_buf.cur->tv = tv;
        device->instrument_buf.cur++;
    }

    va_end( ap );
    return 0;
}

unsigned acq32_getDeltaTime( 
    struct timeval* tv_prev, 
    struct timeval* tv_update 
    ) 
{
    struct timeval tv_now;
    unsigned delta_t;

    do_gettimeofday( &tv_now );

    delta_t =  (tv_now.tv_sec - tv_prev->tv_sec)*1000000 +
        tv_now.tv_usec - tv_prev->tv_usec;

    if ( tv_update ){
        memcpy( tv_update, &tv_now, sizeof(struct timeval) );
    }
    return delta_t;
}
/*
 * define a behaviour per minor device
 */

static struct file_operations acq32_fops;          // default ops
static struct file_operations acq32_test_fops;     // sandbox
#ifndef ACQ200
static struct file_operations acq32_rom_fops;      // for rom prog
#endif
static struct file_operations acq32_dmabuf_fops;   // debug dmabuf
static struct file_operations acq32_master_fops;   // master device control
static struct file_operations acq32_channel_fops; 
static struct file_operations acq32_rowdev_fops;
static struct file_operations acq32_hostbuf_fops;   // read bigbuf

static struct file_operations sim_channel_fops;
static struct file_operations sim_rowdev_fops;

static struct file_operations acq32_AO_immediate_fops;
static struct file_operations acq32_AO_fungen_fops;
static struct file_operations acq32_DO_immediate_fops;
static struct file_operations acq32_DO_fungen_fops;

static struct file_operations acq32_status_fops;

void acq32_readbuffer_create( struct ReadBuffer* rb )
{
    rb->rbuffer = KMALLOC( READBUFFERLEN, GFP_KERNEL );
    
    ASSERT( rb->rbuffer );
    rb->ibput = rb->ibget = 0;
}
void acq32_readbuffer_delete( struct ReadBuffer* rb )
{
    KFREE( rb->rbuffer );
    rb->ibput = rb->ibget = 0;
    rb->rbuffer = 0;
}


struct Acq32Path* acq32_makePathDescriptor( 
	dev_t minor, struct Acq32Device* device)
{
    struct Acq32Path*   path = KMALLOC( sizeof(struct Acq32Path), GFP_KERNEL );

    if ( !path ){
        return NULL;
    }
    memset( path, 0, sizeof(struct Acq32Path) );
    acq32_path_readbuffer_create( path );
    
    path->minor = minor;
    path->device = device;
    path->is_blocking = !acq32_is_nonblocking;
    path->nsamples = 0;

    // default is to use a _copy_ of channel info 
    path->info = &path->_info;
    path->buffer.start_sample = 0L;
    path->buffer.iput         = 0;
    path->buffer.iget         = 0;
    path->buffer.max_data     = acq32_path_buffer_size;

    path->buffer.data = KMALLOC( acq32_path_buffer_size, GFP_KERNEL );

    if ( !path->buffer.data ){
        KFREE( path );
        return NULL;
    }
    memcpy( path->info, &device->cfi, sizeof(Acq32ChannelFileInfo) );

    init_waitqueue_head( &path->return_waitq );

    return path;
}

static struct Acq32Path *makePathDescriptor(dev_t minor)
{
	return acq32_makePathDescriptor(minor, acq32_get_device(minor));
}



void acq32_freePathDescriptor( struct Acq32Path* path )
{
    ASSERT( path );
    acq32_path_readbuffer_delete( path );
    if ( path->buffer.data ){
        KFREE( path->buffer.data );
    }
    KFREE( path );    
}

/* acq32_locate / acq32_get_device_count removed: replaced by
 * pci_register_driver + acq32_pci_probe (see below). */

static void clearIoMapping( struct IoMapping* iomap, int is_rom )
{
    if ( !is_rom && iomap->va ){
        iounmap( iomap->va );
    }
#ifdef LINUX_NEW_PCI    
    if ( !is_rom && strlen( iomap->name ) ){
        release_mem_region( iomap->pa, iomap->len );
    }
#endif
    memset( iomap, 0, sizeof( struct IoMapping ) );     
}
static void unmap_pci_memory( struct Acq32Device* device )
{
    clearIoMapping( &device->csr, 0 );
    clearIoMapping( &device->ram, 0 );
    clearIoMapping( &device->rom, 1 );
}


#define CSR_SIZE_MAX 0x1000

int acqXX_mmap_csr( struct file* filp, struct vm_area_struct* vma )
{
	unsigned long vsize     = vma->vm_end - vma->vm_start;

	struct Acq32Device* device = PDEV(filp);
	unsigned long physical  = (long)device->csr.pa;
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

	if (physical & (PAGE_SIZE-1)){
		return -ENXIO;                    /* need aligned offsets */
	}else if (vsize > CSR_SIZE_MAX){
		return -EINVAL;                   /* spans too high */
	}else{
#ifdef PGMCOMOUT
		if (boot_cpu_data.x86 > 3)
			pgprot_val(vma->vm_page_prot) |= _PAGE_PCD;
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



#ifdef LINUX_NEW_PCI
int acq32_read_proc(
    char* buf, char** start, off_t offset,
    int len, int* unused1, void* unused2   )
#else
    int acq32_read_proc(char *buf, char **start, off_t offset,
                        int len, int unused)
#endif
{
    int idev;
#define LIMIT (PAGE_SIZE-80) /* don't print any more after this size */
#define PRINTF(fmt, args...) sprintf(buf+len, fmt, ## args)

    len = 0;

    if ( S_acq32.ndevs == 0 ){
        len += PRINTF( "acq32: NO DEVICES FOUND\n" );
        return len;
    }

#define ACQ32_FORMATT  "%5s %1s %3s %8s %12s %12s %12s %5s %s\n"
#define ACQ32_FORMATD  "%5s %1d %3s %8s %12s %12s %12s %5d %3d\n"
//#define ACQ32_FORMATD  "%d\n%5s\n%3s\n%8s\n%12s\n%12s\n%12s\n%5d\n%3d\n"

    len += PRINTF( ACQ32_FORMATT, 
                   "model", "n", "chn", "s/n", "arm-fw", "lca-fw", 
                   "cal date", "cycle", "heartbeat" );

    for( idev = 0; idev < S_acq32.ndevs; idev++) {
	struct Acq32Device *device = S_acq32.devices[idev];
	struct Acq32ImagesDef id = {
	    "ACQ32", "32", "D22222", "boot66c3", "acq32-lca", "251299"
	};
	int cycle = -1;
	int heartbeat = -1;

	if ( acq32_simulate == 0 ){
	    memset( &id, 0, sizeof(struct Acq32ImagesDef) );
	}else{
	    id.serialnum[strlen(id.serialnum)-1] += idev;
	    cycle = idev;
	    heartbeat = 98+idev;
	}

	if ( device->getImagesDef( device, &id ) == 0 ){
	    cycle = acq32_getCycle( device );
	    heartbeat = acq32_getHeartbeat( device );
	}

	len += PRINTF( ACQ32_FORMATD, 
		       id.model,
		       idev,
		       id.channels,
		       id.serialnum,
		       id.armfw,
		       id.lcafw,
		       id.caldate,
		       cycle,
		       heartbeat );
	if ( len > LIMIT ){
	    break;
	}
    }

    return len;

#undef ACQ32_FORMAT1
#undef ACQ32_FORMAT2
}










int acq32_read_proc_bus(
    char* buf, char** start, off_t offset,
    int len, int* unused1, void* unused2   )
{
    int idev;
#define LIMIT (PAGE_SIZE-80) /* don't print any more after this size */
#define PRINTF(fmt, args...) sprintf(buf+len, fmt, ## args)

    len = 0;

    if ( S_acq32.ndevs == 0 ){
        len += PRINTF( "acq32: NO DEVICES FOUND\n" );
        return len;
    }

    len += PRINTF( "# bus fun irq" "%10s %10s %10s\n", 
		   "IO", "SRAM", "ROM" );

#define ACQ32_FORMAT1  "%d %2d %2d "
#define ACQ32_FORMAT2  "0x%02x 0x%08lx 0x%08lx 0x%08lx\n"

    for( idev = 0; idev < S_acq32.ndevs; idev++) {
        struct Acq32Device *device = S_acq32.devices[idev];

        if ( acq32_simulate == 0 ){
            unsigned irq    = device->p_pci->irq;               
                
            len += PRINTF( ACQ32_FORMAT1,
                           idev, 
			   device->p_pci->bus->number, 
			   device->p_pci->devfn );
        
            len += PRINTF( ACQ32_FORMAT2, irq, 
                           device->csr.pa, 
                           device->ram.pa, 
                           device->rom.pa );

        }else{
            len += PRINTF( ACQ32_FORMAT1, idev, idev, idev );
            len += PRINTF( ACQ32_FORMAT2,
                           idev, 0xf0000000L, 0xf0000000L, 0xf0000000L );
        }
    }

    return len;

#undef ACQ32_FORMAT1
#undef ACQ32_FORMAT2
}



int acq32_report_version( char *buf, int max_len )
{
    int len = 0;

    len += PRINTF( "acq32-drv: build " __DATE__ " " __TIME__ "\n" );

    len += PRINTF( 
        "VERSION: %s num devices %d\n", DTACQ_RELEASE_STRING, S_acq32.ndevs );
    len += PRINTF( "acq32busprot_rev %s\n", acq32_acq32busprot_rev );
    
    ASSERT( len < max_len );
    
    return len;
}

int acq32_debug_read_proc(
    char* buf, char** start, off_t offset,
    int len, int* unused1, void* unused2   )
{
    int iboard;

    len = acq32_report_version( buf, 4096 );
    
    for ( iboard = 0; iboard != S_acq32.ndevs; ++iboard ){
        
        struct Acq32Device* device = acq32_get_device_from_board( iboard );

#define F_PRINTF( fmt, field ) \
        len += PRINTF( "%30s:" fmt "\n", #field, device->m_dpd.field )
#define F_PRINTF_OP( fmt, field, op ) \
        len += PRINTF( "%30s:" fmt "\n", #field, device->m_dpd.field op )

        len += PRINTF( "Device %d\n", iboard );
        F_PRINTF_OP( "%c", rom_is_enabled, ?'Y':'N' );
        F_PRINTF   ( "%d", nchannels );
        F_PRINTF   ( "%d", total_sample_length );
        F_PRINTF   ( "%d", state );
        F_PRINTF   ( "0x%08x", channel_mask );
        F_PRINTF   ( "%d", mode );
        F_PRINTF   ( "%d", nsamples_requested.pre );
        F_PRINTF   ( "%d", nsamples_requested.post );   
        F_PRINTF   ( "%d", nsamples_actual.pre );
        F_PRINTF   ( "%d", nsamples_actual.post );      

        F_PRINTF   ( "%d", itrigger );
        F_PRINTF   ( "%d", clock.internal );
        F_PRINTF   ( "%x", clock.doX );
        F_PRINTF   ( "%x", clock.diX );
        F_PRINTF   ( "%d", clock.divisor );
        F_PRINTF   ( "%d", sample_tagging );
        F_PRINTF   ( "0x%08x", streaming );

        len += PRINTF( "\n" );  
    }

    return len;
}



int acq32_debug2_read_proc(
    char* buf, char** start, off_t offset,
    int len, int* unused1, void* unused2   )
{
    int iboard;

    len = 0;

    for ( iboard = 0; iboard != S_acq32.ndevs; ++iboard ){
        
        struct Acq32Device* device = acq32_get_device_from_board( iboard );

#define D_PRINTF( fmt, field ) \
        len += PRINTF( "%30s:" fmt "\n", #field, device->field )

        len += PRINTF( "Device %d\n", iboard );

        D_PRINTF   ( "%6d",      m_dpd.i2o_packets_outgoing  );
        D_PRINTF   ( "%6d",      m_dpd.i2o_last_id_out       );
        D_PRINTF   ( "%6d",      m_dpd.i2o_packets_incoming  );
        D_PRINTF   ( "%6d",      m_dpd.i2o_packets_returned  );
        D_PRINTF   ( "%6d",      m_dpd.i2o_packets_discarded );
        D_PRINTF   ( "0x%p",     m_dpd.i2o_last_in           );
        D_PRINTF   ( "0x%p\n",   dmabuf.va                  );
        D_PRINTF   ( "%d",       streambuf.def.iput              );
        D_PRINTF   ( "%d",       streambuf.def.iget              );
        D_PRINTF   ( "%d",       streambuf.def.iputback          );
        D_PRINTF   ( "%d",       streambuf.def.qpresent          );
        D_PRINTF   ( "%d",       streambuf.def.nwrapped          ); 
        D_PRINTF   ( "%d\n",     streambuf.def.hitide            );
        len += PRINTF( "%30s: %d\n", "streamNumEntries()", 
                       streamNumEntries( &device->streambuf ) );

        D_PRINTF   ( "0x%08lx",    bigbuf.bb.pa     );
        D_PRINTF   ( "0x%p",     bigbuf.bb.va            );
        D_PRINTF   ( "0x%08x",   bigbuf.bb.len              );
        D_PRINTF   ( "%d",       bigbuf.contains_valid_data  );

        len += PRINTF( "\n" );  
    }

    return len;
}

int acq32_globs_read_proc(
    char* buf, char** start, off_t offset,
    int len, int* unused1, void* unused2   )
{
    len = 0;

#define G_PRINTF( glob, fmt ) \
    len += PRINTF( "%30s:" fmt "\n", #glob, glob )

    len += PRINTF( "MODULE_PARMS\n" );

    G_PRINTF(  acq32_use_interrupts, "%d" );
    G_PRINTF(  acq32_double_tap, "%d" );
    G_PRINTF(  acq32_simulate, "%d" );
    G_PRINTF(  acq32_gate_hi, "%d" );
    G_PRINTF(  acq32_extclock, "%d" );
    G_PRINTF(  acq32_debug, "%d" );
    G_PRINTF(  acq32_path_buffer_size, "%d" );
    G_PRINTF(  acq32_instrument_buf_len, "%d" );
    G_PRINTF(  acq32_is_nonblocking, "%d" );
    G_PRINTF( acq32_big_buf_base, "0x%08lx" );
    G_PRINTF( acq32_big_buf_len, "0x%08lx" );
    G_PRINTF( acq32_dumdma_to, "%d" );
    G_PRINTF( acq32_debounce_clk, "%d" );
    G_PRINTF( acq32_enable_emtrig_bit, "%d" );
    G_PRINTF( acq32_command_debug, "%d" );
    G_PRINTF( acq32_busprot_debug, "%d" );
    G_PRINTF( acq32_fill_vma, "%d" );
    
    len += PRINTF( "\n" );      

    return len;
}


/*
 * inst proc file:
 */

static void delta_tv( 
    struct timeval* diff, struct timeval* old, struct timeval* cur )
{
    struct timeval tnew;

    tnew = *cur;

    if ( cur->tv_usec < old->tv_usec ){
        cur->tv_usec += 1000000;
        cur->tv_sec -= 1;
    }
    if ( old->tv_sec ){
        diff->tv_sec  = cur->tv_sec  - old->tv_sec;
        diff->tv_usec = cur->tv_usec - old->tv_usec;
    }else{
        diff->tv_sec = diff->tv_usec = 0;
    }

    *old = tnew;
}


#ifdef LINUX_NEW_PCI
int acq32_inst_read_proc(
    char* buf, char** start, off_t offset,
    int len, int* unused1, void* unused2   )
#else
    int acq32_inst_read_proc(char *buf, char **start, off_t offset,
                             int len, int unused)
#endif
{
    len = 0;

    if ( acq32_instrument_buf_len == 0 ){
        len += PRINTF( "Instrumentation disabled\n" );
    }else{
        int iboard;               
   
        for ( iboard = 0; iboard != S_acq32.ndevs; ++iboard ){

            struct timeval prev = {};
            struct timeval cur;
            InstrumentBuf* ib;
            struct Acq32Device* device = acq32_get_device_from_board( iboard );

            len += PRINTF( "Board %d\n", iboard );

            for ( ib = device->instrument_buf.buf; 
                  ib != device->instrument_buf.cur; ++ib ){

                            
                delta_tv( &cur, &prev, &ib->tv );

                if ( len < LIMIT ){
                    len += PRINTF( "%c,%02ld.%06ld,%2ld.%06ld,%s\n",
                                   ib->id, ib->tv.tv_sec%100, ib->tv.tv_usec,
                                   cur.tv_sec, cur.tv_usec,
                                   ib->ids                     );
                }
                // /proc limited to 4K ... so log it as well
                PDEBUGL(0)( "%c,%02ld.%06ld,%2ld.%06ld,%s\n",
                            ib->id, ib->tv.tv_sec%100, ib->tv.tv_usec,
                            cur.tv_sec, cur.tv_usec,
                            ib->ids                     );

            }

            // clear the buffer for next time
            device->instrument_buf.cur = device->instrument_buf.buf;
        }
    }
                                
    return len;
}    



int verify_ioctl( struct inode *inode, struct file *filp,
                         unsigned int cmd, unsigned long arg)
{
#define MKRWDEF( arg ) ((struct ACQ32_RW_DEF*)arg)

    int size         = _IOC_SIZE(cmd); /* the size bitfield in cmd */

    /*
     * extract the type and number bitfields, and don't decode
     * wrong cmds: return EINVAL before verify_area()
     */
    if (_IOC_TYPE(cmd) != ACQ32_IOC_MAGIC) return -EINVAL;
    if (_IOC_NR(cmd) > ACQ32_IOC_MAXNR)    return -EINVAL;

    /*
     * the direction is a bitmask, and VERIFY_WRITE catches R/W
     * transfers. `Type' is user-oriented, while
     * access_ok is kernel-oriented, so the concept of "read" and
     * "write" is reversed.  access_ok returns 1 on success (opposite of
     * the old verify_area which returned 0 on success).
     */
    if ((_IOC_DIR(cmd) & _IOC_READ) &&
        !access_ok(VERIFY_WRITE, (void __user *)arg, size))
        return -EINVAL;
    else if ((_IOC_DIR(cmd) & _IOC_WRITE) &&
             !access_ok(VERIFY_READ, (void __user *)arg, size))
        return -EINVAL;
    else
        return 0;
}
                 
int acq32_ioctl (struct inode *inode, struct file *filp,
                 unsigned int cmd, unsigned long arg)
{
    struct Acq32Device *dev = acq32_get_device( filp->f_dentry->d_inode->i_rdev );
    int rc = verify_ioctl( inode, filp, cmd, arg );
    
    if ( rc != 0 ) return rc;
    
    switch(cmd) {
    case ACQ32_IOCHARDRESET:
    {
        int maxtries = 0;

        while ( MOD_IN_USE ){
            if ( ++maxtries > 10 ){
                PDEBUG( "acq32_ioctl() - giving up\n" );
                return -EIO;
            }else{
                MOD_DEC_USE_COUNT;
                PDEBUG( "acq32_ioctl() MOD_DEC_USE_COUNT\n" );
            }
        }
        MOD_INC_USE_COUNT;            /* close() sets to zero */
    }
    break;
    case ACQ32_IOREAD_LOCALBUF:
        
        PDEBUGL(2)( "ioctl ACQ32_IOREAD_LOCALBUF\n" );
                        
        if ( IN_RANGE( CHANNEL(filp->f_dentry->d_inode->i_rdev), 0, 31 ) ){

            struct READ_LOCALBUF_DESCR descr;
                
            copy_from_user( &descr, (void*)arg, sizeof(descr) );
                
            return dev->fetchDataToLocalBuffer( 
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
    case ACQ32_IOREAD_GETPHYSICAL:
        if ( IN_RANGE( CHANNEL(filp->f_dentry->d_inode->i_rdev), 0, 31 ) ){
            return acq32_getBufferPhysicalAddr( 
                PD(filp),
                CHANNEL(filp->f_dentry->d_inode->i_rdev),
                (void*)arg
                );
        }else{
            return -EINVAL;
        }
       
    case ACQ32_IOS_VALIDATE_BIGBUF:
	acq32_validate_bigbuf( PDEV(filp), (unsigned)arg );
	break;
    case ACQ32_IOSMBX( 0 ):
    case ACQ32_IOSMBX( 1 ):
    case ACQ32_IOSMBX( 2 ):
    case ACQ32_IOSMBX( 3 ):
	dev->set_mailbox( dev, _IOC_NR(cmd)-ACQ32_IO_MBX_NR, arg );
        break;
#ifndef ACQ200
    case ACQ32_IOSROM_WORD_A01:
        set_rom_word_A01( dev, arg );
        break;
#endif
    case ACQ32_IOGMBX( 0 ):
    case ACQ32_IOGMBX( 1 ):
    case ACQ32_IOGMBX( 2 ):
    case ACQ32_IOGMBX( 3 ): 
    {
        unsigned value;
        dev->get_mailbox( dev, _IOC_NR( cmd )-ACQ32_IO_MBX_NR, &value );

        put_user( value, (unsigned*)arg );
    }
    break;    
    case ACQ32_IOSLIMIT:
        // WORKTODO
        PD(filp)->nsamples = 
            MIN( filp->f_pos+arg, PDEV(filp)->m_dpd.nsamples_actual.post ); 
        break;
    case ACQ32_IOSFORMAT:
        PD(filp)->info->mode = arg;
        break;
    case ACQ32_IOSENDI2O:
        return acq32_sendI2O( PD( filp ), (void*)arg );
    case ACQ32_IOS_INTS_DISABLE:
	return acq32_intsDisable( (unsigned)arg );
    case ACQ32_IOS_INTS_ENABLE:
	return acq32_intsEnable( (unsigned)arg );
    case ACQ32_IOS_SWALLOW_KMEM:
	return acq32_swallowKmem( (unsigned)arg );
    case ACQ32_IOARMSPIN:
        acq32_setArmSpin( PDEV(filp));
        break;
    case ACQ32_IOSDEBUG:
        switch( arg ){
        case 2000:
            acq32_busprot_debug = 0;
            break;
        case 2001:
            acq32_busprot_debug = 1;
            break;
        case 1000:
            acq32_command_debug = 0;
            break;
        case 1001:
            acq32_command_debug = 1;
            break;
        default:
            acq32_debug = arg;
        }
        break;
    case ACQ32_IOG_PCIREGS_OFFSET: {
	    unsigned ba = dev->csr.pa&(PAGE_SIZE-1);
#ifdef ACQ200
	    ba += 0x10;
#endif
	    put_user(ba, (unsigned*)arg);
        break;
    }
#ifndef ACQ200
// target memory access
    case ACQ32_IOG_READ32:
        return acq32_userTargetAccessRead32( inode, dev, MKRWDEF(arg) );
    case ACQ32_IOS_WRITE32:
        return acq32_userTargetAccessWrite32( inode, dev, MKRWDEF(arg) );
    case ACQ32_IOG_READ16:
        return acq32_userTargetAccessRead16( inode, dev, MKRWDEF(arg) );
    case ACQ32_IOS_WRITE16:
        return acq32_userTargetAccessWrite16( inode, dev, MKRWDEF(arg) );
    case ACQ32_IOS_WAIT_EVENT:
        return acq32_WaitEvent( PD(filp), (struct ACQ32_WAIT_DEF*)arg );
#endif
    default:  
        return -EINVAL;
    }
    return 0;
}


int acq32_rowdev_ioctl (struct inode *inode, struct file *filp,
                        unsigned int cmd, unsigned long arg)
{
    int rc = verify_ioctl( inode, filp, cmd, arg );
    
    if ( rc != 0 ) return rc;
    
    switch(cmd) {
    case ACQ32_IOREAD_LOCALBUF:
        
        PDEBUGL(2)( "ioctl ACQ32_IOREAD_LOCALBUF\n" );
                        
        if ( IN_RANGE( CHANNEL(filp->f_dentry->d_inode->i_rdev), 0, 31 ) ){

            struct READ_LOCALBUF_DESCR descr;
                
            copy_from_user( &descr, (void*)arg, sizeof(descr) );
                
            return acq32_fetchDataToLocalBuffer( 
                filp,
                ALLCHANNELS,
                descr.buffer,
                descr.nsamples,
                descr.istart,
                descr.istride
                );
        }else{
            return -EINVAL;
        }
    default:
        return acq32_ioctl( inode, filp, cmd, arg );
    }
}

static const char* getDevName(struct Acq32Device *device)
{
	if (strlen(device->devname) == 0){
		int idev;

		for ( idev = 0; idev != S_acq32.idev; ++idev ){
			if ( device == S_acq32.devices[idev] ){
				sprintf( device->devname, 
					 PROCFILENAME ".%d", idev );
				return device->devname;
			}
		}

		strcpy( device->devname, PROCFILENAME ".X" );
	}   
	return device->devname;
}
/*
 * responsibilities:
 *    specific_open - shall call MOD_INC_USE_COUNT() on success
 *    specific_close - shall call acq32_close()
 */

int acq32_request_irq( struct Acq32Path* path )
{

    struct Acq32Device* device = path->device; 
    const char* isr_name = getDevName(device);
    int rv = 0;
    int result = 0;
    
    ASSERT( device->nclients >= 0 );    
    
    PDEBUGL(3)( "acq32_request_irq() nclients %d\n", device->nclients );

    down( &device->m_dpd.irq_req_mutex );

    if ( ++device->nclients == 1 ){
/*
 * if interrupts are requested globally, AND the board is wanting an interrupt,
 * ask Linux for the use thereof
 */
        if ( acq32_use_interrupts != 0 && device->p_pci->irq != 0 ){

            result = request_irq(
                device->p_pci->irq,
                device->isr,
                IRQF_SHARED,
                isr_name,
                device
                );

            device->use_interrupts = result==0;
            path->uses_irq = 1;
        }
        rv = device->p_md->OnOpen && device->p_md->OnOpen( path );
    }

    up( &device->m_dpd.irq_req_mutex );

    if ( result != 0 ){
        PDEBUGL(3)( "request_irq failed: %d\n", result );
    }
       
    return rv;
}


int acq32_free_irq( struct Acq32Path* path )
{
    struct Acq32Device* device = path->device;
    int rv = 0;

    if ( path->uses_irq == 0 ){
        return 0;
    }
        
    PDEBUGL(3)( "acq32_free_irq() nclients %d\n", device->nclients );

    down( &device->m_dpd.irq_req_mutex );
    
    if ( --device->nclients == 0 ){
        int used_interrupts = device->use_interrupts != 0;
        rv = device->p_md->OnRelease && device->p_md->OnRelease( path ); 
            
        if ( used_interrupts ){
            free_irq( device->p_pci->irq, device );
            device->use_interrupts = 0;
        }
    }
    
    up( &device->m_dpd.irq_req_mutex );
    
    ASSERT( device->nclients >= 0 );
    return rv;
}


/*
 * acq32_fops ... generic actions
 */

int acq32_open( struct inode* inode, struct file* filp )
{
    int ierrno;
    int should_request_irq = 1;        // most node types should try it

    if ( !acq32_device_exists( inode->i_rdev )  ){
        return -ENODEV;
    }

    PDEBUGL(3)( "acq32_open filp->f_op %p\n", filp->f_op );

    /*
     * set up PATH structure
     * info is a _copy_ of device info by default
     * other open()'s may override ...
     */
    filp->private_data = makePathDescriptor( inode->i_rdev );

    if ( PDEV(filp)->in_exclusive_use ){
        return -EBUSY;
    }
    if ( !filp->private_data ){
        return -ENOMEM;
    }
    
/* customise methods according to device class */
    
    if ( IS_MASTER( inode->i_rdev ) ){
        filp->f_op = &acq32_master_fops;
    }else if ( IS_CHANNEL_DEV( inode->i_rdev ) ){
        int channel = CHANNEL( inode->i_rdev );
        
        if ( !IN_RANGE( channel, 0, ACQ32_MAX_CHANNELS-1 ) ){
            return -ECHRNG;
        }
        filp->f_op = acq32_simulate? &sim_channel_fops: &acq32_channel_fops;        
        
        PDEBUGL(3)( "acq32_open minor: %d (%d) Channel %d\n", 
                    inode->i_rdev, inode->i_rdev, channel );
            
    }else if ( ROWDEV( inode->i_rdev ) ){
        filp->f_op = acq32_simulate? &sim_rowdev_fops: &acq32_rowdev_fops;
    }else if ( STATDEV( inode->i_rdev ) ){
	filp->f_op = &acq32_status_fops;
#ifndef ACQ200
    }else if ( IS_ROM( inode->i_rdev ) ){ 
        filp->f_op = &acq32_rom_fops;
        should_request_irq = 0;
#endif
    }else if ( DEVRAW( inode->i_rdev ) ){
        should_request_irq = 0;
    }else if ( DEVDMABUF( inode->i_rdev ) ){
        filp->f_op = &acq32_dmabuf_fops;
    }else if ( BIGDUMPDEV( inode->i_rdev ) ){
        filp->f_op = &acq32_hostbuf_fops;
    }else if ( TESTDEV( inode->i_rdev ) ){
        filp->f_op = &acq32_test_fops;
    }else if ( AOIDEV( inode->i_rdev ) ){
        filp->f_op = &acq32_AO_immediate_fops;
    }else if ( AOFDEV( inode->i_rdev ) || AOFRDEV( inode->i_rdev ) ){
        filp->f_op = &acq32_AO_fungen_fops;
    }else if ( DOIDEV( inode->i_rdev ) ){
        filp->f_op = &acq32_DO_immediate_fops;
    }else if ( DOFDEV( inode->i_rdev ) || DOFRDEV( inode->i_rdev ) ){
        filp->f_op = &acq32_DO_fungen_fops;
    }else{
        return -ENODEV;
    } 

    // if a new open method has been specified, call it

    if ( filp->f_op->open != NULL                  &&       // 1
         filp->f_op->open != acq32_open            &&       // 2
         (ierrno = filp->f_op->open( inode, filp ))   ){    // 3
        return ierrno;
    }else{
        if ( should_request_irq ) {
            acq32_request_irq( PD(filp) );
        }
        MOD_INC_USE_COUNT;
        return 0;
    }
}

int acq32_release( struct inode* inode, struct file* filp )
{
    PDEBUGL(3)( "acq32_release()\n" );
    acq32_free_irq( PD(filp) );
    PDEV(filp)->in_exclusive_use = 0;
    acq32_freePathDescriptor( (struct Acq32Path*)filp->private_data );
    MOD_DEC_USE_COUNT;
    return 0;
}

int acq32_master_release( struct inode* inode, struct file* filp )
// transfer any residual response from path buffer to device buffer
{
    acq32_path_readbuffer_stash( PD( filp ) );
    return acq32_release( inode, filp );
}

int acq32_status_open( struct inode* inode, struct file* filp )
{
    PD(filp)->my_cos_index = PDEV(filp)->m_dpd.cos.enable++;

    if ( PD(filp)->my_cos_index >= MAX_COS_PATHS ){
	PDEV(filp)->m_dpd.cos.enable--;
	PD(filp)->my_cos_index = -1;
	return -EBUSY;
    }else{
	PDEV(filp)->m_dpd.cos.files[PD(filp)->my_cos_index] = filp;
	PDEBUGL(1)( "acq32_status_open() OK\n" );
	return 0;
    }
}

int acq32_status_release( struct inode* inode, struct file* filp )
{
    if ( PD(filp)->my_cos_index != -1 ){
	PDEV(filp)->m_dpd.cos.files[PD(filp)->my_cos_index] = 0;
	PDEV(filp)->m_dpd.cos.enable--;
	PD(filp)->my_cos_index = -1;
    }
    return acq32_release( inode, filp );
}


#ifndef ACQ200
void acq32_enable_rom(struct Acq32Device* device, int enable);


int acq32_rom_open( struct inode* inode, struct file* filp )
{
        struct Acq32Device* device = PDEV(filp);

        PDEBUGL(3)( "rom_open" );
            
        if ( device->nclients >= 1 ){
                return -EBUSY;
        }else{
                device->in_exclusive_use = 1;
                acq32_enable_rom( PDEV(filp), TRUE );
       }
       return 0;
}

int acq32_rom_release( struct inode* inode, struct file* filp )
{
    acq32_enable_rom( PDEV(filp), FALSE );
    return acq32_release( inode, filp );
}
#endif

int acq32_master_open( struct inode* inode, struct file* filp )
{  
    PD(filp)->info = &PDEV(filp)->cfi;  // USE DEVICE GLOBAL info
    return 0;
}


void acq32_show_busy( int set )
{
    if ( set ){     
        MOD_INC_USE_COUNT;
    }else{
        MOD_DEC_USE_COUNT;
    }
}

ssize_t linear_buffer_read( 
    struct file *filp,
    char* buf,
    size_t count,
    char* pkmem,
    int kmax
    )
{
    unsigned long f_pos = (unsigned long)(filp->f_pos);

    if ( pkmem == NULL ){
        return 0;
    }else if ( f_pos > kmax ){
        return 0;
    }else if ( f_pos + count > kmax ){
        count = kmax - f_pos;
    }

    // not sure if memcpy_fromio can copy to user buf, so use temp
    {
        int running_count = 0;
        int ncopy;
            
        for ( ; (ncopy = MIN( count-running_count, sizeof(PD(filp)->scratch) )); 
              running_count += ncopy ){
            memcpy_fromio( PD(filp)->scratch, pkmem+f_pos+running_count, ncopy );
            copy_to_user( buf+running_count, PD(filp)->scratch, ncopy );
        }
    }

    filp->f_pos += count;
    return count;
}

ssize_t 
acq32_dmabuf_read ( 
    struct file* filp, char* buf, size_t count, loff_t* posp 
    )
{
    return linear_buffer_read(
        filp, buf, count, (char*)PDEV(filp)->dmabuf.va, DMABUF_SIZE );
}


ssize_t 
acq32_test_read ( struct file* filp, char* buf, size_t count, loff_t* posp )
// WORKTODO .... read data from debug output buffer. currently done at use level
{
#ifdef LINUX_NEW_PCI
    PDEBUGL(1)( "LINUX_NEW_PCI stub\n" );
    return 0;
#else
    return 0;
#endif
}

ssize_t 
acq32_test_write (struct file *filp, const char *ubuf, size_t count, loff_t * posp)
// write data as if from debug port
{
    
    // get an I2o buffer, copy data from ubuf to it, send it off
    return 0;   
}
 
 
loff_t  
acq32_lseek ( struct file* filp, loff_t off, int whence )
{
    struct Acq32Device* device = PDEV(filp);
    loff_t newpos = 0;

    PDEBUGL(1)( "acq32_lseek() f_pos %lld off %lld whence %d new %lld\n", 
                filp->f_pos, off, whence, newpos );


    switch(whence) {
    case 0: /* SEEK_SET .. relative to beginning */
        newpos = off;
        break;
    case 1: /* SEEK_CUR .. relative to current */
        newpos = filp->f_pos + off;
        break;
    case 2: /* SEEK_END .. relative to end */
        newpos = device->m_dpd.nsamples_actual.post - off;
        break;
    case 3:  /* SEEK_TRIG ... pgm special: relative to trigger */
        newpos = device->m_dpd.itrigger + off;
        break;
    default: /* can't happen */
        return -EINVAL;
    }

    PDEBUGL(1)( "acq32_lseek() f_pos %lld off %lld whence %d new %lld\n", 
                filp->f_pos, off, whence, newpos );

    if ( newpos<0 ){
        return -EINVAL;
    }else{
        return filp->f_pos = newpos;
    }
}



extern int acq32_mmap_channel( struct file* filp, struct vm_area_struct* vma );


int acq32_mmap_bigphysmem( struct file* filp, struct vm_area_struct* vma )
// mmap the large contig area of mem at acq32_big_buf_base to vma
// NB: assumes this memory has been excluded from the kernel before boot
// NB: this is crude but effective.
{
#ifndef LINUX_NEW_PCI
    unsigned long off       = vma->vm_offset;
#endif    
    unsigned long vsize     = vma->vm_end - vma->vm_start;

    // assume DMA dump to local RAM, 64M up
    unsigned long physical = acq32_big_buf_base;

#ifndef LINUX_NEW_PCI
    physical += off;
#endif    

#if 0
    PDEBUGL(2)( "acq32_mmap(): start off:%d physical:0x%08x vsize:0x%x",
                off, physical, vsize );
#endif
#ifndef LINUX_NEW_PCI
    if ( off & (PAGE_SIZE-1) ){
        return -ENXIO;                          /* need aligned offsets */
    }else if ( vsize > acq32_big_buf_len-off ){
        return -EINVAL;                         /* spans too high */
    }
#endif     
       
    return remap_pfn_range(
        vma, vma->vm_start, physical >> PAGE_SHIFT, vsize, vma->vm_page_prot
        );
}


int _acq32_mmap_host( struct file* filp, struct vm_area_struct* vma )
{
#ifndef LINUX_NEW_PCI
    unsigned long off       = vma->vm_offset;
#else
    unsigned long off       = 0;
#endif    
    unsigned long vsize     = vma->vm_end - vma->vm_start;

    // assume DMA dump to local RAM, 64M up
    unsigned long physical = acq32_big_buf_base;
#ifndef LINUX_NEW_PCI
    physical += off;
#endif    
    PDEBUGL(2)( "acq32_mmap(): start off:%ld physical:0x%08lx vsize:0x%lx",
                off, physical, vsize );

#ifndef LINUX_NEW_PCI
    if ( off & (PAGE_SIZE-1) ){
        return -ENXIO;                          /* need aligned offsets */
    }else if ( vsize > acq32_big_buf_len-off ){
        return -EINVAL;                         /* spans too high */
    }else
#endif    
        return remap_pfn_range(
		vma, vma->vm_start, physical >> PAGE_SHIFT, vsize, vma->vm_page_prot );
}

int _acq200_mmap_host( struct file* filp, struct vm_area_struct* vma )
{
	struct Acq32Device* dev = PDEV(filp);
	unsigned long off       = 0;
	unsigned long vsize     = vma->vm_end - vma->vm_start;

	unsigned long physical = dev->dmabuf.pa;

	PDEBUGL(2)( "acq32_mmap(): start off:%ld physical:0x%08lx vsize:0x%lx",
		    off, physical, vsize );

        return remap_pfn_range(
		vma, vma->vm_start, physical >> PAGE_SHIFT, vsize, vma->vm_page_prot );
}


ssize_t
acq32_hostbuf_read ( struct file* filp, char* buf, size_t count, loff_t* posp )
/* output host buffer settings */
{
	char myline[80];
	struct Acq32Device* dev = PDEV(filp);
	int pos = (int)*posp;

	int len = sprintf(myline, "BIGBUF: pa=0x%08x len=0x%08x\n",
			dev->dmabuf.pa, dev->dmabuf.len);

	if (pos < len){
		len = min((int)count, len - pos);
		copy_to_user(buf, myline + pos, len);
		*posp += len;
		return len;
	}else{
		return 0;
	}
}


int acq32_mmap_host(struct file* filp, struct vm_area_struct* vma )
{
	struct Acq32Device* dev = PDEV(filp);

	if (dev->dmabuf.va == dev->bigbuf.bb.va){
		return _acq200_mmap_host(filp, vma);
	}else{
		return _acq32_mmap_host(filp, vma);
	}
}
/*
 * instantiate fops 
 */

/*
 * NB: the release field in ALL fops MUST call acq32_release,
 * either directly or indirectly .. or there will be a resource leak
 */

static struct file_operations acq32_fops = {          // default ops
    .owner = THIS_MODULE,
#ifndef ACQ200
    llseek:  acq32_lseek,
    read:    acq32_read,

    mmap:    acq32_mmap,    // acq32_mmap
#endif
    ioctl:   acq32_ioctl,    // acq32_ioctl
    mmap:    acqXX_mmap_csr,
    open:    acq32_open,
    release: acq32_release,
};


static struct file_operations acq32_AO_immediate_fops = {
    .owner = THIS_MODULE,
    write:    acq32_AO_immediate_write,
    release:  acq32_release,
};
static struct file_operations acq32_AO_fungen_fops = {
    .owner = THIS_MODULE,
    write:    acq32_AO_function_write,
    open:     acq32_AO_fungen_open,
    release:  acq32_AO_fungen_release,
};

static struct file_operations acq32_DO_immediate_fops = {
    .owner = THIS_MODULE,
    write:    acq32_DO_immediate_write,
    release:  acq32_release,
};

static struct file_operations acq32_DO_fungen_fops = {
    .owner = THIS_MODULE,
    write:    acq32_DO_function_write,
    open:     acq32_DO_fungen_open,
    release:  acq32_DO_fungen_release,
};


static struct file_operations acq32_test_fops = {
    .owner = THIS_MODULE,
    read:     acq32_test_read,
    release:  acq32_release,
};
#ifndef ACQ200
static struct file_operations acq32_rom_fops = {
    .owner = THIS_MODULE,
    read:    acq32_rom_read,    // acq32_read
    ioctl:   acq32_ioctl,       // acq32_ioctl
    mmap:    acq32_mmap_rom,    // acq32_mmap
    open:    acq32_rom_open,    // acq32_open
    release: acq32_rom_release, // acq32_release
};
#endif
static struct file_operations acq32_dmabuf_fops = {
    .owner = THIS_MODULE,
#ifndef ACQ200
    read:    acq32_dmabuf_read,    // acq32_read
#endif
    ioctl:   acq32_ioctl,          // acq32_ioctl
    mmap:    acq32_mmap_bigphysmem,// acq32_mmap
    release: acq32_release,
};

static struct file_operations acq32_hostbuf_fops = {
	.owner = THIS_MODULE,
	read: acq32_hostbuf_read,
	mmap:    acq32_mmap_host,    // acq32_mmap
	release: acq32_release,
};

static struct file_operations acq32_master_fops = {
    .owner = THIS_MODULE,
    read:    acq32_master_read,    // acq32_read
    write:   acq32_master_write,   // acq32_write
    ioctl:   acq32_ioctl,          // acq32_ioctl
    open:    acq32_master_open,    // acq32_open
    release: acq32_master_release, // acq32_release
};

static struct file_operations acq32_channel_fops = {
    .owner = THIS_MODULE,
    read:    acq32_channel_read,    // acq32_read
    write:   acq32_data_write,      // acq32_write
    ioctl:   acq32_ioctl,           // acq32_ioctl
    mmap:    acq32_mmap_channel,    // acq32_mmap
    release: acq32_release,         // acq32_release
};

static struct file_operations acq32_rowdev_fops = {
    .owner = THIS_MODULE,
    read:    acq32_row_read,        // acq32_read
    write:   acq32_data_write,      // acq32_write
    ioctl:   acq32_ioctl,           // acq32_ioctl
    release: acq32_release,         // acq32_release
};

static struct file_operations acq32_status_fops = {
    .owner = THIS_MODULE,
    open:    acq32_status_open,
    read:    acq32_status_read,
    write:   acq32_status_write,
    release: acq32_status_release,
};
static struct file_operations sim_channel_fops = {
    .owner = THIS_MODULE,
    read:    sim_channel_read,    // acq32_read
    write:   acq32_data_write,    // acq32_write
    ioctl:   acq32_ioctl,    // acq32_ioctl
    release: acq32_release,    // acq32_release
    mmap:    acq32_mmap_channel,    // acq32_mmap
};

static struct file_operations sim_rowdev_fops = {
    .owner = THIS_MODULE,
    read:    sim_rowdev_read,    // acq32_read
    write:   acq32_data_write,    // acq32_write
    ioctl:   acq32_ioctl,    // acq32_ioctl
    release: acq32_release,    // acq32_release
};




static void cos_task( void* args ) {
    // handles all changes of state for device.
    // WORKTODO - termination ??
    struct Acq32Device* device = (struct Acq32Device*)args;
    int ifilp;
    struct file* filp;

    PDEBUGL(1)( "cos_task() 2\n" );

    while ( !statbufIsEmpty( &device->statusbuf ) ){
	u32 status = statbufGet( &device->statusbuf );
    
	for ( ifilp = 0; ifilp != MAX_COS_PATHS; ++ifilp ){

	    PDEBUGL(3)( "cos_task [%d] filp %p\n", 
			ifilp, device->m_dpd.cos.files[ifilp] );

	    if ( (filp = device->m_dpd.cos.files[ifilp] ) ){
		acq32_doGetStateWork( filp, status, device->m_dpd.last_status);
	    }
	}
	device->m_dpd.last_status = status;
    }

    for ( ifilp = 0; ifilp != MAX_COS_PATHS; ++ifilp ){
	if ( (filp = device->m_dpd.cos.files[ifilp] ) ){
	    wake_up_interruptible( &PD(filp)->return_waitq );
	}
    }
}

static void init_cos_task( struct Acq32Device* device ) {
    if ( !device->m_dpd.cos.task_has_been_created ){
	INIT_TQUEUE( &device->m_dpd.cos.task, cos_task, device );
	device->m_dpd.cos.task_has_been_created = 1;
    }
}
/*
 * Finally, the module stuff
 */






#include <linux/delay.h>


#include "acq200_debug.h"


extern void acq32_device_init(struct Acq32Device *device);

struct Acq32Device* 
acq32_create_device(struct pci_dev* p_dev )
{
	struct Acq32Device *device = acq32_allocate_device();
	getDevName(device);

	/*
	 * Register your major, and accept a dynamic number
	 */

	info("register_chrdev %s\n", device->devname);

	device->major = register_chrdev( 
	    device->major = 0, device->devname, &acq32_fops );
 
	if ( device->major < 0) {
	    printk(KERN_WARNING "acq32: can't get major %d\n", device->major );
	    return 0;
	}
 
	device->nclients = 0;
 
	device->cfi.mode = CM_BINARY;
 
	device->p_pci = p_dev;
	memcpy( &device->pci_stash, p_dev, sizeof(struct pci_dev));
 
	device->p_md = acq32_simulate?
	    acq32_getSimul(): acq32_getDriver();
 
	device->m_dpd.rom_is_enabled = FALSE;
              
	acq32_device_readbuffer_create( device );
	/*
	 * WORKTODO ... deduce this info from board type
	 */
	device->m_dpd.nchannels           = ACQ32_MAX_CHANNELS;   
	device->m_dpd.total_sample_length = DEF_SAMPLE_TOTAL;
	device->m_dpd.state               = ST_STOP;
	device->m_dpd.channel_mask        = DEF_CHANNEL_MASK;
	device->m_dpd.mode                = M_GATED_TRANSIENT;
 
	device->m_dpd.nsamples_requested.pre  =
	    device->m_dpd.nsamples_requested.post =
	    device->m_dpd.nsamples_actual.pre =
	    device->m_dpd.nsamples_actual.post= 0;
	memset( &device->m_dpd.work_task, 0, sizeof(device->m_dpd.work_task) );
 
	init_waitqueue_head( &device->m_dpd.waitq );
	init_waitqueue_head( &device->m_dpd.aux_waitq );
 
	init_MUTEX( &device->m_dpd.mbox_mutex );
	init_MUTEX( &device->m_dpd.irq_req_mutex );
	init_MUTEX( &device->m_dpd.ioread_fetch_mutex );
 
	appbufInit( &device->appbuf );
	streamBufInit( &device->streambuf );
	streamBufInit( &device->downstreambuf );
	statBufInit( &device->statusbuf );

	if ( acq32_instrument_buf_len ){
		device->instrument_buf.cur =
			device->instrument_buf.buf = 
			KMALLOC( acq32_instrument_buf_len*
				 sizeof(InstrumentBuf), 
				 GFP_KERNEL );
	}else{
		device->instrument_buf.cur = 
			device->instrument_buf.buf = NULL;
	}

	init_cos_task( device );
	
	info("99 device %s major %d", device->devname, device->major);
	return device;
}


void init_fops(void)
{
	/* SET_MODULE_OWNER() was a 2.4 macro that assigned fops->owner
	 * = THIS_MODULE.  In 2.6 the macro is gone; .owner is set
	 * statically in each file_operations initialiser above, so this
	 * function is now a no-op (kept as a stable call-site for the
	 * existing init_module path). */
}


void init_proc_entries(void)
{
// register the /proc fs entries

#define CPRE(name, entry ) \
        create_proc_read_entry( name, 0, NULL, entry, 0)
	CPRE(PROCFILENAME, acq32_read_proc);
	CPRE(PROCFILENAME ".bus", acq32_read_proc_bus);
	CPRE("." PROCFILENAME, acq32_debug_read_proc);
	CPRE("." PROCFILENAME ".d2", acq32_debug2_read_proc);
	CPRE("." PROCFILENAME ".globs", acq32_globs_read_proc);
	CPRE("." PROCFILENAME ".inst", acq32_inst_read_proc);
#undef CPRE
}

void remove_proc_entries(void)
{
// register the /proc fs entries

#define RMPE(name, entry ) \
        remove_proc_entry( name, 0)
	RMPE(PROCFILENAME, acq32_read_proc);
	RMPE(PROCFILENAME ".bus", acq32_read_proc_bus);
	RMPE("." PROCFILENAME, acq32_debug_read_proc);
	RMPE("." PROCFILENAME ".d2", acq32_debug2_read_proc);
	RMPE("." PROCFILENAME ".globs", acq32_globs_read_proc);
	RMPE("." PROCFILENAME ".inst", acq32_inst_read_proc);
#undef RMPE
}

static void coreDevInit( struct Acq32Device *device )
{
	if ( acq32_simulate ){
		return;
	}

	device->coreDevInit( device );
}

#ifndef ACQ200
/*
 * 2.6 PCI driver model: register a pci_driver and let the kernel call
 * acq32_pci_probe() once per matching device.  This replaces the 2.4
 * pci_find_device() polling loop.
 */

static int acq32_pci_probe( struct pci_dev* p_dev,
                            const struct pci_device_id* id )
{
    struct Acq32Device* device;
    int rc;

    rc = pci_enable_device( p_dev );
    if ( rc ){
        PDEBUG( "pci_enable_device failed: %d\n", rc );
        return rc;
    }

    device = acq32_create_device( p_dev );
    if ( !device ){
        pci_disable_device( p_dev );
        return -ENOMEM;
    }

    allocateDmaBuffer( device );
    acq32_device_init( device );

    pci_set_drvdata( p_dev, device );
    return 0;
}

static void acq32_pci_remove( struct pci_dev* p_dev )
{
    /* Per-device teardown is performed in cleanup_module() by walking
     * S_acq32.devices[] (preserves the original 2.4 cleanup ordering).
     * pci_disable_device() is paired here to undo pci_enable_device()
     * from probe; the device struct itself is freed in cleanup_module. */
    pci_disable_device( p_dev );
}

static struct pci_device_id acq32_pci_id_table[] = {
    { PCI_DEVICE( VENDOR_ID, DEVICE_ID ) },
    { 0, }
};
MODULE_DEVICE_TABLE( pci, acq32_pci_id_table );

static struct pci_driver acq32_pci_driver = {
    .name     = "acq32",
    .id_table = acq32_pci_id_table,
    .probe    = acq32_pci_probe,
    .remove   = acq32_pci_remove,
};

#define MAXDEAD (sizeof(struct pci_dev)/sizeof(unsigned))

static int acq32_init_module(void)
{
    if ( acq32_simulate ){
        int idev;

        for ( idev = 0; idev != 4; ++idev ){
            struct pci_dev* p_dev =
                KMALLOC( sizeof(struct pci_dev), GFP_KERNEL );
            int idead;
            struct Acq32Device* device;

            for ( idead = 0; idead < MAXDEAD; ++idead ){
                ((unsigned*)p_dev)[idead] = 0xdeadbeef;
            }
            device = acq32_create_device( p_dev );
            if ( !device ){
                return -ENOMEM;
            }
            allocateDmaBuffer( device );
            acq32_device_init( device );
        }
        S_acq32.ndevs = 4;
        return 0;
    }

    return pci_register_driver( &acq32_pci_driver );
}
#endif

static void iterate_coreDevInit(void)
{
	int idev;

	for ( idev = 0; idev != S_acq32.ndevs; ++idev ){
		coreDevInit( S_acq32.devices[idev] );
	}
}

int init_module(void)
{
	PDEBUG( "ACQ32 copyright (c) D-TACQ Solutions Ltd 2001\n"
		"ACQ200 copyright (c) D-TACQ Solutions Ltd 2003\n"
		"Release: %s\n", DTACQ_RELEASE_STRING );

	PDEBUGL(3)( "acq32busprot_rev %s\n", acq32_acq32busprot_rev );

	init_MUTEX( &S_acq32.ioread_fetch_mutex );
#ifdef ACQ200
	/* 
         * this assigns acq200 devices
         */
	acq200_init_module();
	/*
	 * locate all acq32's, then allocate device memory
	 */

	acq200_bb_init( acq32_big_buf_base, acq32_big_buf_len, S_acq32.ndevs );
#else
	acq32_init_module();
#endif

	if ( !acq32_simulate ){
		S_acq32.ndevs = S_acq32.idev;
	}
	PDEBUG( "D-TACQ num devices %d\n", S_acq32.ndevs );

	iterate_coreDevInit();

	init_fops();
	init_proc_entries();

	PDEBUG( "ACQ32: init_module() done\n" );
	return 0; /* succeed */
}

void cleanup_module(void)
{
    int ii;

#ifdef ACQ200
    acq200_cleanup_module();
#else
    if ( !acq32_simulate ){
        pci_unregister_driver( &acq32_pci_driver );
    }
#endif
    remove_proc_entries();

    /* use of idev is deliberate - include _all_ devices */

    for ( ii = 0; ii != S_acq32.ndevs; ii++){
  
        struct Acq32Device* device = S_acq32.devices[ii];

        unregister_chrdev( device->major, getDevName(device));

        acq200_bb_pool_delete(device);
	acq200_bb_free(&device->bigbuf.bb);
        acq32_device_readbuffer_delete( device );
        
        if ( !acq32_simulate && device->p_pci ){
            unmap_pci_memory( device );
        }
        if ( device->instrument_buf.buf ){
            KFREE( device->instrument_buf.buf );
        }

#ifndef ACQ200
        freeDmaBuffer( device );
#endif
	KFREE(device->p_md);
	KFREE(device);
    }

    /* and call the cleanup functions for friend devices */
}
