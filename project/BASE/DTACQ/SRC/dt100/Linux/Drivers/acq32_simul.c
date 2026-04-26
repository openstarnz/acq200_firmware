/*****************************************************************************
 *
 * File: acq32_simul.c - simulation core for acq32 driver
 *
 * $RCSfile: acq32_simul.c,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:

WARNING: this functionality has rotted - don't expect it to work

 *
 * $Id: acq32_simul.c,v 1.39.4.1 2004/10/21 09:05:26 pgm Exp $
 * $Log: acq32_simul.c,v $
 * Revision 1.39.4.1  2004/10/21 09:05:26  pgm
 * correct warnings
 *
 * Revision 1.39  2002/11/03 21:08:23  pgm
 * updated simul from hols
 *
 * Revision 1.38  2002/08/04 12:07:11  pgm
 * boost opt to setChannelMask
 *
 * Revision 1.37  2002/03/27 15:48:39  pgm
 * clean up all handling
 *
 * Revision 1.36  2002/03/12 12:12:40  pgm
 * clean format rools OK
 *
 * Revision 1.35  2002/03/08 19:22:00  pgm
 * bigger readbuffer
 *
 * Revision 1.34  2002/02/14 09:11:09  pgm
 * handa2:trigger_DIx on setModeTriggeredConinuous
 *
 * Revision 1.33  2001/12/19 09:56:07  pgm
 * Linux 2.4 first cut runs, mapping dodgy
 *
 * Revision 1.32  2001/10/20 15:52:11  pgm
 * clock redistribution opts included
 *
 * Revision 1.31  2001/06/25 21:57:01  pgm
 * view commands added
 *
 * Revision 1.30  2001/03/28 21:18:30  pgm
 * i2o command handling for FETCH cmpiles - willit work??
 *
 * Revision 1.29  2001/03/23 19:44:00  pgm
 * mmap() as kernel malloc to user mapping work
 *
 * Revision 1.27  2001/01/02 12:11:08  pgm
 * slight cleanup
 *
 * Revision 1.26  2000/10/28 21:18:59  pgm
 * getSetNumSamples
 *
 * Revision 1.25  2000/10/07 21:33:05  pgm
 * trigger continuous  etc commands impl
 *
 * Revision 1.24  2000/09/26 19:46:52  pgm
 * GATED_CONTINUOUS pre/post, lseek, spin
 *
 * Revision 1.23  2000/09/22 13:59:45  pgm
 * min reads per sec timing implemented
 *
 * Revision 1.22  2000/07/01 06:31:31  pgm
 * SOFT_CONTINUOUS is valid mode, streams withno bh
 *
 * Revision 1.21  2000/05/28 10:31:13  pgm
 * streaming works passibly in simulator
 *
 * Revision 1.20  2000/05/20 11:03:48  pgm
 * Rev 6 commands work + sim sample tagging
 *
 * Revision 1.19  2000/05/19 21:15:58  pgm
 * ICD rev6 command handling parsers in
 *
 * Revision 1.18  2000/01/08 22:07:50  pgm
 * get num channels now reflects multi master mask
 *
 * Revision 1.17  2000/01/08 20:38:17  pgm
 * interrupt handling (not default)
 *
 * Revision 1.16  1999/11/12 22:15:20  pgm
 * lines and dos mode added
 *
 * Revision 1.15  1999/11/12 17:53:14  pgm
 * remove numeric overflow from volts calc
 *
 * Revision 1.14  1999/11/12 17:10:40  pgm
 * fix starange volts format bug; acqN.mN device
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
 * Revision 1.10  1999/11/01 17:31:50  pgm
 * first stab at data fetch (not complete)
 *
 * Revision 1.9  1999/10/26 15:16:54  pgm
 * struct debug, mbox commands
 *
 * Revision 1.8  1999/10/15 21:06:17  pgm
 * clock detect, active hi gate work
 *
 * Revision 1.7  1999/10/15 18:47:00  pgm
 * extclock, gate_hi prams in, GATED_CONTINUOUS
 *
 * Revision 1.6  1999/10/15 15:10:11  pgm
 * GATE input to SM workscvs status | grep Status
 *
 * Revision 1.5  1999/10/14 22:51:50  pgm
 * simul engine runs - SHIP IT QUICKcvs status | grep Status
 *
 * Revision 1.4  1999/10/14 21:10:28  pgm
 * master command handling works
 *
 * Revision 1.3  1999/10/12 21:24:20  pgm
 * master command handling top down connection OK
 *
 * Revision 1.2  1999/10/11 20:21:02  pgm
 * basic command handling and data format OK
 *
 * Revision 1.1  1999/10/09 21:40:59  pgm
 * args handling in write taped
 *
 *
\*****************************************************************************/

#define EBSA_DEBUG

#include "local.h"

#include <linux/module.h>

#include <linux/kernel.h> /* printk() */
#include <linux/fs.h>     /* everything... */
#include <linux/errno.h>  /* error codes */
#include <linux/types.h>  /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>        /* O_ACCMODE */
#include <linux/pci.h>
#include <linux/sched.h>        /* schedule() */

#include <asm/io.h>       /* ioremap()         */
#include <asm/uaccess.h>  /* VERIFY_READ|WRITE */
#include <asm/system.h>   /* cli(), *_flags */
#include <asm/segment.h>  /* memcpy and such */


#include "acq32.h"        /* local definitions */
#include "acq32ioctl.h"   /* APP interface defs */

#include "platform.h"


#define MAXCHAN 128
#define CHANID  0x8000
#define MAXCODE 0x7fff


#define DATA( channel, pos ) \
   (unsigned)(((pos)%MAXCHAN)==(channel)? CHANID: \
                    ((channel)>=0x10? (pos)/0x10: (pos))%MAXCODE)


/*
 * generate saample tags ... should be good for non-simul too
 */
#define SUBFRAME_MASK 0x3f
#define NSUBFRAMES    0x40
#define NID_BITS      0x30

#define nX_bit 15
#define T_bit  14
#define s0_bit  8

unsigned short acq32_generate_sample_tag( 
    struct Acq32Device* device,
    unsigned short dio,					      
    unsigned short trigger,
    unsigned short extra
    )
    /*
      * sample tag structure (ICD rev 6 )
      *
      *       d1111110000000000
      *        5432109876543210
      *        nTssssssdddddddd
      *        XT54321076543210
      *
      *  where nX { X 0..47 } is the SAMPLE NUMBER for subframe 0
      *  T                    Trigger
      *  s{5..0}              is the subframe number
      *  d{7..0}              DIO state (D0..7) on even subframe,
      *                                 (D15..8) on odd subframe,
      *  nX { X 48..63 } are extra bits (not used)
      */
{
    int iline = device->m_dpd.subframe_id;
    unsigned short the_tag;

    switch( iline ){
    case 0:
        the_tag = 0xfe;            // start of frame marker (not spec'd)
	device->m_dpd.subframe_sample_id = device->m_dpd.global_sample_id;
	break;
    case 1:
        the_tag = 0xed;
	break;
    default:
        the_tag = (iline&0x1)? dio>>8: dio&0xff;     
    }

    the_tag |= iline << s0_bit;
    the_tag |= trigger << T_bit;
    
    if ( iline < NID_BITS ){
	unsigned nX = (device->m_dpd.subframe_sample_id >> iline)&0x1;
	the_tag |= nX << nX_bit;
    }else{
	unsigned xX = (extra >> (iline-NID_BITS))&0x1;
	the_tag |= xX << nX_bit;
    }

    device->m_dpd.subframe_id = (++device->m_dpd.subframe_id)&SUBFRAME_MASK;
    return the_tag;
}


int acq32_raw2mV10( unsigned short raw, char buf[] )
{
#define VFS     10                // full scale volts
#define KMV10   10000             // converts volts to mV10's

    short sraw = raw;    
    long long int mv_huge = sraw;     // let's see you overflow now, sucker
    int mv;

    mv_huge *= KMV10*VFS;
    mv_huge /= 0x8000U;

    mv = mv_huge;
    
    if ( buf ){
        sprintf( buf, "%c%d.%d", MV_SIGN( mv ), MV( mv ), MV10( mv ) );
    }
    return mv;
}

ssize_t sim_channel_read ( 
    struct file* filp, char* buf, size_t count, loff_t* posp 
    )
{
    int channel = CHANNEL( PD(filp)->minor );
    size_t tcount = 0;
    int NSAMPLES = PDRVR(filp)->GetNumSamples( PD(filp) );

    unsigned long f_pos = (unsigned long)filp->f_pos;

    if ( f_pos > NSAMPLES ){
        return 0;
    }

/*
 * formatting ... assume single ASCII format at present
 */

    for ( ; f_pos < NSAMPLES; ++f_pos ){
        union {
            unsigned short bin;
            char aline[20]; 
        }
	kdata;
        int add_chars;
        unsigned short data = DATA( channel, f_pos );
 
 	switch( ACFI_MODE&CM_EXCLUSIVE_OPTS ){
        case CM_BINARY:
            kdata.bin = data;
            add_chars = sizeof(unsigned short);
            break;            
        case CM_VOLTS:
	{
	    char buf[10];
	    acq32_raw2mV10( data, buf );

	    add_chars = sprintf( kdata.aline, ACFI_FORMAT, buf );
	    strcat( kdata.aline+add_chars, "\n" );
	    add_chars += strlen( "\n" );
	}
	break;
        default:
	    add_chars = sprintf( kdata.aline, ACFI_FORMAT, data );
            strcat( kdata.aline+add_chars, "\n" );
            add_chars += strlen( "\n" );
        }

        if ( tcount+add_chars <= count ){
            copy_to_user( buf+tcount, kdata.aline, add_chars );
            tcount += add_chars;
        }else{
            break;
        }
    }

    filp->f_pos = f_pos;
    return tcount;
}

ssize_t sim_streaming_rowdev_read (
    struct file* filp, char* buf, size_t count, loff_t* posp 
    )
/*
 * special entry for the streaming case. 
 * assume that client _is_ keeping up with data generation (BAD??)
 */  
{
    struct DriverPrivate* dp = & PDEV(filp)->m_dpd;
    unsigned mode = ACFI_MODE&CM_EXCLUSIVE_OPTS;
    unsigned tagging = mode==CM_BINARY && PDEV(filp)->m_dpd.sample_tagging;

    unsigned long f_pos = (unsigned long)(filp->f_pos);
    unsigned stride = dp->streaming&0xffff;
    size_t tcount = 0;

    // mean NOT catered for

    // first, always sleep

    volatile long samples_now;
    long samples_available;

    unsigned short kdata_bin[ACQ32_MAX_CHANNELS+1];

    while ( f_pos >= (samples_now = dp->nsamples_actual.pre) ){
        int rv;

        if ( !MOD_IN_USE ){
	    PDEBUGL(1)( "oops - module not in use, bale out\n" );
            return 0;
        }

        init_waitqueue_head( &dp->waitq );
	rv = interruptible_sleep_on_timeout( &dp->waitq, 100 );

	if ( rv == 0 ){
	    PDEBUGL(4)( "streaming_rowdev_read() ... timeout\n" );
	}else{
	    PDEBUGL(4)( "streaming_rowdev_read() ... have been woken\n" );
	    if ( samples_now == dp->nsamples_actual.pre ){
		PDEBUGL(4)( "streaming .. no progress, return\n" );
		return 0;
	    }
	}
    }

    samples_available = samples_now;

    PDEBUGL(3)( 
	"streaming_rowdev_read() pos:%ld stride:%d count:%d"
	"available:%ld\n", 
	f_pos, stride, count, 
	samples_available );


    for ( ; f_pos < samples_available; f_pos += stride ){

        int channel;
        int iactive_channel = 0;
        int active_channel_mask = 0x1;
        int add_chars = 0;

	PDEV(filp)->m_dpd.global_sample_id += stride;  // this one always increments
	if ( tagging ){
	    unsigned short tag =
		acq32_generate_sample_tag( PDEV(filp), f_pos&0xffff, 1, 0xdead );

	    kdata_bin[iactive_channel++] = tag;

	    PDEBUGL(2)( "Tag is 0x%04x\n", tag );
	    add_chars += sizeof(unsigned short);
	}    

	for ( channel = 0; channel != ACQ32_MAX_CHANNELS; 
	      ++channel, active_channel_mask <<= 1 ){

	    if ( (PDEV(filp)->m_dpd.channel_mask&active_channel_mask) == 0 ){
		continue;
	    }else{

		unsigned short data = DATA( channel, f_pos );

		switch( mode ){
		case CM_BINARY:
		    kdata_bin[iactive_channel++] = data;
		    add_chars += sizeof(unsigned short);
		    break; 
		default:
		    PDEBUGL(1)( "CM_BINARY is the only supported mode\n" );
		    return -1;
		}
	    }
	}


        ASSERT( add_chars <= sizeof(kdata_bin) );

	PDEBUGL(4)( "streaming_rowdev_read: copy_to_user %p %p %d\n",
		    buf+tcount, kdata_bin, add_chars );

        if ( tcount+add_chars <= count ){
            copy_to_user( buf+tcount, kdata_bin, add_chars );
            tcount += add_chars;
        }else{
            break;
        }
    }

    filp->f_pos = f_pos;

    PDEBUGL(3)( "streaming_rowdev_read() returning %d\n", tcount );

    return tcount;
}
ssize_t sim_rowdev_read ( 
    struct file* filp, char* buf, size_t count, loff_t* posp 
    )
{
    //    struct inode* inode = filp->f_dentry->d_inode;
    size_t tcount = 0;
    int NSAMPLES = PDRVR(filp)->GetNumSamples( PD(filp) );

    unsigned long f_pos = (unsigned long)(filp->f_pos);
    unsigned mode = ACFI_MODE&CM_EXCLUSIVE_OPTS;
    unsigned tagging = mode==CM_BINARY && PDEV(filp)->m_dpd.sample_tagging;

    if ( PDEV(filp)->m_dpd.streaming != 0 ){
        return sim_streaming_rowdev_read( filp, buf, count, posp );
    }

    if ( f_pos > NSAMPLES ){
        return 0;
    }

/*
 * formatting ... assume single ASCII format at present
 */

    PDEBUGL(3)( "sim_rowdev_read pos:%ld count:%d NSAMPLES %d\n", 
		f_pos, count, NSAMPLES );



    for ( ; f_pos < NSAMPLES; ++f_pos ){
        union {
            unsigned short bin[ACQ32_MAX_CHANNELS+1];
            char aline[ACQ32_MAX_CHANNELS*32];
        }
	kdata;
	int channel;
	int iactive_channel = 0;
	int active_channel_mask = 0x1;
	int add_chars = 0;

	PDEV(filp)->m_dpd.global_sample_id++;   // this one always increments

	if ( tagging ){
	    unsigned short tag =
		acq32_generate_sample_tag( PDEV(filp), f_pos&0xffff, 1, 0xdead );

	    kdata.bin[iactive_channel++] = tag;

	    PDEBUGL(4)( "Tag is 0x%04x\n", tag );
	    add_chars += sizeof(unsigned short);
	}else{
	    PDEBUGL(4)( "NO Tagging\n" );
	}

        for ( channel = 0; channel != ACQ32_MAX_CHANNELS; 
	      ++channel, active_channel_mask <<= 1 ){

	    if ( (PDEV(filp)->m_dpd.channel_mask&active_channel_mask) == 0 ){
		continue;
	    }else{

		unsigned short data = DATA( channel, f_pos );

		switch( mode ){
		case CM_BINARY:
		    kdata.bin[iactive_channel++] = data;
		    add_chars += sizeof(unsigned short);
		    break;            
		case CM_VOLTS:
		{
		    char buf[10];
		    acq32_raw2mV10( data, buf );

		    add_chars += sprintf( 
			kdata.aline+add_chars, 
			ACFI_FORMAT, 
			buf 
			); 
		    strcat( kdata.aline+add_chars, "," );
		    add_chars += strlen( "," );
		}
		kdata.aline[add_chars++] = '\n';
		break;
		default:
		    add_chars += sprintf( 
			kdata.aline+add_chars, 
			ACFI_FORMAT, data );
		    strcpy( kdata.aline+add_chars, "," );
		    add_chars += strlen( "," );
		    kdata.aline[add_chars++] = '\n';
		}
	    }
        }


        ASSERT( add_chars < sizeof(kdata) );

        if ( tcount+add_chars <= count ){
            copy_to_user( buf+tcount, kdata.aline, add_chars );
            tcount += add_chars;
        }else{
            break;
        }
    }

    PDEBUGL(3)( "sim_rowdev_read pos:%ld count:%d end\n", f_pos, tcount );

    filp->f_pos = f_pos;
    return tcount;
}

static void init_capture( struct Acq32Path* path )
{
    path->device->m_dpd.itrigger = 
	path->device->m_dpd.nsamples_actual.post = 
	path->device->m_dpd.nsamples_actual.pre  = 0;

    path->device->m_dpd.global_sample_id = 0ULL;
    path->device->m_dpd.subframe_id = 0;
}

static enum STATE sim_GetState( struct Acq32Path* path )
{
    return path->device->m_dpd.state;
}

static int sim_SetMode( 
    struct Acq32Path* path, enum MODE mode, int samples, int pre, int dix )
{
    ASSERT( mode==M_GATED_TRANSIENT || 
            mode==M_GATED_CONTINUOUS ||
            mode==M_SOFT_TRANSIENT        );

    path->device->m_dpd.mode = mode;
    path->device->m_dpd.nsamples_requested.post = samples;
    path->device->m_dpd.nsamples_requested.pre = pre;

    init_capture( path );
    return 0;
}



#define PORTA  0x0378
#define PP_DATA   (PORTA)
#define PP_STATUS (PORTA+1)


#define GATE_HI     ((inb_p(PP_STATUS)&0X20)!=0)
#define GATE_ON     (GATE_HI==acq32_gate_hi)
#define GATE_OFF    (!GATE_ON)

#define CLOCK   ((inb_p(PP_STATUS)&0x40)!=0)


#define MAXCLOCK    10

#ifdef DEBUG_EXTCLOCK
static char statuses[80];
#endif


/*
 * correct for internal clock. assume ext clock 200kHz
 */
#define SAMPLES_PER_TICK(clk)     ((clk==0?200000:clk)/HZ)

static int is_clocking()
{
    if ( acq32_extclock ){

        int iclock;
        int level = CLOCK;

        for ( iclock = 0; iclock != MAXCLOCK; ++iclock ){
            if ( CLOCK != level ){
                return TRUE;
	    }
        }

        return FALSE;
    }else{
        return TRUE;             
    }
}

static int idbg;              // output from pport



void sim_SoftTransientTimerTask( void* args )
{
    struct Acq32Device* device = (struct Acq32Device*)args;
    struct DriverPrivate* dp = &device->m_dpd;
    static int istatus;
  
    idbg = (idbg+1)&0x0f; outb( idbg, PP_DATA );

    if ( dp->nsamples_actual.post < dp->nsamples_requested.post &&
	 dp->state != ST_STOP                                           ){

        dp->state = ST_RUN;

#ifdef DEBUG_EXTCLOCK
	// how fast can we poll the ppa to detect clock?
        while ( istatus != 80 ){
            statuses[istatus++] = inb(PP_STATUS);
        }
#endif
        if ( is_clocking() ){
	    dp->nsamples_actual.post += SAMPLES_PER_TICK( dp->clock.internal);
        }
        acq32_restart_timer_task( dp );
    }else{
        acq32_finish_with_engines( dp );
        istatus = 0;
    }
}


void sim_GatedTransientTimerTask( void* args )
{
    struct Acq32Device* device = (struct Acq32Device*)args;
    struct DriverPrivate* dp = &device->m_dpd;
    static int prescale;

    prescale = (prescale+1)&0x1f;


    switch( dp->state ){
    case ST_ARM:

        idbg ^= prescale==0? 0x1: 0x0; outb( idbg, PP_DATA );

        if ( GATE_ON ){
            dp->state = ST_RUN;    // fall thru
        }else{
	    break;                 // keep running
        }

    case ST_RUN:

        idbg = (idbg+1)&0x0f; outb( idbg, PP_DATA );

        if ( GATE_ON && 
             dp->nsamples_actual.post < dp->nsamples_requested.post ){
	    if ( is_clocking() ){
	        dp->nsamples_actual.post += SAMPLES_PER_TICK(dp->clock.internal);
	    }
            break;
	}
	// else fall thru
    default:
    case ST_STOP:
        acq32_finish_with_engines( dp );
        return;                   // finished
    }

    acq32_restart_timer_task( dp );
}


// WORKTODO ... gated continuous NOT implemented YET

void sim_GatedContinuousTimerTask( void* args )
{
    struct Acq32Device* device = (struct Acq32Device*)args;
    struct DriverPrivate* dp = &device->m_dpd;
    static int prescale;

    prescale = (prescale+1)&0x1f;

#define MAX_PRESAMPLES \
    (DEF_SAMPLE_TOTAL/ACQ32_MAX_CHANNELS - dp->nsamples_requested.pre)
#define MAX_POSTSAMPLES \
    (dp->itrigger+dp->nsamples_requested.post)

    switch( dp->state ){
    case ST_ARM:

        idbg ^= prescale==0? 0x1: 0x0; outb( idbg, PP_DATA );

        if ( GATE_ON ){
            dp->state = ST_RUN;    // and fall thru

            idbg = (idbg & 0xf0)&0x09;     /* WRONG: alwasy zero ?? */

        }else{
	    break;                 // keep state
        }
        
    case ST_RUN:
	/*
	 * WORKTODO ... nasty HACK to check streaming
	 * NB NEVER triggers.
	 */

        if ( dp->streaming ){
	    dp->nsamples_actual.pre += SAMPLES_PER_TICK(dp->clock.internal);
	    break;                    // keep state
        }else if ( GATE_ON ){

	    // crude approx of cyclic buffer 

            if ( dp->nsamples_actual.pre < MAX_PRESAMPLES && is_clocking() ){

	        if ( prescale&0x10 ){
                    idbg ^= 0x0f; outb( idbg, PP_DATA );
                }
	        dp->nsamples_actual.pre += SAMPLES_PER_TICK(dp->clock.internal);
	    }
            break;                               // keep state
	}else{
            dp->state = ST_TRIGGER;
	    dp->itrigger = dp->nsamples_actual.pre;  // fall thru
        }

    case ST_TRIGGER:
        if ( dp->nsamples_actual.post < MAX_POSTSAMPLES ){
	    if ( is_clocking() ){
                dp->nsamples_actual.post += SAMPLES_PER_TICK(dp->clock.internal);

                idbg = (idbg+1)&0x0f; outb( idbg, PP_DATA );
	    }
            break;                            // repeat
        }
	// else fall thru and stop
    default:
    case ST_STOP:
        acq32_finish_with_engines( dp );
        return; 
    }

    acq32_restart_timer_task( dp );
}
void sim_SoftContinuousTimerTask( void* args )
{
    struct Acq32Device* device = (struct Acq32Device*)args;
    struct DriverPrivate* dp = &device->m_dpd;
    static int prescale;

    prescale = (prescale+1)&0x1f;
/*
  #define MAX_PRESAMPLES \
  (DEF_SAMPLE_TOTAL/ACQ32_MAX_CHANNELS - dp->nsamples_requested)
  #define MAX_POSTSAMPLES \
  (dp->itrigger+dp->nsamples_requested)
*/
    switch( dp->state ){
    case ST_ARM:
        dp->state = ST_RUN;    // and fall thru
        break;                 // keep state
    case ST_RUN:
	/*
	 * WORKTODO ... nasty HACK to check streaming
	 * NB NEVER triggers.
	 */

	dp->nsamples_actual.pre += SAMPLES_PER_TICK(dp->clock.internal);
	break;                    // keep state
	// else fall thru and stop
    default:
    case ST_STOP:
        acq32_finish_with_engines( dp );
        return; 
    }

    acq32_restart_timer_task( dp );
}

static int sim_SetChannelMaskAll( 
    struct Acq32Path* path, 
    char* smask,
    int boost
)
/* smask is a string of ascii '0' - disable and '1' enable */
    // BEWARE: nasty cut of acq_setChannelMask
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
	device->m_dpd.channel_mask = *pmask;

        if ( finished ){
            return 0;
        }      
    } 

    return -ENODEV;
}


static int sim_SetArmed( struct Acq32Path* path )
{
    struct DriverPrivate* dp = &path->device->m_dpd;

    PDEBUGL(2)( "sim_SetArmed GATED_ON=%d\n", GATE_ON );


    switch ( dp->state ){
    case ST_STOP:
        if ( !dp->work_task.routine ){
            acq32_show_busy( TRUE );
            init_capture( path );
#ifdef LINUX_NEW_PCI
            init_waitqueue_head( &dp->waitq );
	    INIT_TQUEUE( &dp->work_task, 
			 dp->mode==M_SOFT_CONTINUOUS?  sim_SoftContinuousTimerTask:
			 dp->mode==M_GATED_TRANSIENT?  sim_GatedTransientTimerTask:
			 dp->mode==M_GATED_CONTINUOUS? sim_GatedContinuousTimerTask:
			 sim_SoftTransientTimerTask,
			 path->device );
#else	    	    
            dp->waitq = NULL;
            dp->work_task.next = 0;
            dp->work_task.sync = 0;
            dp->work_task.routine = 
	        dp->mode==M_SOFT_CONTINUOUS?  sim_SoftContinuousTimerTask:
                dp->mode==M_GATED_TRANSIENT?  sim_GatedTransientTimerTask:
                dp->mode==M_GATED_CONTINUOUS? sim_GatedContinuousTimerTask:
		sim_SoftTransientTimerTask;
            dp->work_task.data = path->device;
#endif	    

            queue_task( &dp->work_task, &tq_timer );
        }else{
            int timed_out = 
                interruptible_sleep_on_timeout( &dp->waitq, 3 ) == 0;

            if ( timed_out ){
		PDEBUG( "ERROR:TIMED OUT - please report to DTACQ \n" );
	    }
	}
        dp->state = ST_ARM;
        break;
    default:
	// do nothing
        ;
    }
    return 0;
}
static int sim_SetAbort( struct Acq32Path* path )
{
#ifdef DEBUG_EXTCLOCK
    int ii;

    for ( ii = 0; ii != 80; ++ii ){
        PDEBUG( "%d 0x%02x", ii, statuses[ii] );
    }
#endif
    path->device->m_dpd.state = ST_STOP;
    return 0;
}

static int sim_GetNumSamples( struct Acq32Path* path )
{
    return path->device->m_dpd.nsamples_actual.pre + 
	path->device->m_dpd.nsamples_actual.pre;
}

static int sim_SetChannel(
    struct Acq32Path* path,
    int ichan, int g1, int g2, int vex )
{
    PDEBUG( "sim_SetChannel( %d %d %d %d )\n", ichan, g1, g2, vex );

    return 0;
}

static int sim_null( struct Acq32Path* path ) 
{
    return 0;
}


static int sim_GetRange( struct Acq32Path* path )
{
    path->device->input_channels.vmax10 = V10( 10 );
    path->device->output_channels.vmin10 = V10( -10 );
    return 0;
}
static int sim_GetAvailableChannels( struct Acq32Path* path )
{
    path->device->input_channels.nchannels = 32;
    path->device->output_channels.nchannels = 2;
    return 0;
}
static int sim_GetInfo( struct Acq32Path* path, enum StatusSelect selector )
{
    switch ( selector ) {
    case SS_FW_REV:
	APR_PRINTF( path, "default:Acq32PCI copyright (c) D-TACQ Solutions Ltd 2001\n" );
        APR_PRINTF( path, "Acq32PCI image built at Sep 18 2002:21:31:55 Build 1694\n" );
	APR_PRINTF( path, "$Id: acq32_simul.c,v 1.39.4.1 2004/10/21 09:05:26 pgm Exp $\n" );
        APR_PRINTF( path, "$Revision: 1.39.4.1 $\n" );
        break;
    case SS_CONFIG:
	APR_PRINTF( path, "Simul Config\n" );
        break;
    case SS_CALINFO:
	APR_PRINTF( path, "Simul CalInfo\n" );
        break;
    case SS_CAPTURESTATS:
	APR_PRINTF( path, "Simul GetCaptureStats\n" );
        break;
    case SS_DEBUG:
	APR_PRINTF( path, "Simul GetDebug\n" );
        break;
    default:
	APR_PRINTF( path, "Simul ERROR: unknown option\n" );
    }
    return 0;
}
/*
 * instantiate the master driver object
 */

struct Acq32MasterDriver* acq32_getSimul()
{
    static struct Acq32MasterDriver _simul = {

	sim_GetState,         // GetState
	sim_SetChannelMaskAll,// SetChannelMask
	NULL,                 // GetMemoryDepth
	sim_SetMode,          // SetMode
	sim_SetArmed,         // SetArmed
	sim_SetAbort,         // SetAbort
	sim_GetNumSamples,    // GetNumSamples
	NULL,                 // GetNumChannels
	NULL,                 // OnOpen
	NULL,                 // OnRelease
	sim_null,             // SetInternalClock
	sim_null,             // SetExternalClock
	sim_null,             // GetInternalClock
	sim_null,             // SetDIO
	sim_null,             // getDIO
	sim_SetChannel,       // SetChannel 

	GetRange:	sim_GetRange,
	GetAvailableChannels:	sim_GetAvailableChannels,
	GetInfo: sim_GetInfo
    };
#define NMETHODS (sizeof(struct Acq32MasterDriver)/(sizeof (void*)))    

    // this dodgy code is a wacky way of achieving inheritance + 
    // member function overloading. c'est la vie

    void** p_simul  = (void**)&_simul;
    void** p_driver = (void**)acq32_getDriver();

    static int imethod = 0;
    
    if ( imethod == 0 ){
        for ( ; imethod != NMETHODS; ++imethod ){
	    if ( p_simul[imethod] == NULL ){
		p_simul[imethod] = p_driver[imethod];
            }
        }
    }
    
    return &_simul;
}

