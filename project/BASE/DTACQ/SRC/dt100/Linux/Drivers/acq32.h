/*****************************************************************************
 *
 * File: acq32.h - Kernel data structs for acq32 device driver
 *
 * $RCSfile: acq32.h,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: acq32.h,v 1.120.4.15 2005/03/28 19:58:51 pgm Exp $
 * $Log: acq32.h,v $
 * Revision 1.120.4.15  2005/03/28 19:58:51  pgm
 * fixes acq196 channel mask
 *
 * Revision 1.120.4.14  2003/12/14 22:01:04  pgm
 * split acq32, acq200 drivers
 *
 * Revision 1.120.4.13  2003/10/26 21:19:47  pgm
 * dumdma emulation work - 1MS limit
 *
 * Revision 1.120.4.12  2003/10/25 19:02:00  pgm
 * acq200_fetchData (NFG)
 *
 * Revision 1.120.4.11  2003/10/19 11:43:25  pgm
 * works with dt100rc
 *
 * Revision 1.120.4.10  2003/09/16 11:03:20  pgm
 * compiles clean on 2.4.20 (RH9x)
 *
 * Revision 1.120.4.9  2003/09/09 20:50:19  pgm
 * *** empty log message ***
 *
 * Revision 1.120.4.8  2003/09/09 17:10:52  pgm
 * upload download WORKS
 *
 * Revision 1.120.4.7  2003/09/08 20:48:35  pgm
 * file upload almost works (but not quite)
 *
 * Revision 1.120.4.6  2003/09/08 12:51:30  pgm
 * i2o file download works
 *
 * Revision 1.120.4.5  2003/09/07 21:39:48  pgm
 * download data possible if no buffers recyled
 *
 * Revision 1.120.4.4  2003/09/07 14:38:23  pgm
 * stub wavegen intro
 *
 * Revision 1.120.4.3  2003/09/06 07:28:03  pgm
 * acq32 now works again after acq200 split
 *
 * Revision 1.120.4.2  2003/09/04 21:11:12  pgm
 * acq200 getFwrev works (sorta) x156
 *
 * Revision 1.120.4.1  2003/08/31 16:53:52  pgm
 * first acq200 testing
 *
 * Revision 1.120.2.1  2003/08/29 17:17:03  pgm
 * first stage acq200 support
 *
 * Revision 1.120  2002/11/03 21:08:23  pgm
 * updated simul from hols
 *
 * Revision 1.118  2002/08/29 18:36:25  pgm
 * COS device tools OK
 *
 * Revision 1.117  2002/08/26 13:09:59  pgm
 * impl status stream - compiles 2.4, needs tesst
 *
 * Revision 1.116  2002/08/23 20:00:32  pgm
 * arm wdt rools OK
 *
 * Revision 1.115  2002/08/20 10:07:42  pgm
 * clean globs, use mutex for IOREAD
 *
 * Revision 1.114  2002/08/09 13:52:53  pgm
 * ioread_fetch_mutex
 *
 * Revision 1.113  2002/08/04 12:07:11  pgm
 * boost opt to setChannelMask
 *
 * Revision 1.112  2002/07/19 18:39:37  pgm
 * mod DUMDMA to avoid message overrun
 *
 * Revision 1.111  2002/07/02 10:59:49  pgm
 * add mmap testing
 *
 * Revision 1.110  2002/06/12 20:20:19  pgm
 * fix master device. gotta work now
 *
 * Revision 1.109  2002/06/11 14:49:32  pgm
 * streaming extra prams
 *
 * Reision 1.108  2002/05/15 09:10:32  pgm
 * downstreambuf
 *
 * Revision 1.107  2002/04/09 12:27:58  pgm
 * better proto debug
 *
 * Revision 1.106  2002/03/28 18:57:29  pgm
 * status device works
 *
 * Revision 1.105  2002/03/28 08:42:42  pgm
 * fixes isr to sep ack, cos, add waitevent, doubletap good
 *
 * Revision 1.104  2002/03/25 09:28:49  pgm
 * checks cycle
 *
 * Revision 1.103  2002/03/12 12:12:40  pgm
 * clean format rools OK
 *
 * Revision 1.102  2002/03/08 19:22:00  pgm
 * bigger readbuffer
 *
 * Revision 1.101  2002/03/08 16:17:55  pgm
 * *** empty log message ***
 *
 * Revision 1.100  2002/03/07 13:32:26  pgm
 * double tap rools OK
 *
 * Revision 1.99  2002/03/07 12:25:19  pgm
 * acq32_double_tap
 *
 * Revision 1.98  2002/02/27 02:08:47  pgm
 * getSyncRoute
 *
 * Revision 1.97  2002/02/26 16:43:24  pgm
 * command debug stream
 *
 * Revision 1.96  2002/02/14 09:11:08  pgm
 * handa2:trigger_DIx on setModeTriggeredConinuous
 *
 * Revision 1.95  2002/02/10 18:15:25  pgm
 * FAST_MULTI opt
 *
 * Revision 1.94  2002/02/02 14:44:42  pgm
 * waitEvent (not complete), multimaster updates
 *
 * Revision 1.93  2002/01/06 12:14:52  pgm
 * data fetch now returns actual num samples available
 *
 * Revision 1.92  2002/01/02 12:14:15  pgm
 * back prot to 2.2.x works
 *
 * Revision 1.91  2001/12/21 21:20:09  pgm
 * fix acq32_GetHeartbeat() logic error - broke startup
 *
 * Revision 1.90  2001/12/21 16:08:59  pgm
 * *** empty log message ***
 *
 * Revision 1.89  2001/12/19 10:09:40  pgm
 * Builds on RH6.2 again
 *
 * Revision 1.88  2001/12/19 09:56:06  pgm
 * Linux 2.4 first cut runs, mapping dodgy
 *
 * Revision 1.87  2001/12/14 12:17:09  pgm
 * Fix CHANNEL coding (all channels coded to 0)
 *
 * Revision 1.86  2001/12/04 15:20:54  pgm
 * works for implicit self==slave
 *
 * Revision 1.85  2001/11/28 21:03:25  pgm
 * minor encode supports more than 4 boards
 *
 * Revision 1.84  2001/11/23 21:54:57  pgm
 * continuous stream speed demo done
 *
 * Revision 1.83  2001/10/21 09:08:40  pgm
 * selectThresholdDetector etc
 *
 * Revision 1.82  2001/10/20 20:42:30  pgm
 * selectThresholdDistributor opts
 *
 * Revision 1.81  2001/10/20 19:44:19  pgm
 * getDebug
 *
 * Revision 1.80  2001/10/20 15:52:11  pgm
 * clock redistribution opts included
 *
 * Revision 1.79  2001/10/06 22:21:53  pgm
 * raw, flash devs no longer use ints
 *
 * Revision 1.78  2001/09/03 18:25:20  pgm
 * channel threshold 1..N, friendly
 *
 * Revision 1.77  2001/09/03 13:47:54  pgm
 * thresh trigger works (above, below right wya round
 *
 * Revision 1.76  2001/08/19 16:59:00  pgm
 * add threshold commands (untested)
 *
 * Revision 1.75  2001/08/19 08:52:55  pgm
 * resetGUT opt
 *
 * Revision 1.74  2001/06/25 21:57:00  pgm
 * view commands added
 *
 * Revision 1.73  2001/06/11 09:02:02  pgm
 * setClock, clockNow hooked up
 *
 * Revision 1.72  2001/06/02 19:57:57  pgm
 * reserveAO
 *
 * Revision 1.71  2001/06/01 20:10:21  pgm
 * GUT - hooking up core, compiles, willit work
 *
 * Revision 1.70  2001/06/01 19:45:32  pgm
 * GUT WIP - hooking up core, compiles
 *
 * Revision 1.69  2001/05/27 18:56:49  pgm
 * *** empty log message ***
 *
 * Revision 1.68  2001/05/17 18:51:40  pgm
 * ACQ32_IOREAD_GETPHYSICAL implemented
 *
 * Revision 1.67  2001/05/15 17:56:09  pgm
 * AO/DO fungen implemented
 *
 * Revision 1.66  2001/05/05 21:04:40  pgm
 * try fix DUMDMA fault with tq_scheduler, not tq_immediate
 *
 * Revision 1.65  2001/05/05 19:44:45  pgm
 * stub write(), fix bigdump dep, fault tolerant DUMDMA
 *
 * Revision 1.64  2001/04/29 09:25:50  pgm
 * now supports >4 devices. whew
 *
 * Revision 1.63  2001/04/29 08:23:18  pgm
 * use one chrdev per board, prep for >4 boards
 *
 * Revision 1.62  2001/04/28 08:01:43  pgm
 * add irq_req_mutex - avoid over registering isr
 *
 * Revision 1.61  2001/04/27 19:32:43  pgm
 * use mutex to guard mbox access - ship it quick
 *
 * Revision 1.60  2001/04/27 13:45:44  pgm
 * maps ROM in SRAM as required as per strict PCI BIOS reqs
 *
 * Revision 1.59  2001/04/15 09:52:44  pgm
 * syncSetRoute() works
 *
 * Revision 1.58  2001/04/01 08:26:32  pgm
 * DUMDMA works - fix with post sync on acq32
 *
 * Revision 1.57  2001/03/31 20:58:23  pgm
 * DUMDMA works in slow mo
 *
 * Revision 1.56  2001/03/30 20:17:01  pgm
 * UDMA first cut working
 *
 * Revision 1.55  2001/03/28 21:18:29  pgm
 * i2o command handling for FETCH cmpiles - willit work??
 *
 * Revision 1.54  2001/03/25 19:19:04  pgm
 * first cut i2o command processing
 *
 * Revision 1.53  2001/03/25 17:28:40  pgm
 * i2o commands work
 *
 * Revision 1.52  2001/03/23 19:44:00  pgm
 * mmap() as kernel malloc to user mapping work
 *
 * Revision 1.51  2001/02/24 18:18:36  pgm
 * test device for lltest, also acqcmd
 *
 * Revision 1.50  2001/01/14 17:54:11  pgm
 * multi thread safemake
 *
 * Revision 1.49  2001/01/02 12:11:08  pgm
 * slight cleanup
 *
 * Revision 1.48  2000/11/12 15:13:43  pgm
 * read() from host memory buffer is perfect
 *
 * Revision 1.47  2000/11/07 22:41:22  pgm
 * read() access works correctly on pre-samples
 *
 * Revision 1.46  2000/10/28 21:18:59  pgm
 * getSetNumSamples
 *
 * Revision 1.45  2000/10/22 20:36:15  pgm
 * Get Internal Clock now returns actual
 *
 * Revision 1.44  2000/10/07 21:33:05  pgm
 * trigger continuous  etc commands impl
 *
 * Revision 1.43  2000/10/06 21:06:36  pgm
 * bigdump mmaps to .host device
 *
 * Revision 1.42  2000/10/01 21:38:34  pgm
 * bigdump, getFwrev features work
 *
 * Revision 1.41  2000/10/01 18:20:35  pgm
 * bigdump, getFwrev first cut
 *
 * Revision 1.40  2000/09/26 19:46:52  pgm
 * GATED_CONTINUOUS pre/post, lseek, spin
 *
 * Revision 1.39  2000/09/22 13:59:45  pgm
 * min reads per sec timing implemented
 *
 * Revision 1.38  2000/09/18 13:29:34  pgm
 * slave iterator from low to hi to get multi board mask right
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
 * Revision 1.34  2000/06/25 19:41:01  pgm
 * soft continuous and others
 *
 * Revision 1.33  2000/06/25 05:53:29  pgm
 * acq32 push works
 *
 * Revision 1.32  2000/06/24 10:57:46  pgm
 * i2o round trip works
 *
 * Revision 1.31  2000/05/28 10:31:12  pgm
 * streaming works passibly in simulator
 *
 * Revision 1.30  2000/05/20 11:03:47  pgm
 * Rev 6 commands work + sim sample tagging
 *
 * Revision 1.29  2000/05/19 21:15:58  pgm
 * ICD rev6 command handling parsers in
 *
 * Revision 1.28  2000/05/18 21:02:02  pgm
 * removed -lc from link (failed under RH6.2
 *
 * Revision 1.27  2000/01/08 22:07:50  pgm
 * get num channels now reflects multi master mask
 *
 * Revision 1.26  2000/01/08 20:38:17  pgm
 * interrupt handling (not default)
 *
 * Revision 1.25  2000/01/04 10:53:49  pgm
 * eliminate slow poll bottleneck
 *
 * Revision 1.24  1999/11/20 21:20:30  pgm
 * 991120 time for bed
 *
 * Revision 1.23  1999/11/13 23:03:40  pgm
 * format volts lines dos finally works!
 *
 * Revision 1.22  1999/11/13 20:31:37  pgm
 * error reporting in master, read funcs
 *
 * Revision 1.21  1999/11/12 22:15:20  pgm
 * lines and dos mode added
 *
 * Revision 1.20  1999/11/12 17:10:40  pgm
 * fix starange volts format bug; acqN.mN device
 *
 * Revision 1.19  1999/11/09 21:00:13  pgm
 * buffering fixed, works with any size buffer
 *
 * Revision 1.18  1999/11/07 09:54:55  pgm
 * fractional mv must not have a sign
 *
 * Revision 1.17  1999/11/07 09:35:06  pgm
 * data handling is a runner
 *
 * Revision 1.16  1999/11/03 21:49:55  pgm
 * stash for backup
 *
 * Revision 1.15  1999/11/02 21:35:53  pgm
 * bocking Arm is reliable - ship it
 *
 * Revision 1.14  1999/11/02 10:43:35  pgm
 * channel read works
 *
 * Revision 1.13  1999/11/01 17:31:49  pgm
 * first stab at data fetch (not complete)
 *
 * Revision 1.12  1999/10/27 22:07:02  pgm
 * acq32-drv.o replaces ebsa-drv
 *
 * Revision 1.11  1999/10/26 15:16:54  pgm
 * struct debug, mbox commands
 *
 * Revision 1.10  1999/10/20 19:57:16  pgm
 * mixed in ebsa driver funcs + new rom offset hakker
 *
 * Revision 1.9  1999/10/15 18:47:00  pgm
 * extclock, gate_hi prams in, GATED_CONTINUOUS
 *
 * Revision 1.8  1999/10/15 15:10:11  pgm
 * GATE input to SM workscvs status | grep Status
 *
 * Revision 1.7  1999/10/14 22:51:50  pgm
 * simul engine runs - SHIP IT QUICKcvs status | grep Status
 *
 * Revision 1.6  1999/10/14 21:10:28  pgm
 * master command handling works
 *
 * Revision 1.5  1999/10/12 21:24:20  pgm
 * master command handling top down connection OK
 *
 * Revision 1.4  1999/10/12 10:23:47  pgm
 * basic master command/readback struct WORKS
 *
 * Revision 1.3  1999/10/09 21:40:59  pgm
 * args handling in write taped
 *
 * Revision 1.2  1999/10/02 21:27:14  pgm
 * first pass compile OK. /proc/acq32 stuffed
 *
 * Revision 1.1  1999/10/02 19:33:46  pgm
 * lets go
 *
 *
\*****************************************************************************/

#ifndef _ACQ32_H_
#define _ACQ32_H_




/*
 * 2.6+ compatibility shim for the 2.4 task-queue API used throughout this
 * driver.  The original code uses tq_struct, INIT_TQUEUE, queue_task and
 * schedule_task; map these onto modern work_struct + schedule_work.  The
 * routine/data fields are kept so existing call-sites that test
 * "if (tq->routine)" or "tq->routine = 0" continue to behave (a NULL
 * routine becomes a no-op dispatch).
 */
#include <linux/workqueue.h>

struct tq_struct {
	struct work_struct work;
	struct list_head list;	/* unused — kept so legacy
				 * INIT_LIST_HEAD(&tq->list) call-sites compile */
	void (*routine)(void *);
	void *data;
};

static inline void __acq32_tq_dispatch(struct work_struct *w)
{
	struct tq_struct *t = container_of(w, struct tq_struct, work);
	if (t->routine)
		t->routine(t->data);
}

#define PREPARE_TQUEUE(_tq, _routine, _data) do {		\
		(_tq)->routine = (_routine);			\
		(_tq)->data    = (_data);			\
	} while (0)

#define INIT_TQUEUE(_tq, _routine, _data) do {			\
		INIT_WORK(&(_tq)->work, __acq32_tq_dispatch);	\
		PREPARE_TQUEUE((_tq), (_routine), (_data));	\
	} while (0)

#define queue_task(_tq, _q)   schedule_work(&(_tq)->work)
#define schedule_task(_tq)    schedule_work(&(_tq)->work)

#define WAIT_QUEUE wait_queue_head_t

/* MOD_*_USE_COUNT are gone in 2.6; module refcount is auto-managed via
 * file_operations->owner = THIS_MODULE.  Make the legacy macros no-ops
 * so existing call-sites continue to build. */
#define MOD_IN_USE 0
#define MOD_INC_USE_COUNT do {} while (0)
#define MOD_DEC_USE_COUNT do {} while (0)




/*
 * Macros to help debugging
 */

#include <linux/interrupt.h>	/* irqreturn_t, IRQ_HANDLED */
#include <linux/time.h>

//#include "printp.h"
//#define printk printp

#include "dt100api.h"

#include "acq32ioctl.h"

#include "acq32busprot.h"    // this is ++NFG - work out how to remove ...

#ifdef __KERNEL__



#define CSR( dev, byte_off )    ((dev)->csr.va+(byte_off)/4)

/*
 * Minors code as follows:
 *
 * d7:1        0x80 - selects MASTER
 * d6:2        0x60 - SPECIAL
 * d4:5        0x1f - Channel {0..31}
 *
 */

#define _MASTERBIT        0x80
#define _SLAVEBITS        0x7f

#define _MASTER(dev)    ((MINOR(dev)&_MASTERBIT)!=0)
#define _SPECIAL(dev)   (!_MASTER(dev)&&(MINOR(dev)&0x40)!=0)
#define _CHANNEL(dev)   ((MINOR(dev)&0xc0)==0)

/*
 * IF _CHANNEL, minor devices code for channel
 *
 * normal channel device returns data as a column for the channel
 * space for 0..63 channels
 */

#define IS_CHANNEL_DEV(dev)    _CHANNEL(dev)
 
#define CHANNEL(dev)    (_CHANNEL(dev)? (MINOR(dev)&0x3f): -1)

/*
 * IF SPECIAL, minor devices code for mastership, ROM or SPECIAL2
 */

#define IS_MASTER(dev)      (_MASTER(dev))
#define SLAVES(dev)         (_MASTER(dev)? (MINOR(dev)&0x7f): 0 )
#define IS_MASTER_ALL(dev)  (SLAVES(dev)==0x7f)

#define SPECIAL_FLAGS(dev)  (_SPECIAL(dev)? MINOR(dev)&0x3f: 0 )



#define IS_ROM(dev)     (_SPECIAL(dev) && SPECIAL_FLAGS(dev)==0)
#define ROWDEV(dev)     (_SPECIAL(dev) && SPECIAL_FLAGS(dev)==0x3f)
#define DEVRAW(dev) 	(_SPECIAL(dev) && SPECIAL_FLAGS(dev)==1)
#define DEVDMABUF(dev)  (_SPECIAL(dev) && SPECIAL_FLAGS(dev)==2)
#define BIGDUMPDEV(dev) (_SPECIAL(dev) && SPECIAL_FLAGS(dev)==3)
#define AOIDEV(dev)     (_SPECIAL(dev) && SPECIAL_FLAGS(dev)==4)
#define AOFDEV(dev)     (_SPECIAL(dev) && SPECIAL_FLAGS(dev)==5)
#define DOIDEV(dev)     (_SPECIAL(dev) && SPECIAL_FLAGS(dev)==6)
#define DOFDEV(dev)     (_SPECIAL(dev) && SPECIAL_FLAGS(dev)==7)

#define AOFRDEV(dev)    (_SPECIAL(dev) && SPECIAL_FLAGS(dev)==8)
#define DOFRDEV(dev)    (_SPECIAL(dev) && SPECIAL_FLAGS(dev)==9)
#define STATDEV(dev)    (_SPECIAL(dev) && SPECIAL_FLAGS(dev)==10)


#define TESTDEV(dev)    (_SPECIAL(dev) && SPECIAL_FLAGS(dev)==15)
#define TFTPDEV(dev)    (_SPECIAL(dev) && (SPECIAL_FLAGS(dev)&0x10))

#define HAS_SLAVE( n )  ((SLAVES(dev)&(1<<(n-1)) != 0)

#define ACQ32_MAX_CHANNELS 32 /* thinking ahead!! */
#define ACQ32_MAX_BOARDS    8

#define DEF_SAMPLE_MEM     (64*MB)

#define DEF_SAMPLE_TOTAL   (DEF_SAMPLE_MEM/2)
#define DEF_CHANNEL_MASK   (0xffffffff)

#define DEF_SAMPLES        1                 // take one sample by default

extern ssize_t acq32_master_read ( 
    struct file* filp, char* buf, size_t count, loff_t* posp);
extern ssize_t acq32_master_write(
    struct file* filp, const char* buf, size_t count, loff_t* posp );

extern ssize_t acq32_status_read ( 
    struct file* filp, char* buf, size_t count, loff_t* posp);
extern ssize_t acq32_status_write(
    struct file* filp, const char* buf, size_t count, loff_t* posp );


extern ssize_t sim_channel_read ( 
    struct file* filp, char* buf, size_t count, loff_t* posp );
extern ssize_t sim_rowdev_read ( 
    struct file* filp, char* buf, size_t count, loff_t* posp );
extern ssize_t acq32_data_write(
    struct file* filp, const char* buf, size_t count, loff_t* posp );

ssize_t acq32_channel_read ( 
    struct file* filp, char* buf, size_t count, loff_t* posp );

ssize_t acq32_row_read ( 		     
    struct file* filp, char* buf, size_t count, loff_t* posp );



struct Acq32ImagesDef {
    char model[16];
    char channels[16];
    char serialnum[16];
    char armfw[16];
    char lcafw[16];
    char caldate[16];
};

typedef struct ACQ32CHANNELFILEINFO {
    enum ChannelMode mode;
    char text_format[12];
}
Acq32ChannelFileInfo;



struct Acq32MasterDriver;

typedef struct INSTRUMENT_BUF {
    int id;
    char ids[100];
    struct timeval tv;
} 
InstrumentBuf;

#define READBUFFERLEN    (1<<12)

struct ReadBuffer {
    unsigned char* rbuffer;
    int ibput;                 /* buffer put and get pointers */
    int ibget;
};


struct MESSAGE;
  
struct ReturnCommandHandler {
	struct ReturnCommandHandler* next;
	struct ReturnCommandHandler* prev;
    
	void (*handler)( 
		struct ReturnCommandHandler* this, 
		struct MESSAGE* messsage );        

	struct Acq32Path* path;                        // active path
	short command_id;                              // command id
	void *clidata;                                 // private data for rch
	spinlock_t *lock;
};
   
#define NSYNCROUTES    6

struct GenericIntBuf {
    u32 buffer_mask;
    int iput;
    int iget;
    int iputback;             // is previous get pointer

	// diagnostics
    int iswrapped;
    int nwrapped;             // # times it has wrapped         
    int hitide;               // max detected samples in stream
    int qpresent;             // current state of the Q
    u32 buffers[0];
};
#define NSTREAMBUF (1<<7)
 //#define NSTREAMBUF (1<<9)     // ACQ32 doesn't have this many buffers
#define NSTATBUF   (1<<4)

/*
 * STATSBUF encoding {jiffies:24,err+stat:8}
 */
#define STATBUF_SET( jiffies, error, status ) \
    (((jiffies)&0x00ffffff)<<8 | ((error)!=0? 0x80: 0) | ((status)&0x7f))
#define STATBUF_GET_JIFFIES( sb ) ((sb)>>8)
#define STATBUF_GET_ERROR( sb )   (((sb)&0x80)!=0)
#define STATBUF_GET_STATUS(sb)    ((sb)&0x7f)


#define DEV_TYPE_ACQ32  0x0100
#define DEV_TYPE_ACQ16  0x0200
#define DEV_TYPE_ACQ196 0x1000
#define DEV_TYPE_ACQ164 0x1100
#define DEV_TYPE_WAV232 0x2000
#define DEV_TYPE_ACQ216 0x2100

/* Opaque structure private to acq200_bigbuf_alloc.c */
struct BigBufPool;

struct Acq32Device {
    int major;                   /* major # of related Linux chrdev */
	int dev_type;

    struct pci_dev* p_pci;

    struct pci_dev pci_stash;    /* hold copies of pci settings here */
    
    int heartbeat_detected;

    Acq32ChannelFileInfo cfi;


	char devname[32];

    struct IoMapping {
	u32* va;
	unsigned long pa;
	int len;
	char name[16];
    }
    csr, ram, rom;
    /*
     * Master read data is buffered here
     */


    struct ReadBuffer mrb;         /* master read buffer        */

    int nclients;                  /* # clients for this driver */
    int use_interrupts;            /* set TRUE if device should use ints */
    int in_exclusive_use;          /* inihbit further opens() if set */

    WAIT_QUEUE wq;         /* master_read blocks here */


    struct CHANNEL_INFO {
	unsigned nchannels;        // #channels available
	short vmax10;              // vmax (volts*10)
	short vmin10;              // vmin (volts*10)
    }
    input_channels,
	output_channels;

    unsigned sync_dest_masks[NSYNCROUTES];    
    /*
     * data structs for streaming
     */
    
    struct StreamBuf {
	struct GenericIntBuf def;
	u32 _buffers[NSTREAMBUF];
    }
    streambuf,
	downstreambuf;            // for MT_STREAM packets only

    struct StatBuf {
	struct GenericIntBuf def;
	u32 _buffers[NSTATBUF];
    }
    statusbuf;   // ISR stores statuses for consumption

    struct AppBuf {
	char* buf;
	size_t count_max;
	size_t count_actual;
    }
    appbuf;

    struct DmaBuf {
	    u32* va;
	    u32 pa;
	    size_t len;
    }
    dmabuf;

    int vmas;                        // count of open vma's

    struct BigBuf {
	    struct IoMapping bb;
	    struct BigBufPool* pool;
	    int contains_valid_data;
	    int testcount;           // for local file copy tests 
	    int direction_outgoing;
    }
    bigbuf;

    struct DriverPrivate {        /* should be accessed ONLY via driver p_md */
	short rom_is_enabled;
	short nchannels;
	unsigned total_sample_length;
	enum STATE state;
	unsigned channel_mask;
	enum MODE mode;
	short cycle;
	short error;
	u32 last_status;

	struct {
	    int pre;
	    int post;
	}
	nsamples_requested, nsamples_actual;
            
	int itrigger;                /* the trigger point */

	struct CLOCK {
	    int internal;          // 0=>EXT CLOCK, else INTCLOCK in Hz 
	    int doX;               // doX line if master
	    int diX;               // diX line if slave
	    int divisor;           // if EXT clock divided
	}
	clock;

	int sample_tagging;             // 1=> enabled

	long long global_sample_id;     
	int subframe_id;
	long long subframe_sample_id;   // id for this subframe

	struct tq_struct work_task;
	struct tq_struct isr_bh;
	
	struct ArmWdtStruct {
	    int busy;
	    struct tq_struct watchdog_task;
	    unsigned start_jiffies;
	    unsigned last_poll_jiffies;;
	} set_arm;
	
	  

	WAIT_QUEUE waitq;


#define MAX_COS_PATHS 4

	struct CosTask {
	    int task_has_been_created;
	    struct tq_struct task;   // kernel task runs all the time

	    struct file* files[MAX_COS_PATHS]; // path descr of each waiter
	    int enable;                    // index to files

	    int inhibit;                   // temp inhibit to ISR
	} cos;
	
	unsigned dio_dir;               // cache of DIO dir (0=>input)
	unsigned dio_outputs;           // value|dio_dir
	unsigned dio_inputs;

	unsigned streaming;             // stride+mean?0x80000000
	int streaming_enabled;

	int i2o_packets_incoming;
	int i2o_packets_returned;
	int i2o_packets_discarded;
	void* i2o_last_in;
		
	int i2o_packets_outgoing;       // # command packets sent
	short i2o_last_id_out;              // last id code
		
	    /* next 2 fields private to rch (yes, should be blind struct */
	    spinlock_t __rch_lock;
	    struct ReturnCommandHandler __first_handler;
	struct ReturnCommandHandler* first_rch;
	
	struct timeval tv_last_streaming_message;

	unsigned streaming_message_delta_usecs;

	unsigned aux_mfa;

	WAIT_QUEUE aux_waitq;
	int aux_task_waiting;
		
	struct semaphore mbox_mutex;    // only one thread at a time at mbox
	struct semaphore irq_req_mutex; // don't request irq multiple times

	struct semaphore ioread_fetch_mutex;    // for fetch phase
    }
    m_dpd;

    int (*volts2bits)( int normalised_volts );// worktodo - custom per brd type
    struct Acq32MasterDriver* p_md;

    struct {
	InstrumentBuf* buf;
	InstrumentBuf* cur;
    }
    instrument_buf;

/*
 * define some device virtual methods. So we can abstract
 * 1G/2G interface detail. viva OOP
 */
	int (*getImagesDef)(
		struct Acq32Device* device,struct Acq32ImagesDef* id);
	void (*set_mailbox)(struct Acq32Device* device, int mbx, u32 value );
	void (*get_mailbox)(struct Acq32Device* device, int mbx, u32 *value);
	irqreturn_t (*isr)(int irq, void* dev_id);
	int (*coreDevInit)(struct Acq32Device* device);
	void (*i2o_return_mfa)(struct Acq32Device* device, u32 mfa);
	void* (*i2o_mfa2va)(struct Acq32Device* device, u32 mfa);
	int (*fetchDataToLocalBuffer)(struct file *file, int channel,
				   short *buffer, int nsamples,
				   int start, int stride);
};

struct Acq32Device* acq32_get_device( dev_t i_rdev );
int acq32_device_exists( dev_t i_rdev );
int acq32_get_board( dev_t i_rdev );
struct Acq32Device* acq32_get_device_from_filp( struct file* filp );
struct Acq32Device* acq32_get_device_from_board( int board );
int acq32_get_board_from_device( struct Acq32Device* device );


extern void acq32_validate_bigbuf( 
    struct Acq32Device* device, unsigned set_valid );
extern int acq32_coreDevInit( struct Acq32Device* device );

extern int acq32_command_debug;

#define PRINTCMD( dir, text ) \
    if ( acq32_command_debug ) printk( KERN_DEBUG "%s%s\n", dir, text ) 

#define PRINTBP( dir, text ) \
    if ( acq32_busprot_debug ) printk( KERN_DEBUG "%s%s\n", dir, text )
    
#define ACQ32_PATH_READBUFFER_PRINTF( path, fmt...) \
{\
    char local[80];\
    sprintf( local, ##fmt );\
    PRINTCMD( "acq32:>", local );\
    acq32_path_readbuffer_put( path, local, strlen(local) ); \
}


int acq32_path_readbuffer_is_empty( struct Acq32Path* path );


int acq32_device_readbuffer_put( 
    struct Acq32Device* p_dev, char* cli_data, int nbytes );
// returns nbytes put on buffer

int acq32_device_readbuffer_get( 
    struct Acq32Device* p_dev, char* cli_data, int maxbytes );

void acq32_device_readbuffer_create( struct Acq32Device* p_dev );
void acq32_device_readbuffer_delete( struct Acq32Device* p_dev );


// returns nbytes pulled from buffer


extern struct Acq32Device* acq32_devices;


/*
 * each filp->private data points to a unique struct Acq32Path
 */

/*
 * channel mapping - each channel may be mapped into a
 * kernel space host memory buffer
 * Max channel length = 128MB/2ch = 64MB
 * Linux 2.2 Max Buffer length 128K - 512 buffers will do!
 */
 
/*
 * 2G increase to 256MB
 * of course, buffer size should increase as well ...
 */
#define MAX_BUFFERS_IN_CHANNEL     (0x10000000/0x20000)

typedef struct CHANNEL_MAPPING {
    struct vm_area_struct* vma;
    // device read buffer done
    char* buffers[MAX_BUFFERS_IN_CHANNEL];
}
ChannelMapping;	  
/*
 * allow this many mappings open at once on one path
 * remember, each channel gets its own path, so 1 would be good enough here ..
 */
#define MAX_CONCURRENT_CHANNEL_MAPPINGS    4

int acq32_copyToChannelMap(
	ChannelMapping* cm, 
	void *src,
	unsigned offset,
	unsigned length );
 


struct Acq32Path {
    dev_t minor;                 // customisation for this node
    struct Acq32Device* device;          // my device
    Acq32ChannelFileInfo* info;   // path uses this info buffer
    // which just might be this one ...
    Acq32ChannelFileInfo _info;   // custom format for this fd

    short is_blocking; 
    short uses_irq;               // not all paths use irq
                   

    int nsamples;

#define OFFSETSOFAR 20            // guess at current offset, on hi side

    struct PATH_BUFFER {
	long start_sample;        // pos cached at begin of buffer
	int iput;                 // put to data[iput]
	int iget;                 // get from data[iget]
	int max_data;             // length of buffer (chars)
	char* data;
	int is_pre_buffer;
    }
    buffer;

    struct ReadBuffer prb;        // path read buffer

    int flushed_device_read_buffer; // set tru on master open when 
           
    ChannelMapping* channel_maps[MAX_CONCURRENT_CHANNEL_MAPPINGS];

    struct ReturnCommandHandler returnCommandHandler;// limit to one per path
    // this could be extended??
    int return_samples;
    WAIT_QUEUE return_waitq;
    struct tq_struct defer;

    /*
     * for cos devices - 
     */
    int my_cos_index;

	void *subclass;            /* a subclass may store more info here */
    char scratch[2048];
};                    // Path descriptor structure

void acq32_readbuffer_create( struct ReadBuffer* rb );
void acq32_readbuffer_delete( struct ReadBuffer* rb );

int acq32_path_readbuffer_put( 
    struct Acq32Path* path, char* cli_data, int nbytes );
int acq32_path_readbuffer_get( 
    struct Acq32Path* path, char* cli_data, int maxbytes );
int acq32_path_readbuffer_stash( struct Acq32Path* path );

void acq32_path_readbuffer_create( struct Acq32Path* path );
void acq32_path_readbuffer_delete( struct Acq32Path* path );

    /*
     * PD    : gets PathDescriptor from filp
     * PDEV  : get Device from filp
     * PDRVR : get Driver from filp
     */

#define PD( filp )     ((struct Acq32Path*)filp->private_data)
#define PDEV(filp)     (PD(filp)->device)
#define PDRVR(filp)    (PDEV(filp)->p_md)

#define ACFI         (PD(filp)->info)
#define ACFI_MODE    (ACFI->mode)
#define ACFI_FORMAT  (ACFI->text_format)


// GUT prams
enum FX { 
    FX_NONE = 0, 
    FX_AI, 
    FX_AO, 
    FX_DO  
};

enum SUBJECT {
    NULL_SUBJECT = 0,
    PH_REQUEST = 0x10, PH_1, PH_2,
    EV_REQUEST = 0x20, EV_1, EV_2, EV_3    
};


struct EventConditionDef {
    int error;
    enum EVENT_CONDITION ec;
    union {
	int trigger_bit;    // EV_TRIGGER_RISING, EV_TRIGGER_FALLING
	struct {
	    short channel;    // EV_DATA_EXCEEDS, EV_DATA_BELOW
	    short below;
	    short above;
	} 
	th;
    }
    prams;
};

enum USERLED { UL_OFF, UL_ON, UL_FLASH };

enum StatusSelect {
    SS_FW_REV, SS_CONFIG, SS_CALINFO, SS_CAPTURESTATS, SS_DEBUG
};

enum ThresholdSelect { 
    THS_ALL = 99, 
    THS_ARM_DATA_SINGLE = -10,
    THS_ARM_DATA_MULTI,
    THS_ARM_EDGE_SINGLE,
    THS_ARM_EDGE_MULTI,
    THS_ARM_FAST_MULTI     
};

/*
 * class to wrap driver core
 */

struct Acq32MasterDriver {
#define THIS   struct Acq32Path* path
 
    enum STATE (*GetState)( THIS );
    int (*SetChannelMask)( THIS, char* smask, int boost );
    int (*GetMemoryDepth)( THIS, int channel );
    int (*SetMode)( 
	THIS, enum MODE mode, int samples, int pre, int trigger_DIx );
    int (*SetArmed)( THIS );
    int (*SetAbort)( THIS );
    int (*GetNumSamples)( THIS );
    int (*GetNumChannels)( THIS );
    int (*OnOpen)( THIS );
    int (*OnRelease)( THIS );

    int (*SetInternalClock)( THIS );
    int (*SetExternalClock)( THIS );
    int (*SetSampleTagging)( THIS );

    // signal conditioning methods 

    int (*SetDIO)( THIS );
    int (*GetDIO)( THIS );
    int (*SetChannel)( THIS, int ichan, int g1, int g2, int vex );
    int (*SetStreaming)( THIS, int npairs, int burst );

    int (*GetInfo)( THIS, enum StatusSelect selector );
    int (*DoBigdump)( THIS );
    int (*SetDistributor)( THIS, int distributor_id );
    int (*SetRoute)( THIS, int route_id );
    
    int (*SetSyncRoute)( THIS, int func, unsigned dest_mask );

    int (*SetCal)( THIS, int ical );
    int (*waitState)( THIS, int state );
    
    // GUT
 
    int (*SetPhase)( THIS, enum FX fx, enum SUBJECT ph, int nsamples );
    int (*GetPhase)( 
	THIS, enum FX fx, enum SUBJECT ph, enum PHASE_PROPERTY property );
    int (*SetEvent)( 
	THIS, 
	enum FX fx, 
	enum SUBJECT event, 
	struct EventConditionDef* ecd 
	);
    int (*GetEvent)( THIS, enum FX fx, enum SUBJECT event );
    int (*FireEvent)(THIS, enum FX fx, enum SUBJECT event );
    int (*SetClock)( THIS, enum FX fx, int clk_source );
    int (*ClockNow)( THIS );
    int (*ResetGut)( THIS );
    
    int (*ReserveAO)( THIS, int nreserve );
    int (*SetUser)( THIS, int user_led, enum USERLED mode );
    
    int (*SetThreshold)( 
	THIS, 
	enum ThresholdSelect channel, 
	short trigger_above, short trigger_below 
	);
	
    int (*GetAvailableChannels)( THIS );
    int (*GetRange)( THIS );
};

    
int acq32_WaitEvent( THIS, struct ACQ32_WAIT_DEF* wait_def );


void acq32_show_busy( int set );

extern struct Acq32MasterDriver* acq32_getDriver();
extern struct Acq32MasterDriver* acq32_getSimul();

void acq32_finish_with_engines( struct DriverPrivate* dp );
void acq32_restart_timer_task( struct DriverPrivate* dp );

int acq32_getDeviceCount();

// common timer restart func

/*
 * prams configured at load time
 */

extern int acq32_use_interrupts; //set to 1 to use interrupts
extern int acq32_simulate;   // set to 1 to enable simulation
extern int acq32_gate_hi;    // set to 1 for gate active high
extern int acq32_extclock;   // set to 0 for internal clock

extern int acq32_debug;      // 0 => no debugs
extern int acq32_command_debug; // 0=> nodebugs
extern int acq32_busprot_debug; // 0=> no debugs

extern int acq32_path_buffer_size;   // default path buffer size
extern int acq32_instrument_buf_len; // set length of inst buffer, if rqd

extern long acq32_big_buf_base;  // PGM:TEMP base addr of big streambuf
extern long acq32_big_buf_len;

extern int acq32_dumdma_to;        // DUMDMA timeout - crank up if debugging

extern int acq32_fill_vma;        // debug aid - fill vma on allocate
extern int acq32_max_channels;    // override to set max channels on all brds 

extern int acq32_double_tap;        // arm-abort-arm fills cache
extern int acq32_debounce_clk;      // enable clock debounce
extern int acq32_enable_emtrig_bit;
extern int acq32_leak_detect;

/*
 * timestamp - profiling tool - DO NOT CALL DIRECTLY,
 *
 * use TIMESTAMP macro (turns off efficiently)
 */
extern int
acq32_timestamp( struct Acq32Device* dev, int id, const char* fmt, ... );



#define TIMESTAMP( dev, typ, str, args... )\
    while(acq32_instrument_buf_len){\
        acq32_timestamp( dev, typ, str, ##args ); break; }

extern unsigned acq32_getDeltaTime( 
    struct timeval* tv_prev, struct timeval* tv_update );


static inline int printfn( int n, const char *fn )
{
	if ( acq32_debug > n ){
		printk( KERN_INFO "%s ", fn );
		return 1;
	}else{
		return 0;
	}
}

#define PDEBUGL( n ) if ( printfn( n, FN ) ) PDEBUG

extern void acq32_dump( char* fmt, void* dp );

#define PDUMPL( n, str, ptr ) \
    if ( acq32_debug>n) acq32_dump( str" "#ptr, ptr )

/*
 * for volt conversions we're forced into integer arithmetic
 * define mV10 as millivolts*10, to allow max resolution 0.1mV
 * ie the limit of resolution of a 16bit convertor
 */

extern int acq32_raw2mV10( unsigned short raw, char buf[] ); // returns mV*10

#define MV( mV10 )      ABS((mV10)/10)     // integer millivolts
#define MV10( mV10 )    ABS(((mV10)%10))     // fractional part - tenths of mV
#define MV_SIGN( mV10)  (SIGN(mV10)>0? '+': '-')

#define VV( mv10 )     ABS((mv10)/10000)
#define VFRAC(mv10)    ABX((mv10)%10000)






/*
 * slave iteration
 */


typedef struct {
    unsigned short slaves;
    unsigned short full_mask;
    unsigned iter_cursor;
    int done_self;
}
SlaveIterState;

struct Acq32Device* acq32_getNextSlave( 
    struct Acq32Path* path, SlaveIterState* state );




/*
 * stdlib standins
 */

long int strtol(const char *nptr, char **endptr, int base);

#define STRTOL( str ) strtol( str, 0, 0 )
#define ATOI( str )   (int)strtol( str, 0, 0 )


/*
 * Streambuf manip
 */

#define STREAM_MFA_INVALID 0xFFFFFFFFU

#ifdef PGMCOMOUT
static inline void streamDump( struct StreamBuf* stream_buf, char* caller ) {
    PDEBUGL(0)( "%10s p:%d g:%d\n", caller, stream_buf->iput, stream_buf->iget );
}
#else
#define streamDump( a, b )
#endif

static inline void genericIntBufInit( struct GenericIntBuf* buf, int nbuf ){
    buf->iput = buf->iget = buf->iputback = 0;
    buf->buffer_mask = nbuf - 1;
}
static inline int genericIntBufInc( struct GenericIntBuf* buf, int ii ){
    return (ii+1)&buf->buffer_mask;
}
static inline int genericIntBufIsFull( struct GenericIntBuf* buf ){
    return genericIntBufInc( buf, buf->iput ) == buf->iputback;
}

static inline int genericIntBufIsEmpty( struct GenericIntBuf* buf ){
    return buf->iget == buf->iput;
}
static inline int genericIntBufPut( struct GenericIntBuf* buf, u32 data ){
    if ( !genericIntBufIsFull( buf ) ){
	int iput;

	streamDump( buf, "streamPut()" );
	buf->buffers[buf->iput] = data;
	iput = genericIntBufInc( buf, buf->iput );
	if ( iput < buf->iput ){
	    buf->iswrapped = 1;
	    buf->nwrapped++;
	}
	buf->iput = iput;
	return 0;
    }else{
	streamDump( buf, "streamPut()E" );
	return -1;
    }
}
static inline u32 genericIntBufGet( struct GenericIntBuf* buf ){
    if ( !genericIntBufIsEmpty( buf ) ){
	u32 data;

	unsigned long flags;
	streamDump( buf, "streamGet()" );

	local_irq_save(flags);
	data = buf->buffers[buf->iget];
	buf->iputback = buf->iget;
	buf->iget = genericIntBufInc( buf, buf->iget );
	local_irq_restore(flags);

	return data;
    }else{
	streamDump( buf, "streamGet()E" );
	return STREAM_MFA_INVALID;
    }
}
static inline void genericIntBufPutBack( struct GenericIntBuf* buf )
// puts last buffer from streamGet() back on the Q
{
    buf->iget = buf->iputback;
}
static inline int genericIntBufNumEntries( struct GenericIntBuf* buf )
// returns # entries in buffer NB: unguarded!
{
    int entries = 0;
    int iget = buf->iget;

    while ( iget != buf->iput ){
	iget = genericIntBufInc( buf, iget );
	++entries;
    }

    return entries;
}

static inline void streamBufInit( struct StreamBuf* stream_buf ){
    genericIntBufInit( &stream_buf->def, NSTREAMBUF );
}
static inline int streamPut( struct StreamBuf* stream_buf, u32 data ){
    return genericIntBufPut( &stream_buf->def, data );
}
static inline u32 streamGet( struct StreamBuf* stream_buf ){
    return genericIntBufGet( &stream_buf->def );
}
static inline void streamPutBack( struct StreamBuf* stream_buf )
// puts last buffer from streamGet() back on the Q
{
    genericIntBufPutBack( &stream_buf->def );
}
static inline int streamNumEntries( struct StreamBuf* stream_buf )
    // returns # entries in buffer NB: expensive!
{
    return genericIntBufNumEntries( &stream_buf->def );
}

static inline int statbufPut( struct StatBuf* stat_buf, u32 data ) {
    return genericIntBufPut( &stat_buf->def, data );
}
static inline u32 statbufGet( struct StatBuf* stat_buf ){
    return genericIntBufGet( &stat_buf->def );
}
static inline int statbufIsEmpty( struct StatBuf* stat_buf ){
    return genericIntBufIsEmpty( &stat_buf->def );
}

static inline void statBufInit( struct StatBuf* buf ){
    genericIntBufInit( &buf->def, NSTATBUF );
}
static inline void appbufInit( struct AppBuf* app_buf ) {
    app_buf->count_max = app_buf->count_actual = 0;
}

extern int acq32_waitInt( struct Acq32Device* device, int timeout );

#endif                      /* __KERNEL__ */


#define SLOW_ACQ 250000    /* slow if gap between messages is SLOW_ACQ uSecs */

#define PAGE_ORDER 5         // 32 pages is max size (from RUBINI)

#define DMABUF_SIZE    ((1U<<PAGE_ORDER)*PAGE_SIZE)


extern int acq32_setArmSpin( struct Acq32Device* device );

struct MESSAGE;
extern int acq32_sendI2O( struct Acq32Path* path, struct MESSAGE* arg );

struct READ_LOCALBUF_DESCR;
extern int acq32_fetchDataToLocalBuffer( 
    struct file* file,
    int channel, 
    short* buffer,
    int nsamples,
    int istart,
    int istride
    );

#define ALLCHANNELS    -1    // set channel to this for rowdev


extern int acq32_getBufferPhysicalAddr(
    struct Acq32Path* path,
    int channel,
    void* buffer
    );

#define DEBUG_MALLOC
#ifdef DEBUG_MALLOC

extern void* dbg_kmalloc( 
    size_t size, int flags, const char* file, int line 
    );
extern void __dbg_kfree( 
    const void* addr, const char* file, int line 
    );
extern unsigned long dbg__get_free_pages( 
    int prio, int order, const char* file, int line 
    );
extern void dbg_free_pages(
    unsigned long pages, int order, const char* file, int line
    );


#define KMALLOC( size, flags ) dbg_kmalloc( size, flags, __FILE__, __LINE__ )
#define KFREE( addr )          __dbg_kfree( addr, __FILE__, __LINE__ )

#define __GET_FREE_PAGES( prio, order ) \
    dbg__get_free_pages( prio, order, __FILE__, __LINE__ )
#define FREE_PAGES( pages, order ) \
    dbg_free_pages( pages, order, __FILE__, __LINE__ )
#else
#define KMALLOC( size,flags )  kmalloc( size, flags )
#define KFREE( addr )          kfree( addr )
#define __GET_FREE_PAGES( prio, order ) __get_free_pages( prio, order )
#define FREE_PAGES( pages, order ) free_pages( pages, order )
#endif

int acq32_devSendCommand( struct Acq32Device* device, unsigned command );
int
acq32_devSendCommandA3A4(
    struct Acq32Device* device, 
    unsigned command,
    int nargs,                   // number of args's to send
    unsigned *a3,                // array of a3's to send
    unsigned *a4
    );


extern int acq32_release( struct inode* inode, struct file* filp );
extern int acq32_getHeartbeat( struct Acq32Device* device );
extern int acq32_getCycle( struct Acq32Device* device );
int acq32_report_version( char *buf, int max_len );

extern int acq32_intsEnable( unsigned irqs );
extern int acq32_intsDisable( unsigned irqs );
extern int acq32_swallowKmem( unsigned orders );

extern int acq32_globalIoreadFetchMutexDown();
extern void acq32_globalIoreadFetchMutexUp();

extern void acq32_doGetStateWork( 
    struct file* filp, unsigned status, unsigned last_status );

/*
 * need to use a mutex to protext mbox transactions
 */
#ifdef PGMCOMOUT 
#define MUTEX_DOWN( pmutex ) do {\
    PDEBUGL(3)( FN " mbox_mutex count:%d\n", (int)device->m_dpd.mbox_mutex.count );\
    PDEBUGL(3)(FN" enter MUTEX\n");down( pmutex );} while(0)
#define MUTEX_UP( pmutex )   do {PDEBUGL(3)(FN" leave MUTEX\n");  up( pmutex );} while(0)
#else
#define MUTEX_DOWN( pmutex ) \
    if ( down_interruptible( pmutex ) ) return -ERESTARTSYS
#define MUTEX_UP( pmutex )   up( pmutex )
#endif



#define APR_PRINTF ACQ32_PATH_READBUFFER_PRINTF
#define V10( vv ) ((int)((vv)*10))


enum STATE
acq32_devGetStatus( struct Acq32Device* device, enum MODE *mode );

void acq200_bb_init( long base, long length, int ndevices );
int acq200_bb_alloc(struct IoMapping *map, long length, char name[] );
int acq200_bb_free(struct IoMapping* map);

int acq32_genericCoreDevInit( struct Acq32Device* device );

void acq32_incoming_i2o_isr( struct Acq32Device* device, u32 mfa );

#define ACQ200_PCIWINSZ  0x01000000
#define ACQ200_PCIWINMSK (ACQ200_PCIWINSZ-1)


/*
 * acq32_specific.c
 */
ssize_t linear_buffer_read( 
    struct file *filp,
    char* buf,
    size_t count,
    char* pkmem,
    int kmax
	);

void set_rom_word_A01( struct Acq32Device* dev, unsigned value );
int acq32_userTargetAccessRead16( 
    struct inode* inode, 
    struct Acq32Device* dev, 
    struct ACQ32_RW_DEF* caller_arg
	);
int acq32_userTargetAccessWrite16( 
    struct inode* inode, 
    struct Acq32Device* dev, 
    struct ACQ32_RW_DEF* caller_arg
	);
ssize_t 
acq32_read ( struct file* filp, char* buf, size_t count, loff_t* posp );
int acq32_mmap_rom( struct file* filp, struct vm_area_struct* vma );
int acq32_mmap( struct file* filp, struct vm_area_struct* vma );
ssize_t 
acq32_rom_read ( 
    struct file* filp, char* buf, size_t count, loff_t* posp 
	);

int acq32_userTargetAccessRead32( 
    struct inode* inode, 
    struct Acq32Device* dev, 
    struct ACQ32_RW_DEF* caller_arg 
	);
int acq32_userTargetAccessWrite32( 
    struct inode* inode,
    struct Acq32Device* dev, 
    struct ACQ32_RW_DEF* caller_arg 
	);

extern struct Acq32Device *acq32_create_device(struct pci_dev *p_dev);
extern void acq32_doorbell_isr( struct Acq32Device* device, u32 doorbell );

#define ACQ32_BB_POOL_SIZE 0x100000
#define ACQ32_BB_POOL_ERROR 0xffffffff

void acq200_bb_pool_create(struct Acq32Device *device);
void acq200_bb_pool_delete(struct Acq32Device *device);

struct IoMapping *acq200_bb_pool_alloc(
	struct Acq32Device *device, 
	struct ReturnCommandHandler** rch, int direction);

void acq200_bb_pool_free(struct Acq32Device *device, struct IoMapping *map);

extern struct Acq32Path* 
acq32_makePathDescriptor(dev_t minor, struct Acq32Device *device);
void acq32_freePathDescriptor( struct Acq32Path* path );

struct DevGlob *acq200_get_device( dev_t i_rdev );


/*
 * return command handler Q
 */



#define RCH    struct ReturnCommandHandler
 
static inline void rchInsert( 
	RCH* first, RCH* new_entry
	)
{
	new_entry->lock = first->lock;

//	info( "new %p lock %p LOCK", new_entry, new_entry->lock );
        spin_lock(new_entry->lock);

	new_entry->prev = first;
	new_entry->next = first->next;
	first->next->prev = new_entry;
	first->next = new_entry;        

	spin_unlock(new_entry->lock);
//	info( "new %p lock %p unlock", new_entry, new_entry->lock );
}

static inline void rchAppend( 
	RCH* first, RCH* new_entry
	)
{
	new_entry->lock = first->lock;

//	info( "new %p lock %p LOCK", new_entry, new_entry->lock );
        spin_lock(new_entry->lock);

	new_entry->prev = first->prev;
	new_entry->next = first;
	first->prev->next = new_entry;
	first->prev = new_entry;        

	spin_unlock(new_entry->lock);
//	info( "new %p lock %p unlock", new_entry, new_entry->lock );
}

static inline void rchRemove( RCH* rem )
{
//	info( "rem %p lock %p LOCK", rem, rem->lock );
	spin_lock(rem->lock);

	rem->prev->next = rem->next;
	rem->next->prev = rem->prev;

	spin_unlock(rem->lock);
//	info( "rem %p lock %p unlock", rem, rem->lock );
}
    
void rchInit( struct Acq32Device* device );


int acq32_sendI2O_kbuf( struct Acq32Path* path, Message* message );

int acq32_request_irq( struct Acq32Path* path );
int acq32_free_irq( struct Acq32Path* path );


int acq32_devGetChannelMask(struct Acq32Device* device, unsigned* mask );
int acq32_devSetChannelMask(
	struct Acq32Device* device, unsigned mask, int boost);
#ifndef FN
#define FN __FUNCTION__
#endif
#endif            /* _ACQ32_H_ */


