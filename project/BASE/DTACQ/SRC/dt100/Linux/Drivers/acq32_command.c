/*****************************************************************************
 *
 * File: acq32_command.c - command processor for acq32-drv
 *
 * $RCSfile: acq32_command.c,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: acq32_command.c,v 1.88.4.4 2005/03/28 19:58:51 pgm Exp $
 * $Log: acq32_command.c,v $
 * Revision 1.88.4.4  2005/03/28 19:58:51  pgm
 * fixes acq196 channel mask
 *
 * Revision 1.88.4.3  2004/10/21 09:10:55  pgm
 * clean compile, no warnings
 *
 * Revision 1.88.4.2  2003/09/16 11:03:20  pgm
 * compiles clean on 2.4.20 (RH9x)
 *
 * Revision 1.88.4.1  2003/09/06 07:28:03  pgm
 * acq32 now works again after acq200 split
 *
 * Revision 1.88  2002/11/25 11:33:45  pgm
 * update from EPFL trip 20 Nov 2002
 *
 * Revision 1.87  2002/11/06 14:37:08  pgm
 * new style getDIO
 *
 * Revision 1.86  2002/11/06 14:35:46  pgm
 * MC_STATE_POSTPROCESS
 *
 * Revision 1.85  2002/11/03 20:14:16  pgm
 * get event
 *
 * Revision 1.84  2002/08/27 16:29:27  pgm
 * status read is interruptible
 *
 * Revision 1.83  2002/08/29 18:36:25  pgm
 * COS device tools OK
 *
 * Revision 1.82  2002/08/26 13:10:00  pgm
 * impl status stream - compiles 2.4, needs tesst
 *
 * Revision 1.81  2002/08/20 10:07:42  pgm
 * clean globs, use mutex for IOREAD
 *
 * Revision 1.80  2002/08/09 15:34:28  pgm
 * setChannelMask mmmm boost
 *
 * Revision 1.79  2002/08/04 12:07:11  pgm
 * boost opt to setChannelMask
 *
 * Revision 1.78  2002/08/04 09:45:31  pgm
 * remove spurious backstop message
 *
 * Revision 1.77  2002/08/03 18:56:14  pgm
 * trace fault in bucket
 *
 * Revision 1.76  2002/06/11 14:49:32  pgm
 * streaming extra prams
 *
 * Revision 1.75  2002/04/09 12:27:58  pgm
 * better proto debug
 *
 * Revision 1.74  2002/03/28 18:57:29  pgm
 * status device works
 *
 * Revision 1.73  2002/03/28 08:42:42  pgm
 * fixes isr to sep ack, cos, add waitevent, doubletap good
 *
 * Revision 1.72  2002/03/12 12:12:40  pgm
 * clean format rools OK
 *
 * Revision 1.71  2002/03/08 16:31:46  pgm
 * *** empty log message ***
 *
 * Revision 1.70  2002/03/08 16:17:55  pgm
 * *** empty log message ***
 *
 * Revision 1.69  2002/03/08 15:33:39  pgm
 * lets try again
 *
 * Revision 1.68  2002/03/08 15:21:54  pgm
 * should fix help probs
 *
 * Revision 1.67  2002/03/08 14:12:25  pgm
 * fix help - this most likely was causing random faults when no args
 *
 * Revision 1.66  2002/02/27 21:08:37  pgm
 * make help assert non-fatal (what's going on here?)
 *
 * Revision 1.65  2002/02/27 18:20:00  pgm
 * fix help - gotta work this time
 *
 * Revision 1.64  2002/02/27 17:12:34  pgm
 * fix getSyncRoute
 *
 * Revision 1.62  2002/02/27 03:09:48  pgm
 * add help part 1
 *
 * Revision 1.61  2002/02/27 02:08:47  pgm
 * getSyncRoute
 *
 * Revision 1.60  2002/02/26 16:43:24  pgm
 * command debug stream
 *
 * Revision 1.59  2002/02/14 09:11:09  pgm
 * handa2:trigger_DIx on setModeTriggeredConinuous
 *
 * Revision 1.57  2002/02/10 18:15:25  pgm
 * FAST_MULTI opt
 *
 * Revision 1.56  2001/11/23 21:54:57  pgm
 * continuous stream speed demo done
 *
 * Revision 1.55  2001/10/21 09:08:40  pgm
 * selectThresholdDetector etc
 *
 * Revision 1.54  2001/10/20 20:42:30  pgm
 * selectThresholdDistributor opts
 *
 * Revision 1.53  2001/10/20 19:44:19  pgm
 * getDebug
 *
 * Revision 1.52  2001/10/20 15:52:11  pgm
 * clock redistribution opts included
 *
 * Revision 1.51  2001/10/06 22:21:53  pgm
 * raw, flash devs no longer use ints
 *
 * Revision 1.50  2001/09/07 11:12:55  pgm
 * acq32SetChannelThreshold volts OK
 *
 * Revision 1.49  2001/09/03 18:25:43  pgm
 * channel threshold 1..N, friendly
 *
 * Revision 1.48  2001/09/03 15:53:05  pgm
 * threshold setting done
 *
 * Revision 1.47  2001/09/03 13:47:55  pgm
 * thresh trigger works (above, below right wya round
 *
 * Revision 1.46  2001/08/23 18:31:22  pgm
 * minor fix to setUser command handling
 *
 * Revision 1.45  2001/08/19 20:23:47  pgm
 * threshold commands safely on board
 *
 * Revision 1.44  2001/08/19 16:59:00  pgm
 * add threshold commands (untested)
 *
 * Revision 1.43  2001/08/19 08:52:55  pgm
 * resetGUT opt
 *
 * Revision 1.42  2001/06/25 21:57:00  pgm
 * view commands added
 *
 * Revision 1.41  2001/06/23 11:22:02  pgm
 * BE opt
 *
 * Revision 1.40  2001/06/11 19:33:30  pgm
 * *** empty log message ***
 *
 * Revision 1.39  2001/06/11 09:02:02  pgm
 * setClock, clockNow hooked up
 *
 * Revision 1.38  2001/06/02 19:57:57  pgm
 * reserveAO
 *
 * Revision 1.37  2001/06/01 20:10:21  pgm
 * GUT - hooking up core, compiles, willit work
 *
 * Revision 1.36  2001/06/01 19:45:32  pgm
 * GUT WIP - hooking up core, compiles
 *
 * Revision 1.35  2001/05/31 18:52:37  pgm
 * GUT - stubs compile
 *
 * Revision 1.34  2001/04/27 17:19:50  pgm
 * fix race in getFwrev
 *
 * Revision 1.33  2001/04/15 09:52:45  pgm
 * syncSetRoute() works
 *
 * Revision 1.32  2001/03/28 21:18:29  pgm
 * i2o command handling for FETCH cmpiles - willit work??
 *
 * Revision 1.31  2001/01/14 17:54:11  pgm
 * multi thread safemake
 *
 * Revision 1.30  2001/01/02 12:11:08  pgm
 * slight cleanup
 *
 * Revision 1.29  2000/10/28 21:18:59  pgm
 * getSetNumSamples
 *
 * Revision 1.28  2000/10/22 20:36:15  pgm
 * Get Internal Clock now returns actual
 *
 * Revision 1.27  2000/10/07 21:33:05  pgm
 * trigger continuous  etc commands impl
 *
 * Revision 1.26  2000/10/01 21:38:34  pgm
 * bigdump, getFwrev features work
 *
 * Revision 1.25  2000/10/01 18:20:35  pgm
 * bigdump, getFwrev first cut
 *
 * Revision 1.24  2000/10/01 08:27:49  pgm
 * fix rowdev read bug - uses channel mask, ArmSpin flash prog feature
 *
 * Revision 1.23  2000/09/26 19:46:52  pgm
 * GATED_CONTINUOUS pre/post, lseek, spin
 *
 * Revision 1.22  2000/09/22 13:59:45  pgm
 * min reads per sec timing implemented
 *
 * Revision 1.21  2000/06/25 19:41:01  pgm
 * soft continuous and others
 *
 * Revision 1.20  2000/06/24 10:57:46  pgm
 * i2o round trip works
 *
 * Revision 1.19  2000/06/11 16:00:44  pgm
 * setChannel converts from world to index units
 *
 * Revision 1.18  2000/05/28 18:07:06  pgm
 * comment
 *
 * Revision 1.17  2000/05/28 10:31:12  pgm
 * streaming works passibly in simulator
 *
 * Revision 1.16  2000/05/20 11:03:47  pgm
 * Rev 6 commands work + sim sample tagging
 *
 * Revision 1.15  2000/05/19 21:15:58  pgm
 * ICD rev6 command handling parsers in
 *
 * Revision 1.14  2000/04/23 18:06:27  pgm
 * fix env var dependencies, warnings
 *
 * Revision 1.13  2000/01/09 21:53:58  pgm
 * fix bug in setChannelMask command - input buf overflow
 *
 * Revision 1.12  2000/01/08 22:07:50  pgm
 * get num channels now reflects multi master mask
 *
 * Revision 1.11  1999/11/20 21:20:30  pgm
 * 991120 time for bed
 *
 * Revision 1.10  1999/11/13 23:03:40  pgm
 * format volts lines dos finally works!
 *
 * Revision 1.9  1999/11/13 20:31:37  pgm
 * error reporting in master, read funcs
 *
 * Revision 1.8  1999/11/12 22:15:20  pgm
 * lines and dos mode added
 *
 * Revision 1.7  1999/11/12 17:10:40  pgm
 * fix starange volts format bug; acqN.mN device
 *
 * Revision 1.6  1999/11/03 21:49:55  pgm
 * stash for backup
 *
 * Revision 1.5  1999/11/02 21:35:53  pgm
 * bocking Arm is reliable - ship it
 *
 * Revision 1.4  1999/10/26 15:16:54  pgm
 * struct debug, mbox commands
 *
 * Revision 1.3  1999/10/16 00:13:00  pgm
 * remove null in output stream BUG
 *
 * Revision 1.2  1999/10/14 22:51:50  pgm
 * simul engine runs - SHIP IT QUICKcvs status | grep Status
 *
 * Revision 1.1  1999/10/14 21:10:43  pgm
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

#include <linux/module.h>

#include <linux/kernel.h> /* printk() */
#include <linux/fs.h>     /* everything... */
#include <linux/errno.h>  /* error codes */
#include <linux/types.h>  /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>        /* O_ACCMODE */
#include <linux/pci.h>
#include <linux/time.h>
#include <linux/sched.h>        /* schedule(), signal_pending() */

#include <asm/io.h>       /* ioremap()         */
#include <asm/uaccess.h>  /* VERIFY_READ|WRITE */
#include <asm/system.h>   /* cli(), *_flags */
#include <asm/segment.h>  /* memcpy and such */


#include "acq32.h"        /* local definitions */
#include "acq32ioctl.h"   /* APP interface defs */

#include "dt100api.h"

#include "platform.h"



#define APR_PRINTF ACQ32_PATH_READBUFFER_PRINTF

#define APR_DPRINTF \
        ACQ32_PATH_READBUFFER_PRINTF( PD(filp), "%s %4d ", __FILE__, __LINE__ );\
        APR_PRINTF
	
static const char* DEF_FMT( enum ChannelMode mode )
{
    switch( mode ){
    default:
    case CM_HEX:
        return "0x%04x";
    case CM_DEC:
        return "%5d";
    case CM_VOLTS:
        return "%8s";
    }
}


// WORKTODO ...

enum { 
    ASPACE     = ' ', 
    AQUOTE     = '"', 
    EOL        = '\n', 
    ASEP       = ';' ,
    C_ESCAPE   = '\\'
};

static void substitute_escape( char* pchar )
{
    switch( *pchar ){
    case 'n':
        *pchar = '\n';        break;
    case 'r':
        *pchar = '\r';        break;
    case 't':
        *pchar = '\t';         break;
    default:
	break;              // typ \" escapes DQUOTE char
    }
}

static void shuffle_up( char* pstr, int ishuffle )
{
    char* pto   = pstr;
    char* pfrom = pstr+ishuffle-1;

    ASSERT( ishuffle <= strlen(pstr) );

    while( *pfrom ){
        *pto++ = *++pfrom;
    }
}
static int getargs( char* aline, char**argv, int maxargs, char** nextline )
// split a line up into args, storing in argv, return argc 
// proceeds until first of - EOS, EOL or SEPARATOR
// outputs: update line position in nextline
{
    char* psep1 = strchr( aline, EOL );
    char* psep2 = strchr( aline, ASEP );
    int isep1 = psep1? psep1 - aline: 0;
    int isep2 = psep2? psep2 - aline: 0;
    int ieol  = MAX( isep1, isep2 );
    int argc = 0;

    enum { 
        IN_SPACE, 
        IN_TOKEN, 
        IN_QUOTES,
        FINISHED_QUOTES
    } state = IN_SPACE;


    int escaped = FALSE;
    int ichar;

    /*
     * parse until first separator
     */
    if ( ieol == 0 ){
        if ( psep1 || psep2 ){
	    *nextline = &aline[1];         // blank line, skip
        }
        return 0;
    }

    for ( ichar = 0; ichar != ieol; ++ichar ){
        if ( argc == maxargs ){
            PDEBUGL(2)( "acq32:WARNING: args limit exceeded\n" );
            break;
        }

        switch( state ){
        case IN_SPACE:
        default:
	    switch( aline[ichar] ){
            case ASPACE:
                continue;
            case AQUOTE:
                argv[argc++] = &aline[ichar];
                shuffle_up( &aline[ichar--], 1 ); ieol--;		
                state = IN_QUOTES;
                continue;
            default:
                argv[argc++] = &aline[ichar];
                state = IN_TOKEN;
                continue;
            }
        case IN_TOKEN:
	    switch( aline[ichar] ){
            case ASPACE:
                aline[ichar] = '\0';
                state = IN_SPACE;
                continue;
            default:
                continue;
            }
        case IN_QUOTES:
	    if ( escaped ){
                substitute_escape( &aline[ichar] );
                escaped = FALSE;
                continue;
            }else{
    	        switch( aline[ichar] ){
                case C_ESCAPE:
                    shuffle_up( &aline[ichar--], 1 ); ieol--;
                    escaped = TRUE;
                    continue;
                case AQUOTE:
                    shuffle_up( &aline[ichar--], 1 ); ieol--;
                    state = FINISHED_QUOTES;
                    continue;
                default:
                    continue;
                }
            }
        case FINISHED_QUOTES:
	    switch( aline[ichar] ){
            case ASPACE:
                aline[ichar] = '\0';
                state = IN_SPACE;
                continue;
            default:
                PDEBUGL(2)( "acq32:WARNING: char %c dropped\n", aline[ichar] );
                state = IN_SPACE;
                continue;
            }
        }
    }

    aline[ichar++] ='\0';    
    *nextline = &aline[ichar];
    return argc;
}

/*
 * a decoder func consumes argv's, starting at iarg
 * returns finishing iarg.
 * if no args consumed, returns iarg unchanged
 * returns negative on error
 */

typedef int (*decoder_func)( 
    struct file* filp, 
    char** argv, int argc, 
    int iarg 
    );

static int do_GetNumChannels( 
    struct file* filp, 
    char** argv, int argc, 
    int iarg 
    )
{
    if ( STREQ( argv[iarg], MC_GET_NUMCHANNELS ) ){

        char buf[80];
        int nchannels = ACQ32_MAX_CHANNELS;

        if ( PDRVR(filp)->GetNumChannels ){
            nchannels = PDRVR(filp)->GetNumChannels( PD(filp) );
        }
        
        sprintf( buf, "%s=%d", MC_GET_NUMCHANNELS, nchannels );

        acq32_path_readbuffer_put( PD( filp ), buf, strlen(buf)
	    );
        return iarg + 1;
    }
    return iarg;
}


static int do_GetMemoryDepth( 
    struct file* filp, 
    char** argv, int argc, 
    int iarg 
    )
{
    if ( STREQ( argv[iarg], MC_GET_MEMORY_DEPTH ) ){

        if ( ++iarg < argc ){
            int ichannel = strtol( argv[iarg++], 0, 0 );
 
            int idepth = PDRVR(filp)->GetMemoryDepth( PD(filp), ichannel );
       
            ACQ32_PATH_READBUFFER_PRINTF( 
                PD(filp), MC_GET_MEMORY_DEPTH "=%d", idepth );

            return iarg;
        }
    }

    return iarg;
}



static int do_GetNumSamples( 
    struct file* filp, 
    char** argv, int argc, 
    int iarg 
    )
{
    if ( STREQ( argv[iarg], MC_GET_NUMSAMPLES ) ){

	int totsamples = PDRVR(filp)->GetNumSamples(PD(filp));

	APR_PRINTF( 
	    PD(filp), 
	    MC_GET_NUMSAMPLES "=%d pre=%d post=%d",
            totsamples, 
	    PDEV(filp)->m_dpd.nsamples_actual.pre,
	    PDEV(filp)->m_dpd.nsamples_actual.post      
	    );
		    
        return iarg + 1;
    }
    return iarg;
}

static int do_GetSetNumSamples( 
    struct file* filp, 
    char** argv, int argc, 
    int iarg 
    )
{
    if ( STREQ( argv[iarg], MC_GET_SETSAMPLES ) ){

	int totsamples = PDRVR(filp)->GetNumSamples(PD(filp));

        ACQ32_PATH_READBUFFER_PRINTF( 
	    PD( filp ),
	    "%s=%d pre=%d post=%d", 
	    MC_GET_SETSAMPLES, 
            totsamples, 
	    PDEV(filp)->m_dpd.nsamples_requested.pre,
	    PDEV(filp)->m_dpd.nsamples_requested.post      
	    );

        return iarg + 1;
    }
    return iarg;
}


static int help_printf( 
    struct file* filp,
    const char* verb, 
    const char* cmd, 
    const char* txt 
    )
{
    if ( STREQ( verb, MC_GET_HELP ) ){
        if ( cmd == 0 ){
	    PDEBUGL(1)( "HPR ret %d\n", (int)txt );
	    return (int)txt;
	}else{
            APR_PRINTF( PD(filp), "%-30s : %s\n", cmd, txt );
	    return 0;
	}
    }
    return -1;
}

/*
 * assumes all decoders have local vars verb, cmd
 */
 
#define HELP_PRINTF( cmd, txt )      help_printf( filp, verb, cmd, txt )

#define HELP_PRINTF_RET( cmd, txt )  \
    do { \
        int rv = help_printf( filp, verb, cmd, txt );\
	    if ( rv >= 0 ) return rv;\
	} while( 0 )


static int do_help( 
    struct file* filp, 
    char** argv, int argc, 
    int iarg 
    )
{
    const char* verb = argv[iarg];
    char buf[256];
    
    if ( STREQ( verb, MC_GET_HELP ) ){
        APR_PRINTF( PD(filp), "\n" );
        acq32_report_version( buf, sizeof(buf) );
	APR_PRINTF( PD(filp), buf );
	return 0;
    }else{
        return iarg;
    }
}
	
static int do_SetAbort( 
    struct file* filp, 
    char** argv, int argc, 
    int iarg 
    )
{
    const char* verb = argv[iarg];
    
    HELP_PRINTF_RET( MC_SET_ABORT, "abort capture" );

    if ( STREQ( argv[iarg], MC_SET_ABORT ) ){
        PDRVR(filp)->SetAbort( PD(filp) );
        iarg += 1;
    }
    return iarg;
}

static int do_SetArm( 
    struct file* filp, 
    char** argv, int argc, 
    int iarg 
    )
{
    const char* verb = argv[iarg];
    
    HELP_PRINTF_RET( MC_SET_ARM, "arm capture" );
    
    if ( STREQ( argv[iarg], MC_SET_ARM ) ){

        iarg += 1;
        if ( iarg != argc && STREQ( argv[iarg], "nonblocking" ) ){
            PD(filp)->is_blocking = FALSE;
            iarg += 1;
        }
        PDEV(filp)->m_dpd.global_sample_id = 0LL;
        PDEV(filp)->m_dpd.subframe_id = 0;

        PDRVR(filp)->SetArmed( PD(filp) );
    }
    return iarg;
}


struct KeyLut {
    int   ikey;
    char* skey;
};


static struct KeyLut S_state_lut[] = {
    { ST_STOP,     MC_STATE_STOP    },
    { ST_ARM,      MC_STATE_ARM     },
    { ST_RUN,      MC_STATE_RUN     },
    { ST_TRIGGER,  MC_STATE_TRIGGER },
    { ST_POSTPROCESS, MC_STATE_POSTPROCESS },
    {}
};

#ifdef PGMCOMOUT
static char* lkupValue( int key, struct KeyLut* lut, int nlut ) {
    int ii;
    
    for ( ii = 0; ii != nlut; ++ii ){
        if ( lut[ii].ikey == key ){
	    return lut[ii].skey;
	}
    }
    
    return NULL;
}
#endif
static int lkupKey( const char* value, struct KeyLut* lut, int nlut ) {
    int ii;
    
    for ( ii = 0; ii != nlut; ++ii ){
        if ( STREQ( lut[ii].skey, value ) ){
	    return lut[ii].ikey;
	}
    }
    
    return -1;
}



static int lkupDIx( const char* key )
{
    static struct KeyLut _lut[] = {
	{ ACQ32_DI_0, DI(0) },
	{ ACQ32_DI_1, DI(1) },
	{ ACQ32_DI_2, DI(2) },
	{ ACQ32_DI_3, DI(3) },
	{ ACQ32_DI_4, DI(0) }		    		    		    		    
    };
#define NLUT (sizeof(_lut)/sizeof(struct KeyLut))
    int diX = lkupKey( key, _lut, NLUT );
	
    return diX;
}


/*
static int getStateFromKey( enum STATE* pstate, char* key )
{
    int ilut;

    for ( ilut = 0; S_state_lut[ilut].skey; ++ilut ){
        if ( STREQ( key, S_state_lut[ilut].skey ) ){
            *pstate = (enum STATE)S_state_lut[ilut].ikey;
            return TRUE;
        }
    }
    
    ASSERT( 0 );
    return FALSE;
}
*/
static char* getKeyFromState( enum STATE state )
{
    int ilut;

    for ( ilut = 0; S_state_lut[ilut].skey; ++ilut ){
        if ( state == (enum STATE)S_state_lut[ilut].ikey ){
            return S_state_lut[ilut].skey;
        }
    }
    
    return "ST_UNKNOWN";
}

static struct KeyLut S_mode_lut[] = {
    { M_GATED_TRANSIENT,      MC_MODE_GATED_TRANSIENT         },
    { M_GATED_CONTINUOUS,     MC_MODE_GATED_CONTINUOUS        },
    { M_SOFT_TRANSIENT,       MC_MODE_SOFT_TRANSIENT          },
    { M_SOFT_CONTINUOUS,      MC_MODE_SOFT_CONTINUOUS         },
    { M_TRIGGERED_CONTINUOUS, MC_MODE_TRIGGERED_CONTINUOUS    },
    {}  
};

static int getModeFromKey( enum MODE* pmode, char* key )
{
    int ilut;

    for ( ilut = 0; S_mode_lut[ilut].skey; ++ilut ){
        if ( STREQ( key, S_mode_lut[ilut].skey ) ){
            *pmode = (enum MODE)S_mode_lut[ilut].ikey;
            return TRUE;
        }
    }
    
    return FALSE;
}

static char* getKeyFromMode( enum MODE mode )
{
    int ilut;

    for ( ilut = 0; S_mode_lut[ilut].skey; ++ilut ){
        if ( mode == (enum MODE)S_mode_lut[ilut].ikey ){
            return S_mode_lut[ilut].skey;
        }
    }
    
    return "M_UNKNOWN";
}

void acq32_doGetStateWork( 
    struct file* filp,
    unsigned status,
    unsigned last_status )
{
    enum STATE state = STATBUF_GET_STATUS( status );
    u32 jiffies_now = STATBUF_GET_JIFFIES( status );
    u32 jiffies_last= STATBUF_GET_JIFFIES( last_status );

    APR_PRINTF( PD(filp), "%08d %04d %d %s %s\n", 
		jiffies_now,
		jiffies_now-jiffies_last,
		state, 
		getKeyFromState( state ),
		STATBUF_GET_ERROR(status)? "ERROR": "OK"
    );
}

static int do_GetState( 
    struct file* filp, 
    char** argv, int argc, 
    int iarg 
    )
{
    const char* verb = argv[iarg];
    
    HELP_PRINTF_RET( MC_GET_STATE, "get board state" );
    
    if ( STREQ( argv[iarg], MC_GET_STATE ) ){
	enum STATE state = PD(filp)->device->p_md->GetState( PD(filp) );

        iarg+= 1;

        APR_PRINTF( PD(filp), "%d %s", state, getKeyFromState( state ) );
    }
    return iarg;
}

static int do_GetMode( 
    struct file* filp, 
    char** argv, int argc, 
    int iarg 
    )
{
    if ( STREQ( argv[iarg], MC_GET_MODE ) ){
        enum MODE mode = PD(filp)->device->m_dpd.mode;

        APR_PRINTF( 
            PD(filp),
            "%d %s ", mode, getKeyFromMode( mode ) 
	    );

        iarg+= 1;
    }
    return iarg;
}

static int do_BigDump(
    struct file* filp, 
    char** argv, int argc, 
    int iarg 
    )
    // send the command
    // wait for interrupts response
    // return useful info
{
    const char* verb = argv[iarg];
    
    HELP_PRINTF_RET( MC_BIGDUMP, "big dump to DMA buf [deprecated]" );
    
    if ( acq32_simulate ) return iarg;
    
    if ( STREQ( argv[iarg], MC_BIGDUMP ) ){
        int ierrno = PD(filp)->device->p_md->DoBigdump( PD(filp) );

        if ( ierrno < 0 ){
	    ACQ32_PATH_READBUFFER_PRINTF( 
                PD(filp), "ERROR:bigdump failed: %d\n", ierrno );
        }

        iarg += 1;
    }
    return iarg;
}

static int do_GetInfo(
    struct file* filp, 
    char** argv, int argc, 
    int iarg 
    )
{
    static struct KeyLut _lut[] = {
        { SS_FW_REV,         MC_GET_FWREV        },
        { SS_CONFIG,         MC_GET_CONFIG       },
        { SS_CALINFO,        MC_GET_CALINFO      },
        { SS_CAPTURESTATS,    MC_GET_CAPTURESTATS },
	{ SS_DEBUG,          MC_GET_DEBUG        }
    };
#define NLUT (sizeof(_lut)/sizeof(struct KeyLut))
    
    enum StatusSelect selector;

    
    if ( acq32_simulate ) return iarg;

    selector = lkupKey( argv[iarg], _lut, NLUT );
    
    if ( selector == - 1 ){
        return iarg;
    }else{
        int ierrno = PD(filp)->device->p_md->GetInfo( PD(filp), selector );

        if ( ierrno < 0 ){
            ACQ32_PATH_READBUFFER_PRINTF( 
		PD(filp), "ERROR:getInfo() failed: %d\n", ierrno );
        }

        iarg += 1;

    }
    return iarg;
}


static int do_SetChannelMask(
    struct file* filp, 
    char** argv, int argc, 
    int iarg 
    )
{
    int iarg_orig = iarg;
    const char* verb = argv[iarg];    

    HELP_PRINTF_RET( 
        MC_SET_CHANNEL_MASK " m1..mmmmm...m32", 
	"m = [01], mask channels in pairs" );
	
    if ( STREQ( argv[iarg], MC_SET_CHANNEL_MASK ) ){
	if ( ++iarg < argc ){
	    char* mask = argv[iarg++];
	    int boost = (iarg < argc )? STRTOL( argv[iarg++] ): 0;
	    int ierrno = PDRVR(filp)->SetChannelMask( PD(filp), mask, boost );

	    if ( ierrno < 0 ){
		ACQ32_PATH_READBUFFER_PRINTF( 
		    PD(filp), "ERROR:SetChannelMask failed: %d\n", ierrno );
	    }
	}else{
	    int ierrno = PDRVR(filp)->SetChannelMask( PD(filp), NULL, 0 );

	    if ( ierrno < 0 ){
		ACQ32_PATH_READBUFFER_PRINTF( 
		    PD(filp), "ERROR:SetChannelMask failed: %d\n", ierrno );
	    }
	}
	return iarg;
    }

    return iarg_orig;
}



static int do_SetMode( 
    struct file* filp, 
    char** argv, int argc, 
    int iarg 
    )
{
    const char* verb = argv[iarg];
    int iarg_orig = iarg;
    
    HELP_PRINTF_RET( 
        MC_SET_MODE " <mode> <postlen>", 
	"mode={ SOFT_TRANSIENT, GATED_TRANSIENT " );
    
    if ( STREQ( argv[iarg], MC_SET_MODE ) ){
        if ( ++iarg < argc ){

            enum MODE mode;

            if ( getModeFromKey( &mode, argv[iarg] ) ){
 
                int ierrno;
                int nsamples;

                if ( ++iarg < argc ){
                    nsamples = strtol( argv[iarg++], 0, 0 );
                }else{
                    nsamples = DEF_SAMPLES;
                }

                ierrno = PDRVR(filp)->SetMode( PD(filp), mode, nsamples, -1, 0 );

                if ( ierrno < 0 ){
                    ACQ32_PATH_READBUFFER_PRINTF( 
                        PD(filp), "ERROR:SetMode failed: %d\n", ierrno );
                }

                return iarg;
	    }else{
                ACQ32_PATH_READBUFFER_PRINTF( 
		    PD(filp), "ERROR:SetMode mode not understood\n" );
            }
	}else{
            ACQ32_PATH_READBUFFER_PRINTF(
                PD(filp), "ERROR:SetMode command - needs mode\n" );
	}
    }
    return iarg_orig;
}

static int do_SetModeGatedContinuous( 
    struct file* filp, 
    char** argv, int argc, 
    int iarg 
    )
{
//#define FN "SetModeGatedContinuous"
    const char* verb = argv[iarg];
    int iarg_orig = iarg;

    HELP_PRINTF_RET( MC_SET_MODE_GC " <pre> <post>", "set pre, post capture" );
    
    if ( STREQ( argv[iarg], MC_SET_MODE_GC ) ){
        if ( ++iarg < argc ){

            int ierrno;
            int nsamples = DEF_SAMPLES;
            int presamples = -1;
	    int diX = DIX_DEFAULT;
	    

            if ( iarg < argc )  presamples = strtol( argv[iarg++], 0, 0);
            if ( iarg < argc )  nsamples = strtol( argv[iarg++], 0, 0 );
	    if ( iarg < argc )  diX = lkupDIx( argv[iarg++] );

            if ( diX == -1 ){
                ACQ32_PATH_READBUFFER_PRINTF( 
		    PD(filp), "ERROR:"" failed: diX (%s) not recognised\n", 
		    argv[iarg-1]    );
            }

            ierrno = PDRVR(filp)->SetMode( 
		PD(filp), M_GATED_CONTINUOUS, 
		nsamples, presamples, diX );

            if ( ierrno < 0 ){
                ACQ32_PATH_READBUFFER_PRINTF( 
		    PD(filp), "ERROR:"" failed: %d\n", ierrno );
            }

            return iarg;
	}else{
            ACQ32_PATH_READBUFFER_PRINTF(
                PD(filp), "ERROR:"" command - needs mode\n" );
	}
    }
    return iarg_orig;
//#undef FN
}

static int do_SetModeTriggeredContinuous( 
    struct file* filp, 
    char** argv, int argc, 
    int iarg 
    )
{
//#define FN "SetModeTriggeredContinuous"
    const char* verb = argv[iarg];
    int iarg_orig = iarg;

    HELP_PRINTF_RET( MC_SET_MODE_TRC " <pre> <post>", "set pre post mode" );
    
    if ( STREQ( argv[iarg], MC_SET_MODE_TRC ) ){
        if ( ++iarg < argc ){

            int ierrno;
            int nsamples = DEF_SAMPLES;
            int presamples = -1;
	    int diX = DIX_DEFAULT;

            if ( iarg < argc )  presamples = strtol( argv[iarg++], 0, 0);
            if ( iarg < argc )  nsamples = strtol( argv[iarg++], 0, 0 );
	    if ( iarg < argc )  diX = lkupDIx( argv[iarg++] );

            if ( diX == -1 ){
                ACQ32_PATH_READBUFFER_PRINTF( 
		    PD(filp), "ERROR:"" failed: diX (%s) not recognised\n", 
		    argv[iarg-1]    );
            }
	        

            ierrno = PDRVR(filp)->SetMode( 
		PD(filp), M_TRIGGERED_CONTINUOUS, 
		nsamples, presamples, diX );

            if ( ierrno < 0 ){
                ACQ32_PATH_READBUFFER_PRINTF( 
		    PD(filp), "ERROR:"" failed: %d\n", ierrno );
            }

            return iarg;
	}else{
            ACQ32_PATH_READBUFFER_PRINTF(
                PD(filp), "ERROR:"" command - needs mode\n" );
	}
    }
    return iarg_orig;
//#undef FN
}


// custom format strings ... work to do ...

#define HAS_QUOTES( tok )        0
#define STRIP_QUOTES( tok )      0


static int do_SetDataRoute_command( 
    struct file* filp,
    char** argv, int argc,
    int iarg
    )
{
    const char* verb = argv[iarg];
    int iarg_orig = iarg;
    int ierrno;
//#define FN "SetRoute_command()"

    HELP_PRINTF_RET( MC_SET_DATA_ROUTE, "set routing [deprecated]" );
    
    if ( STREQ( argv[iarg], MC_SET_DATA_ROUTE ) && ++iarg < argc ){
   
        char* token = argv[iarg++];
        int route_id;

        if ( STREQNL( token, "raw" ) ){
            route_id = 0;
        }else if ( STREQNL( token, "row" ) ){
            route_id = 1;
        }else if ( STREQNL( token, "chan" ) ){
            route_id = 2;
        }else{
	    ACQ32_PATH_READBUFFER_PRINTF( PD(filp), "ERROR:bad key\n" );
	    return iarg_orig;
        }

        ierrno = PDRVR( filp )->SetRoute( PD( filp ), route_id );

        if ( ierrno < 0 ){
            ACQ32_PATH_READBUFFER_PRINTF( PD(filp), 
					  "ERROR:"" failed %d\n", ierrno );
        }
    }    
//#undef FN
    return iarg;
}

static int do_SetDistributor_command( 
    struct file* filp,
    char** argv, int argc,
    int iarg
    )
{
    const char* verb = argv[iarg];
    int iarg_orig = iarg;

    int ierrno;
//#define FN "SetDistributor_command()"

    HELP_PRINTF_RET( MC_SET_DISTRIBUTOR, "set data distributor [expert]" );
    
    if ( STREQ( argv[iarg], MC_SET_DISTRIBUTOR ) && ++iarg < argc ){
           
        int dist_id = strtol( argv[iarg++], 0, 0 );

	ierrno = PDRVR( filp )->SetDistributor( PD( filp ), dist_id );

	if ( ierrno < 0 ){
            ACQ32_PATH_READBUFFER_PRINTF( PD(filp), 
					  "ERROR:"" failed %d\n", ierrno );
	}

        return iarg;
    }else{
        return iarg_orig;
    }    
//#undef FN
}



static int do_format_command( 
    struct file* filp,
    char** argv, int argc,
    int iarg
    )
{
    const char* verb = argv[iarg];
    int iarg_orig = iarg;

    Acq32ChannelFileInfo* p_cfi = PD(filp)->info;

    HELP_PRINTF_RET( CC_FORMAT, "formatted output (small data sets only)" );
    
    if ( STREQ( argv[iarg], CC_FORMAT ) ){

        char* custom_format = NULL;

        // expect one or two strings ...
        while( ++iarg < argc ){

            char* token = argv[iarg];

            if ( STREQNL( token, CC_FORMAT_BIN ) ){
                p_cfi->mode = CM_BINARY;
                p_cfi->text_format[0] = '\0';
                return ++iarg;
	    }else if ( STREQNL( token, CC_FORMAT_BIGEND ) ){
		p_cfi->mode = CM_BIGEND;
                p_cfi->text_format[0] = '\0';
                return ++iarg;			
            }else if ( STREQNL( token, CC_FORMAT_HEX ) ){
                p_cfi->mode = CM_HEX;
            }else if ( STREQNL( token, CC_FORMAT_DEC ) ){
                p_cfi->mode = CM_DEC;
            }else if ( STREQNL( token, CC_FORMAT_VOLTS ) ){
                p_cfi->mode = CM_VOLTS;
            }else if ( STREQNL( token, CC_FORMAT_LINENUMS ) ){
                p_cfi->mode |= CM_LINENUMS;
            }else if ( STREQNL( token, CC_FORMAT_DOS ) ){
                p_cfi->mode |= CM_DOS_LINES;
            }else if ( HAS_QUOTES( token ) ){
                custom_format = STRIP_QUOTES( token );
            }else{
                ACQ32_PATH_READBUFFER_PRINTF( PD(filp), "ERROR:bad key\n" );
                return iarg_orig;
            }
        }               

	strcpy( p_cfi->text_format, 
                custom_format? 
                custom_format: 
                DEF_FMT(p_cfi->mode&CM_EXCLUSIVE_OPTS) );

        return iarg;
    }else{
        return iarg;
    }    
}


static int do_seek_command (
    struct file* filp,
    char** argv, int argc,
    int iarg
    )
{
    const char* verb = argv[iarg];
    int iarg_orig = iarg;

    HELP_PRINTF_RET( CC_SEEK, "seek in data set" );
    
    if ( STREQ( argv[iarg], CC_SEEK ) ){

        PDEBUGL(2)( "do_seek_command %d\n", __LINE__ );

        if ( !PDEV(filp)->m_dpd.nsamples_actual.post ){
            PDRVR(filp)->GetNumSamples( PD(filp) );
        }


        // expect one or two strings ...
        if( ++iarg < argc ){            // seek <whence> - compulsory

	    struct Acq32Device* device = PDEV(filp);
	    char* token = argv[iarg];
	    char* value = "0";
	    long old_pos = filp->f_pos;
	    long pos;

	    int n_pretrigger = device->m_dpd.nsamples_actual.pre;
	    int n_total      = device->m_dpd.nsamples_actual.post+n_pretrigger;

	    if ( ++iarg < argc ){
		value = argv[iarg++];      // seek <whence> [n]
	    }
	    pos = strtol( value, 0, 0 );

	    if ( STREQNL( token, CC_SEEK_BEGIN ) ){
		filp->f_pos = pos;
	    }else if ( STREQNL( token, CC_SEEK_END ) ){
		filp->f_pos = n_total-pos;
	    }else if ( STREQNL( token, CC_SEEK_CURRENT ) ){
		filp->f_pos += pos;
	    }else if ( STREQNL( token, CC_SEEK_TRIG ) ){
		filp->f_pos = n_pretrigger+pos;
	    }else{
		PDEBUGL(2)( "do_seek_command %d (ERROR)\n", __LINE__ );
		ACQ32_PATH_READBUFFER_PRINTF( PD(filp), "ERROR:bad key\n" );
		return iarg_orig;
	    }
	    filp->f_pos = MAX( 0, filp->f_pos );
	    filp->f_pos = MIN( filp->f_pos, n_total );           

	    PDEBUGL(1)( "seek %s %ld. f_pos was %ld now %ld\n",
			token, pos, old_pos, (long)filp->f_pos      );
	}
    }              

    return iarg;  
}    

static int do_limit_command (
    struct file* filp,
    char** argv, int argc,
    int iarg
    )
{
    const char* verb = argv[iarg];
    int iarg_orig = iarg;

    if ( STREQ( verb, CC_LIMIT ) ){

        if ( !PDEV(filp)->m_dpd.nsamples_actual.post ){
            PDRVR(filp)->GetNumSamples( PD(filp) );
        }


        // expect one string ...
        if( ++iarg < argc ){            // limit <n> - compulsory

            char* value = argv[iarg++];
            int nlimit = strtol( value, 0, 0 );

            if ( nlimit > 0 ){
		PD(filp)->nsamples = 
		    MIN( nlimit, PDEV(filp)->m_dpd.nsamples_actual.post ); // ???
		return iarg;
            }else{
		ACQ32_PATH_READBUFFER_PRINTF( 
		    PD(filp), "ERROR:limit must be greater than 0\n" );
            }
        }
    }               
    return iarg_orig;	
}    



static int do_bucket_command( 
    struct file* filp,
    char** argv, int argc,
    int iarg
    )
    // chew up the rest of the args
{
//#define FN "do_bucket_command() "
    char reject_line[80];
    const char* verb = argv[iarg];

    PDEBUGL(1)( " %s\n", argv[0] );
    HELP_PRINTF_RET( (char*)0, (char*)argc );
    
    strcpy( reject_line, "ERROR in command:" );
    while( iarg != argc ){
        strcat( reject_line, argv[iarg] );
        strcat( reject_line, " " );
    	++iarg;
    }
    strcat( reject_line, "(bucket)" );
    acq32_path_readbuffer_put( PD(filp), reject_line, strlen(reject_line) );
    
    PDEBUGL(1)( " returns %d\n", iarg );

    return iarg;
//#undef FN
}


static int 
do_SetInternalClock_command(
    struct file* filp,
    char** argv,
    int argc,
    int iarg
    )
{
    const char* verb = argv[iarg];
    int iarg_orig = iarg;

    int ierrno;

    HELP_PRINTF_RET( MC_SET_INTERNAL_CLOCK " <Hz> [DOx]", 
		     "set internal clock" );    
    
    if ( STREQ( argv[iarg], MC_SET_INTERNAL_CLOCK ) ){
        if ( ++iarg < argc ){
            PDEV(filp)->m_dpd.clock.internal = strtol( argv[iarg++], 0, 0 );
            PDEV(filp)->m_dpd.clock.doX = 0;
	    PDEV(filp)->m_dpd.clock.diX = 0; 

            // optional master arg
	    if ( iarg < argc ){
		static struct KeyLut _lut[] = {
		    { ACQ32_DO_0, DO(0) },
		    { ACQ32_DO_1, DO(1) },
		    { ACQ32_DO_2, DO(2) },
		    { ACQ32_DO_3, DO(3) },
		    { ACQ32_DO_4, DO(0) }		    		    		    		    
		};
#define NLUT (sizeof(_lut)/sizeof(struct KeyLut))
		        
		int doX = lkupKey( argv[iarg++], _lut, NLUT );
			    
		if ( doX != -1 ){
		    PDEV(filp)->m_dpd.clock.doX = doX;
		}else{
		    APR_PRINTF( PD(filp), "ERROR:SetInternalClock <freq> [DOx] not %s\n",
				argv[iarg-1]    );
		    return iarg_orig;
		}
	    }
            ierrno = PDRVR( filp )->SetInternalClock( PD(filp) );
  
            if ( ierrno < 0 ){
		ACQ32_PATH_READBUFFER_PRINTF( 
		    PD(filp), 
		    "ERROR:SetInternalClock failed %d\n", ierrno );
            }
        }
        return iarg;    // arg has been consumed
    }
  
    return iarg_orig;  // no args consumed
}

static int 
do_SetExternalClock_command(
    struct file* filp,
    char** argv,
    int argc,
    int iarg
    )
{
    const char* verb = argv[iarg];
    int iarg_orig = iarg;

    int ierrno;
    int divisor = 1;
  
    HELP_PRINTF_RET( MC_SET_EXTERNAL_CLOCK " DIx [<div> DOx]", 
		     "select ext clock" );  
    
    if ( STREQ( argv[iarg], MC_SET_EXTERNAL_CLOCK ) ){
        if ( ++iarg < argc ){
		        
	    int diX = lkupDIx( argv[iarg++] );

            if ( diX != -1 ){
		PDEV(filp)->m_dpd.clock.diX = diX;
	    }else{
		APR_PRINTF( PD(filp), "ERROR:SetExternalClock needs DIx\n" );
		return iarg_orig;
	    }    
            PDEV(filp)->m_dpd.clock.internal = 0;
            PDEV(filp)->m_dpd.clock.doX = ACQ32_DIO_NONE;

	    // optional divisor arg
	    if ( iarg < argc ){
		
		divisor = strtol( argv[iarg++], 0, 0 );

            // optional master arg
		if ( iarg < argc ){
		    static struct KeyLut _lut[] = {

			{ ACQ32_DO_0, DO(0) },
			{ ACQ32_DO_1, DO(1) },
			{ ACQ32_DO_2, DO(2) },
			{ ACQ32_DO_3, DO(3) },
			{ ACQ32_DO_4, DO(4) }
		    };
#define NLUT (sizeof(_lut)/sizeof(struct KeyLut))
		        
		    int doX = lkupKey( argv[iarg++], _lut, NLUT );
		
		    if ( doX != -1 ){
			PDEV(filp)->m_dpd.clock.doX = doX;
		    }else{
			APR_PRINTF( PD(filp),"ERROR:SetExtClock expecting DOx\n");
			return iarg_orig;
		    }
		}
	    }
	    PDEV(filp)->m_dpd.clock.divisor = divisor;
            ierrno = PDRVR( filp )->SetExternalClock( PD(filp) );
  
            if ( ierrno < 0 ){
		ACQ32_PATH_READBUFFER_PRINTF( 
		    PD(filp), 
		    "ERROR:SetInternalClock failed %d\n", ierrno );
            }
        }
        return iarg;    // arg has been consumed
    }
  
    return iarg_orig;  // no args consumed
}

static int do_SetUser (
    struct file* filp,
    char** argv,
    int argc,
    int iarg
    )
{
    const char* verb = argv[iarg];
    int iarg_orig = iarg;


    HELP_PRINTF_RET( 
        MC_SET_USER_LED " {3|4} " 
	MC_SET_USER_LED_ON " " 
	MC_SET_USER_LED_OFF " "
	MC_SET_USER_LED_FLASH " ", "acq32cpci only" );    
	    
    if ( STREQ( argv[iarg], MC_SET_USER_LED ) ){
        if ( ++iarg < argc && iarg+1 < argc ){

            int ierrno;

            int led_num = strtol( argv[iarg++], 0, 0 );
	    const char* smode = argv[iarg++];
	    int imode;
		
            static struct KeyLut _lut[] = {
                { UL_ON,    MC_SET_USER_LED_ON        },
                { UL_OFF,   MC_SET_USER_LED_OFF       },
                { UL_FLASH, MC_SET_USER_LED_FLASH      },
            };
#define NLUT (sizeof(_lut)/sizeof(struct KeyLut))
 		
		    
	    if ( !(led_num == 3 || led_num == 4) ){
		APR_DPRINTF( PD(filp), "setUserLed bad lednum %d\n", led_num );
		return iarg_orig;
	    }
			
            imode = lkupKey( smode, _lut, NLUT );
    
            if ( imode == - 1 ){
		APR_PRINTF( PD(filp), "ERROR: bad mode %s\n", smode );
                return iarg;
	    }
		
            ierrno = PDRVR( filp )->SetUser( PD(filp), led_num, imode );
  
            if ( ierrno < 0 ){
		ACQ32_PATH_READBUFFER_PRINTF( 
		    PD(filp), 
		    "ERROR:SetUser failed %d\n", ierrno 
		    );
            }
        }
        return iarg;    // arg has been consumed
    }
  
    return iarg_orig;  // no args consumed
#undef NLUT    
}





static int
do_GetChannelMask(
    struct file* filp,
    char** argv,
    int argc,
    int iarg
    )
{
    const char* verb = argv[iarg];
    int iarg_orig = iarg;

    HELP_PRINTF_RET( MC_GET_CHANNEL_MASK, "" );
    
    if ( STREQ( verb, MC_GET_CHANNEL_MASK ) ){

        char buf[128];

        int ichannel;
        int imask = 0x1;

	if (PDEV(filp)->dev_type == DEV_TYPE_ACQ196){
		buf[0] = '\0';
		for ( ichannel = 0; ichannel != 3; ++ichannel, imask <<= 1 ){
			strcat(buf, 
				PDEV(filp)->m_dpd.channel_mask&imask? 
/*                             '12345678901234567890123456789012' */
			       "11111111111111111111111111111111": 
			       "00000000000000000000000000000000");
		}
	}else{
		for ( ichannel = 0; ichannel != 32; ++ichannel, imask <<= 1 ){
			buf[ichannel] = 
				PDEV(filp)->m_dpd.channel_mask&imask? '1': '0';
		}
		buf[ichannel] = '\0';
	}

        ACQ32_PATH_READBUFFER_PRINTF( 
	    PD( filp ),
	    MC_GET_CHANNEL_MASK "=%s", buf );

        return iarg + 1;
    }else{
        return iarg_orig;
    }
}

static int 
do_GetInternalClock_command(
    struct file* filp,
    char** argv,
    int argc,
    int iarg
    )
{
    const char* verb = argv[iarg];
    int iarg_orig = iarg;

    HELP_PRINTF_RET( MC_GET_INTERNAL_CLOCK, "" );
    
    if ( STREQ( verb, MC_GET_INTERNAL_CLOCK ) ){

	ACQ32_PATH_READBUFFER_PRINTF(
	    PD(filp),
	    "%s=%d", MC_GET_INTERNAL_CLOCK, 
	    PDEV(filp)->m_dpd.clock.internal
	    );

        return iarg + 1;
    }else{
        return iarg_orig;
    }
}



static int 
do_SetSampleTagging_command(
    struct file* filp,
    char** argv,
    int argc,
    int iarg
    )
{
    const char* verb = argv[iarg];
    int iarg_orig = iarg;
  
    if ( STREQ( verb, MC_SET_SAMPLE_TAGGING ) ){
        if ( ++iarg < argc ){

            int ierrno;

            PDEV(filp)->m_dpd.sample_tagging = ATOI( argv[iarg++] );

	    PDEBUGL(2)( "Sample tagging %d\n",  
			PDEV(filp)->m_dpd.sample_tagging );
            ierrno = PDRVR( filp )->SetSampleTagging( PD(filp) );

            if ( ierrno < 0 ){
		APR_PRINTF( PD(filp), 
			    "ERROR:SetSampleTagging failed %d\n", ierrno );
            }
        }
        return iarg;    // arg has been consumed
    }
  
    return iarg_orig;  // no args consumed
}

static int build_DIO( struct Acq32Device* device, char string[] )
    // builds DIO output mask from string
{
    int imask = 0x1;
    int ichannel;
    
    for ( ichannel = 0; 
	  ichannel != 32 && string[ichannel] != '\0'; 
	  ++ichannel, imask <<= 1                       ){
	switch( string[ichannel] ){
	case DIO_MASK_INPUT:
	    device->m_dpd.dio_dir &= ~imask;
	    device->m_dpd.dio_outputs &= ~imask; // for tidyness
	    break;
	case DIO_MASK_OUTPUT1:
	    device->m_dpd.dio_dir |= imask;
	    device->m_dpd.dio_outputs |= imask;
	    break;
	case DIO_MASK_OUTPUT0:
	    device->m_dpd.dio_dir |= imask;
	    device->m_dpd.dio_outputs &= ~imask;
	    break;
	default:
	    return -1;
	}
    }

    return 0;
}


static int 
do_SetDIO_command(
    struct file* filp,
    char** argv,
    int argc,
    int iarg
    )
{
    const char* verb = argv[iarg];
    int iarg_orig = iarg;

    HELP_PRINTF_RET( SC_SET_DIO " ddddd ", "d = [-01]" );
    
    if ( STREQ( verb, SC_SET_DIO ) ){
	if ( ++iarg < argc ){
	 
	    int ierrno = build_DIO( PDEV(filp), argv[iarg++] );

	    if ( ierrno < 0 ){
		ACQ32_PATH_READBUFFER_PRINTF( 
		    PD(filp), 
		    "ERROR:SetDIO at failed at build_DIO(): %d\n", 
		    ierrno 
		    );
	    }else{
		int ierrno = PDRVR(filp)->SetDIO( PD(filp) );

		if ( ierrno < 0 ){
		    ACQ32_PATH_READBUFFER_PRINTF( 
			PD(filp), 
			"ERROR:SetDIO failed: %d\n",
			ierrno );
		}
	    }
	}
	return iarg;
    }

    return iarg_orig;
}


static int build_DIO_string( struct Acq32Path* path )
{
#define ADD_SPACE (*pstr++ = ' ')
#define ADD_EOS   (*pstr++ = '\0')

    struct Acq32Device* device = path->device;
    char loc_str[128];
    char* pstr = loc_str;

    int ichannel;
#define INPUT( bit )   ((device->m_dpd.dio_inputs&1<<(bit)) != 0)
#define OUTPUT( bit )  ((device->m_dpd.dio_outputs&1<<(bit)) != 0)
#define OUTSEL( bit )  ((device->m_dpd.dio_dir&1<<(bit)) != 0)

/* first the current input state */
    for ( ichannel = 0; ichannel != 16; ++ichannel ){
	*pstr++ = INPUT( ichannel )?  DIO_MASK_INPUT1: DIO_MASK_INPUT0;
    }
    
    ADD_SPACE;
/* next the outputs we asked for */
    for ( ichannel = 0; ichannel != 16; ++ichannel ){
	*pstr++ = 
	    OUTSEL(ichannel)&&OUTPUT(ichannel)? DIO_MASK_OUTPUT1: 
	    OUTSEL(ichannel)? DIO_MASK_OUTPUT0: DIO_MASK_INPUT;
    }
    ADD_SPACE;

#define ADD_CT_ACTIVE( bit ) \
    (*pstr++ = INPUT(bit)? DIO_CT_ACTIVE: DIO_CT_INACTIVE)

/* finally, the AIC, AOC, AOT, DOC, DOT states */
    ADD_CT_ACTIVE( 28 ); // AI CLK
    ADD_SPACE;
    ADD_CT_ACTIVE( 24 ); // AO CLK
    ADD_CT_ACTIVE( 25 ); // AO TRG
    ADD_SPACE;
    ADD_CT_ACTIVE( 20 ); // DO CLK
    ADD_CT_ACTIVE( 21 ); // DO TRG
    ADD_EOS;

    APR_PRINTF( path, "%s=%s", SC_GET_DIO, loc_str );
    return 0;
}

static int 
do_GetDIO_command(
    struct file* filp,
    char** argv,
    int argc,
    int iarg
    )
{
    const char* verb = argv[iarg];
    int iarg_orig = iarg;

    HELP_PRINTF_RET( SC_GET_DIO, "" );
    
    if ( STREQ( argv[iarg], SC_GET_DIO ) ){

        int ierrno = PDRVR(filp)->GetDIO( PD(filp) );

        if ( ierrno < 0 ){
            APR_PRINTF( PD(filp), "ERROR:GetDIO() failed: %d\n", ierrno );
        }else{
	    build_DIO_string( PD(filp) );
        }
        return iarg + 1;
    }

    return iarg_orig;
}

static int channel_lookup( int ikey, int nvalues, int* the_values )
{
    int ivalue;

    for ( ivalue = 0; ivalue != nvalues; ++ivalue ){
        if ( ikey == the_values[ivalue] ){
	    return ivalue;
        }
    }

    return 0;   // safe default
}


static int do_SetChannel_command(
    struct file* filp,    
    char** argv,    
    int argc,    
    int iarg
    )
    // command format is setChannel <channel> <g1> <g2> <exoutputte>

{
    int iarg_orig = iarg;

    static int G1[] = { 1, 10, 100, 1000 };
#define NG1 4
    static int G2[] = { 1, 2, 4, 8 };
#define NG2 4
    static int V_EX[] = { 1, 2, 5, 10 };
#define NV_EX 4

    if ( STREQ( argv[iarg], SC_SET_CHANNEL ) ){
	if ( iarg+4 < argc ){
	    int ichannel = ATOI( argv[++iarg] );
	    int g1       = ATOI( argv[++iarg] );
	    int g2       = ATOI( argv[++iarg] );
	    int v_ex     = ATOI( argv[++iarg] );

	    int ierrno = PDRVR(filp)->SetChannel( 
		PD(filp),
		CLAMP( ichannel, 0, 31 ), 
		channel_lookup( g1, NG1, G1 ), 
		channel_lookup( g2, NG2, G2 ),
		channel_lookup( v_ex, NV_EX, V_EX )
		);
	    if ( ierrno < 0 ){
	        ACQ32_PATH_READBUFFER_PRINTF(
		    PD(filp),
		    "ERROR:SetChannel() failed %d\n",
		    ierrno );
	    }

	    ++iarg;
	}else{
	    ACQ32_PATH_READBUFFER_PRINTF( 
	        PD(filp),
		"ERROR:" SC_SET_CHANNEL "needs 4 args\n" );
        }
	return iarg;
    }

    return iarg_orig;
}


static int do_SetCal(
    struct file* filp,
    char** argv,
    int argc,
    int iarg
    )
{
    const char* verb = argv[iarg];
    int iarg_orig = iarg;

  
    if ( STREQ( verb, MC_SET_CAL ) ){
        if ( ++iarg < argc ){
	    int ical = ATOI( argv[iarg] );

            int ierrno = PDRVR( filp )->SetCal( PD(filp), ical );

            if ( ierrno < 0 ){
		APR_PRINTF( PD(filp), 
			    "ERROR:SetCal failed %d\n", ierrno );
            }
	    return ++iarg;    // arg has been consumed
        }else{
	    APR_PRINTF( PD(filp), "ERROR:ReserveAO needs <length>\n" );
	    return iarg;
	}
    }
    
    return iarg_orig;
}

static int do_WaitState(
    struct file* filp,
    char** argv,
    int argc,
    int iarg
    )
{
    const char* verb = argv[iarg];
    int iarg_orig = iarg;
    int timeout = 1000;
  
    if ( STREQ( verb, MC_WAIT_STATE ) ){
        if ( ++iarg < argc ){
	    timeout = ATOI( argv[iarg] );
	}
#ifdef PGMCOMOUT
// WORKTODO - apr buffer NEVER empty ...
	if ( 1 || acq32_path_readbuffer_is_empty( PD(filp) ) ){
	    int timeleft;

	    PDEBUGL(2)( "do_WaitState sleep %d\n", timeout );

	    timeleft = interruptible_sleep_on_timeout( 
		&PD(filp)->return_waitq, timeout );

	    PDEBUGL(2)( "do_WaitState awake %d\n", timeleft );

	    if ( timeleft == 0 ){
		struct timeval tv;
	    
		do_gettimeofday( &tv );
		acq32_doGetStateWork( filp, &tv );
		APR_PRINTF( PD(filp), " (TIMEOUT)" );
	    }
	}
#endif

        return ++iarg;    // arg has been consumed
    }
    
    return iarg_orig;
}


// Gut commands have this form
// <command> <FX> [PH|EV] <values>

struct GutDecode {
    const char* command;
       
    enum FX  fx;

    enum SUBJECT subject;
	
    int wants_args;
	
    int match;
    int error;
};

#define FX_ANY FX_NONE
#define NO_SUBJECT_REQUESTED NULL_SUBJECT
#define WANTS_ARGS 1

struct LKUP { const char* key; int value; };

static int lkup( const struct LKUP lut[], int nlut, const char* key )
{
    int ii;
    
    for ( ii = 0; ii != nlut; ++ii ){
        if ( STREQ( lut[ii].key, key ) ){
	    return lut[ii].value;
	}
    }
    
    return -1;
}
static const char* rlkup( const struct LKUP lut[], int nlut, int value ){
    int ii;

    for ( ii = 0; ii != nlut; ++ii ){
	if ( lut[ii].value == value ){
	    return lut[ii].key;
	}
    }
    
    return NULL;
}
static int  gutDecode (
    struct file* filp,    
    char** argv,    
    int argc,    
    int iarg,
    struct GutDecode *gd
    )
{
    int iarg_orig = iarg;
    
    static struct LKUP fx_lut[] = {
        { GUT_FXN_AO, FX_AO },
	{ GUT_FXN_AI, FX_AI },
	{ GUT_FXN_DO, FX_DO }
    };
#define NFX (sizeof(fx_lut)/sizeof(struct LKUP))
    static struct LKUP subj[] = {
        { GUT_PHASE(1), PH_1 },
	{ GUT_PHASE(2), PH_2 },
	{ GUT_EVENT(1), EV_1 },
	{ GUT_EVENT(2), EV_2 },
	{ GUT_EVENT(3), EV_3 }
    };
#define NSUBJ (sizeof(subj)/sizeof(struct LKUP))    

    int subject;
    
    gd->error = gd->match = 0;
    
    if ( STREQ( argv[iarg], gd->command ) ){
        gd->match = 1;
	gd->error = 1;
	    
        if ( ++iarg < argc ){
	    if ( (gd->fx = lkup( fx_lut, NFX, argv[iarg] )) != -1 ){
		if ( gd->subject != NO_SUBJECT_REQUESTED ){
                    if ( ++iarg < argc ){
			if ( (subject=lkup(subj,NSUBJ,argv[iarg])) != -1 ){
			    if ( (gd->subject&subject) != 0 ){					    
					    
				gd->subject = subject;
						    
				if ( gd->wants_args ){
				    if ( ++iarg < argc ){
					gd->error = 0;
					return iarg;
				    }else{
					APR_PRINTF( PD(filp), 
						    "ERROR:wants args\n" );
					return argc;
				    }
				}else{
				    gd->error = 0;
				    return iarg;
				}
			    }else{
				APR_PRINTF( PD(filp), 
					    "ERROR: wrong subject\n" );
				return argc;
			    }
			}else{
			    APR_PRINTF( PD(filp),
					"ERROR:subject not recognised\n" );
			}
		    }else{
			APR_PRINTF( PD(filp),
				    "ERROR:please supply a subject\n" );
		    }
		}else{
		    if ( gd->wants_args ){
			if ( ++iarg < argc ){
			    gd->error = 0;
			    return iarg;
			}else{
			    APR_PRINTF( PD(filp),
					"ERROR:wants args\n" );
			}
		    }else{
			return iarg;
		    }
		}
	    }else{
		APR_PRINTF( PD(filp),
			    "ERROR:fx must be one of AO, AI, DO\n" );
		return argc;
	    }
	}else{
	    APR_PRINTF( PD(filp), "ERROR:please supply fx\n" );   
	    return argc;
	}
    }
    
    return iarg_orig;
}

static const char* strGetFx( int fx ) {
    return fx==FX_AI? "AI": 
	   fx==FX_AO? "AO": 
	   fx==FX_DO? "DO": "no function";
}
static const char* strGetSubject( int subject ) {
    return subject==NO_SUBJECT_REQUESTED? "no subject requested":
	   subject==PH_1? "P1":
	   subject==PH_2? "P2":
	   subject==EV_1? "E1":
	   subject==EV_2? "E2":
	subject==EV_3? "E3": "no subject";
}

static void dumpDecodeSuccess( 
    struct file* filp, struct GutDecode* gd, const char* arg1 )
{
    PDEBUGL(1)( "command: %s fxn: %s subject:%s (0x%04x) arg1:%s OK\n",
                gd->command, 
		strGetFx( gd->fx ),
		strGetSubject( gd->subject ),
		gd->subject,
		arg1
	);			
}

#define DUMP_DECODE_SUCCESS dumpDecodeSuccess

static int  do_SetPhase (
    struct file* filp,    
    char** argv,    
    int argc,    
    int iarg
    )
{
    const char* verb = argv[iarg];

    int ierrno;
    struct GutDecode gd = { GUT_SETPHASE, FX_ANY, PH_REQUEST, WANTS_ARGS };
    int ilast;

    HELP_PRINTF_RET( GUT_SETPHASE " f p n", "set phase samples" );
	                
    ilast = gutDecode( filp, argv, argc, iarg, &gd );
    
    if ( gd.match && !gd.error ){
        DUMP_DECODE_SUCCESS( filp, &gd, argv[ilast] );

        ierrno = PDRVR(filp)->SetPhase(
	    PD(filp), gd.fx, gd.subject, ATOI(argv[ilast]) );
			
        if ( ierrno < 0 ){
	    APR_PRINTF( PD(filp), "ERROR:SetPhase failed %d\n", ierrno );
	}	    	
	return ++ilast;
    }
    
    return ilast;
}    
    
static int do_GetPhase (
    struct file* filp,    
    char** argv,    
    int argc,    
    int iarg
    )
{
    const char* verb = argv[iarg];
    int ierrno;
    struct GutDecode gd = { GUT_GETPHASE, FX_ANY, PH_REQUEST, WANTS_ARGS };    
    int ilast;
    
    HELP_PRINTF_RET( GUT_GETPHASE " f p property", "get property" );
    
    ilast = gutDecode( filp, argv, argc, iarg, &gd );

    
    if ( gd.match && !gd.error ){

        static struct LKUP props[] = {
	    { GUT_GETPHASE_KEY_REQUESTED_SAMPLES, PP_REQUESTED_SAMPLES },
	    { GUT_GETPHASE_KEY_ACTUAL_SAMPLES,    PP_ACTUAL_SAMPLES },
	    { GUT_GETPHASE_KEY_STATE,             PP_STATE }
	};
#define NPROPS (sizeof(props)/sizeof(struct LKUP))
        enum PHASE_PROPERTY pp;

        if ( (pp = lkup( props, NPROPS, argv[ilast] )) != -1 ){
            DUMP_DECODE_SUCCESS( filp, &gd, argv[ilast] );
	    ierrno = PDRVR(filp)->GetPhase( 
		PD(filp), gd.fx, gd.subject, pp );
	
	    if ( ierrno < 0 ){
		APR_PRINTF( PD(filp), "ERROR:GetPhase failed %d\n", ierrno );
	    }else{
		APR_PRINTF( PD(filp), "%d", ierrno ); // WORKTODO!! DODGY
	    }
			
	    return ++ilast;
	}else{
	    APR_PRINTF( PD(filp), "ERROR:getPhase property not recognised %s\n",
			argv[ilast] );
    	    return ++ilast;
	}
    }
    
    return ilast;
}    
    
static struct LKUP ec_lut[] = {
        { EV_TRUE,                  EC_TRUE             },
	{ EV_SOFT,                  EC_SOFT             },
	{ EV_TRIGGER_RISING,        EC_TRIGGER_RISING   },
	{ EV_TRIGGER_FALLING,       EC_TRIGGER_FALLING  },
	{ EV_NONE,                  EC_NONE             },
	{ EV_DATA_EXCEEDS,          EC_DATA_ABOVE     },
	{ EV_DATA_BELOW,            EC_DATA_BELOW       }
};
#define NEC (sizeof(ec_lut)/sizeof(struct LKUP))

static int decodeEventCondition( 
    struct file* filp,
    char** argv, 
    int argc, 
    int iarg, 
    struct EventConditionDef* ecd 
    )
// build ec from remaining args. return last iarg on success
{
    enum EVENT_CONDITION ec = 0;


    ecd->error = 1;    // guilty until proved innocent

    if ( (ec = lkup( ec_lut, NEC, argv[iarg] )) != -1 ){
        iarg++;
        switch( ecd->ec = ec ){
	case EC_TRIGGER_RISING:
	case EC_TRIGGER_FALLING: 
		    
// look for multiple prams DIx DIy .... {x,y:0..5}
		    
	    if ( iarg < argc ){
			
		ecd->prams.trigger_bit = 0;
				
		while( iarg < argc ){
		    char* di_token = argv[iarg++];
				
		    if ( strlen( di_token ) == 3 && strstr( di_token, "DI" ) && 
			 IN_RANGE( di_token[2], '0', '5' ) ){
			ecd->prams.trigger_bit |= 1 << (di_token[2]-'0');
		    }else{
			APR_PRINTF( PD(filp), "ERROR DI def should be DI[0-5]\n" );
			return argc;
		    }
		}
	    }else{
		APR_PRINTF( PD(filp), "ERROR needs DI bit\n" ); 
		return argc;
	    }
	    break;   
	case EC_DATA_ABOVE:
	case EC_DATA_BELOW:
	    if ( iarg+1 < argc ){
		ecd->prams.th.channel = ATOI( argv[iarg++] );
		ecd->prams.th.below = ec==EC_DATA_BELOW? 
		    ATOI( argv[iarg++] ): 0x8000;
		ecd->prams.th.above = ec==EC_DATA_ABOVE? 
		    ATOI( argv[iarg++] ): 0x7fff;
	    }else{
		APR_PRINTF( PD(filp), "ERROR wants ch th\n" );
		return argc;
	    }
	    break;
	default:
	    break;
	}
	ecd->error = 0;
	return iarg;
    }else{
	APR_PRINTF( 
	    PD(filp), "ERROR: failed to decode EVENT CONDITION %s\n",
	    argv[iarg] );
	return iarg;
    }    
}

static int do_SetEvent  (
    struct file* filp,    
    char** argv,    
    int argc,    
    int iarg
    )
{
    const char* verb = argv[iarg];
    int ierrno;
    struct GutDecode gd = { GUT_SETEVENT, FX_ANY, EV_REQUEST, WANTS_ARGS };
    struct EventConditionDef ecd;
    int ilast;
    
    HELP_PRINTF_RET( GUT_SETEVENT "f E p", "set event property" );     
    
    ilast = gutDecode( filp, argv, argc, iarg, &gd );
    
    if ( gd.match && !gd.error ){
    
        decodeEventCondition( filp, argv, argc, ilast, &ecd );
	
	if ( ecd.error == 0 ){
	    
            DUMP_DECODE_SUCCESS( filp, &gd, argv[ilast] );
	
            ierrno = PDRVR(filp)->SetEvent( PD(filp), gd.fx, gd.subject, &ecd );
	}
	return argc;
    }
    
    return ilast;
}    
    

static int do_GetEvent  (
    struct file* filp,    
    char** argv,    
    int argc,    
    int iarg
    )
{
    const char* verb = argv[iarg];
    struct GutDecode gd = { GUT_GETEVENT, FX_ANY, EV_REQUEST };
    int ilast;
    int ierrno;
    
    HELP_PRINTF_RET( GUT_GETEVENT " f E", "get event property" );
    
    ilast = gutDecode( filp, argv, argc, iarg, &gd );
    
    if ( gd.match && !gd.error ){
        DUMP_DECODE_SUCCESS( filp, &gd, argv[ilast] );

        ierrno = PDRVR(filp)->GetEvent( PD(filp), gd.fx, gd.subject );

	if ( ierrno < 0 ){
	    APR_PRINTF( PD(filp), "ERROR: GetEvent Failed\n" );
	}else{
	    static const char* DIVALUES[6] = {
		"DI0", "DI1", "DI2", "DI3", "DI4", "DI5"
	    };
	    unsigned ev_type = ierrno>>8;
	    unsigned ev_value= ierrno&0xff;

	    const char* ev_typestr = rlkup( ec_lut, NEC, ev_type );
	    const char* ev_valuestr = "";

	    if ( ev_typestr == NULL ){
		ev_typestr = "error";
	    }
	    switch( ev_type ){
	    case EC_TRIGGER_RISING:
	    case EC_TRIGGER_FALLING: {
		unsigned tbit = 1;
		int ilut;
		
		for ( ilut = 0; ilut != 6; ++ilut ){
		    if ( tbit&ev_value ){
			ev_valuestr = DIVALUES[ilut];
		    }
		}
	    }
	    default:
		;
	    }
	    APR_PRINTF( PD(filp), "getEvent %s %s %s %s\n",
			strGetFx( gd.fx ),
			strGetSubject( gd.subject ),
			ev_typestr, ev_valuestr      );
	}
			
	// WORKTODO ... do the action
	return argc;
    }
    
    return ilast;
}    

#undef NEC

static int do_GetAvailableChannels  (
    struct file* filp,    
    char** argv,    
    int argc,    
    int iarg
    )
{
    const char* verb = argv[iarg];

    HELP_PRINTF_RET( MC_GET_MAX_CHANNELS, "" );
    
    if ( STREQ( argv[iarg], MC_GET_MAX_CHANNELS ) ){

        int ierrno = PDRVR(filp)->GetAvailableChannels( PD(filp) );

	if ( ierrno < 0 ){
	    APR_PRINTF( PD(filp), 
			"ERROR:" MC_GET_MAX_CHANNELS " failed %d\n", ierrno );
	}else{
		
	    APR_PRINTF(
		PD(filp),
		MC_GET_MAX_CHANNELS " AI=%d AO=%d", 
		PDEV(filp)->input_channels.nchannels,
		PDEV(filp)->output_channels.nchannels
	        );
        }
        return iarg + 1;
    }
    return iarg;
}    

static int do_GetRange  (
    struct file* filp,    
    char** argv,    
    int argc,    
    int iarg
    )
{
    const char* verb = argv[iarg];

    HELP_PRINTF_RET( MC_GET_VRANGE, "" );
    
    if ( STREQ( verb, MC_GET_VRANGE ) ){

        int ierrno = PDRVR(filp)->GetRange( PD(filp) );
	struct CHANNEL_INFO* input = &PDEV(filp)->input_channels;
	struct CHANNEL_INFO* output= &PDEV(filp)->output_channels;

	if ( ierrno < 0 ){
	    APR_PRINTF( PD(filp), 
			"ERROR:" MC_GET_VRANGE " failed %d\n", ierrno );
	}else{		
    	    APR_PRINTF(
		PD(filp),
		MC_GET_VRANGE " AI=%d.%dV,%d.%dV AO=%d.%dV,%d.%dV", 
		input->vmin10/10,  ABS(input->vmin10%10),
		input->vmax10/10,  ABS(input->vmax10%10),
		output->vmin10/10, ABS(output->vmin10%10),
		output->vmax10/10, ABS(output->vmax10%10)		    
	        );
        }            

        return iarg + 1;
    }
    return iarg;

}    
    
    

static int do_FireEvent  (
    struct file* filp,    
    char** argv,    
    int argc,    
    int iarg
    )
{
    const char* verb = argv[iarg];
    struct GutDecode gd = { GUT_FIRE_EVENT, FX_ANY, EV_REQUEST };
    int ilast;
    
    HELP_PRINTF_RET( GUT_FIRE_EVENT " f E", "" );
    
    ilast = gutDecode( filp, argv, argc, iarg, &gd );
    
    if ( gd.match && !gd.error ){
        int ierrno;
        DUMP_DECODE_SUCCESS( filp, &gd, argv[ilast] );
    	
        ierrno = PDRVR(filp)->FireEvent( PD(filp), gd.fx, gd.subject );
			
        if ( ierrno < 0 ){
	    APR_PRINTF( PD(filp), "ERROR:FireEvent failed %d\n", ierrno );
	}	    	
	    
	return argc;
    }
    
    return ilast;
}    
    

static int do_SetClock  (
    struct file* filp,    
    char** argv,    
    int argc,    
    int iarg
    )
{
    const char* verb = argv[iarg];

    struct GutDecode gd = { 
        GUT_SET_CLOCK, FX_ANY, NO_SUBJECT_REQUESTED, WANTS_ARGS 
    };
    static struct LKUP cs_lut[] = {
        { DI(0),               CS_DI0             },
        { DI(1),               CS_DI1             },
        { DI(2),               CS_DI2             },
        { DI(3),               CS_DI3             },
        { DI(4),               CS_DI4             },
        { DI(5),               CS_DI5             },					
	{ SOFT_CLOCK,          CS_SOFT_CLOCK      },
	{ INTERNAL_CLOCK,      CS_INT_CLOCK       }
    };
#define NCS (sizeof(cs_lut)/sizeof(struct LKUP))
    
    int ilast;
    
    HELP_PRINTF_RET( GUT_SET_CLOCK " f DIx", "select fxn clock source" );
     
    ilast = gutDecode( filp, argv, argc, iarg, &gd );
    
    if ( gd.match && !gd.error ){
        int cs;
	
        DUMP_DECODE_SUCCESS( filp, &gd, argv[ilast] );

        if ( (cs = lkup( cs_lut, NCS, argv[ilast] )) != -1 ){
            int ierrno = PDRVR(filp)->SetClock( PD(filp), gd.fx, cs );
			
            if ( ierrno < 0 ){
		APR_PRINTF( PD(filp), "ERROR:SetClock failed %d\n", ierrno );
	    }	    	
	    return ++ilast;
	}else{
	    APR_PRINTF( PD(filp), 
			"ERROR:SetClock failed, unknown subject %s\n", 
			argv[ilast] );
	}
	// WORKTODO ... do the action
	return argc;
    }
    
    return ilast;
}    

    
static int 
do_resetGUT(
    struct file* filp,
    char** argv,
    int argc,
    int iarg
    )
{
    const char* verb = argv[iarg];
    int iarg_orig = iarg;
  
    HELP_PRINTF_RET( GUT_RESET, "" );
    
    if ( STREQ( argv[iarg++], GUT_RESET ) ){

        int ierrno;

        ierrno = PDRVR( filp )->ResetGut( PD(filp) );

        if ( ierrno < 0 ){
    	    APR_PRINTF( PD(filp), "ERROR:resetGUT failed %d\n", ierrno );
        }
        return iarg;    // arg has been consumed
    }
  
    return iarg_orig;  // no args consumed
}
	    
static int do_ClockNow(
    struct file* filp,    
    char** argv,    
    int argc,    
    int iarg
    )
// WORKTODO - what exactly is this ??
{
    const char* verb = argv[iarg];
    int iarg_orig = iarg;

    if ( STREQ( verb, GUT_FIRE_CLOCK ) ){
        int ierrno = PDRVR( filp)->ClockNow( PD(filp) );
	
	if ( ierrno < 0 ){
	    APR_PRINTF( PD(filp), "ERROR:ClockNow failed %d\n", ierrno );
	}
        return ++iarg;
    }else{
	return iarg_orig;
    }
}    


static int do_ReserveAO(
    struct file* filp,
    char** argv,
    int argc,
    int iarg
    )
{
    const char* verb = argv[iarg];
    int iarg_orig = iarg;

    HELP_PRINTF_RET( MC_RESERVE_AO " nsamples", "" );
    
    if ( STREQ( verb, MC_RESERVE_AO ) ){
        if ( ++iarg < argc ){
	    int nreserve = ATOI( argv[iarg] );

            int ierrno = PDRVR( filp )->ReserveAO( PD(filp), nreserve );

            if ( ierrno < 0 ){
		APR_PRINTF( PD(filp), 
			    "ERROR:ReserveAO failed %d\n", ierrno );
            }
	    return ++iarg;    // arg has been consumed
        }else{
	    APR_PRINTF( PD(filp), "ERROR:ReserveAO needs <length>\n" );
	    return iarg;
	}
    }
    
    return iarg_orig;
}

//#include <ctype.h>


// WORKTODO - discriminate voltage range depending on board type.

#define NORMAL 1000

#define MAX16 32767
#define MIN16 -32768


int acq32_volts2bits( 
    struct Acq32Device* device, int volts, int vmin, int vmax )
// volts, vmin, vmax are all scaled V*NORMAL
{
    int bits = 0x0ffff * ( volts - vmin )/ (vmax - vmin );

    bits -= 32768;    // make it signed HACK
    PDEBUGL(2)( "volts2bits( device, %d, %d, %d ) bits %d\n", 
		volts, vmin, vmax, bits );
    
    if ( bits > MAX16 ) bits = MAX16;
    if ( bits < MIN16 ) bits = MIN16;
    
    PDEBUGL(2)( "volts2bits return %d 0x%04x\n", bits, bits );
    
    return bits;            
}

static int isdigit( char c )
{
    return IN_RANGE( c, '0', '9' );
}


// class IntBuf
struct INTBUF {
    char buf[20];
    int ibuf;
    int sign;
    int value;
};
		
static void intbufInit( struct INTBUF* _this ) {
    memset( _this, 0, sizeof(struct INTBUF) );
    _this->sign = 1;
		    
    PDEBUGL(2)( "intbufInit() done\n" );
}

static void intbufAppend( struct INTBUF* _this, char c ) {
	
    PDEBUGL(2)( "intbufAppend() start\n" );
		
    if ( _this->ibuf < 5 ){
	_this->buf[_this->ibuf++] = c;
	PDEBUGL(2)( "intbufAppend() [%d] = %c \"%s\"\n",
		    _this->ibuf-1, c,  _this->buf   );
    }
			
    PDEBUGL(2)( "intbufAppend() 99\n" );
}

static void intbufSetSign( struct INTBUF* _this, int sign ) {
    _this->sign = sign;
}	    
		
static int intbufGetValue( struct INTBUF* _this ) {
    return _this->sign*STRTOL( _this->buf );
}
		
static void intbufNormalise( struct INTBUF* _this, int normal ) {
    int decades = 0;
    int normal2;
		
    for ( decades = 0, normal2 = normal; normal2>=10; normal2/=10, decades++ )
	;
		
    PDEBUGL(2)( "intbufNormalise() normal:%d decades:%d len:%d\n",
		normal, decades, strlen(_this->buf) );
			
    while( strlen( _this->buf ) < decades ){
	strcat( _this->buf, "0" );
    }	
    while( strlen( _this->buf ) > decades ){
	_this->buf[strlen(_this->buf)-1] = '\0';
    }
    PDEBUGL(2)( "intbufNormalise() normal:%d decades:%d len:%d (fixed)\n",
		normal, decades, strlen(_this->buf) );
		
}
// class IntBuf		
		
static int decode_threshold( 
    struct Acq32Device* device,
    const char* token, short* value )
{
    // WORKTODO ... decode volts here,reiinvent the wheel 
    // (no sscanf, floating point)
    if ( strchr( token, 'V' ) != 0 || strchr( token, 'v' ) != 0 ){
        
	struct INTBUF integer, fraction;
	const char* ptoken;
	enum { PROC_UNITS, PROC_FRACT, PROC_DONE } state = PROC_UNITS;
	    
	PDEBUGL(1)( "decode threshold V decode token %s\n", token );
	    
        intbufInit( &integer );
	intbufInit( &fraction );

	for ( ptoken = token; *ptoken != '\0'; ptoken++ ) {
	    
	    PDEBUGL(2)( "decode state:%d \"%s\" %d %d\n", 
			state, ptoken, integer.ibuf, fraction.ibuf );

	    switch( state ){
	    case PROC_UNITS:
		if ( integer.ibuf==0 && *ptoken=='-' ){
		    intbufSetSign( &integer, -1 );
		    intbufSetSign( &fraction, -1 );
		}else if ( !isdigit( *ptoken ) ){
		    if ( *ptoken == '.' ){
			state = PROC_FRACT;
		    }else{
			state = PROC_DONE;
		    }
		}else{
		    intbufAppend( &integer, *ptoken );
		}
		break;
	    case PROC_FRACT:
		if ( isdigit( *ptoken ) ){
		    intbufAppend( &fraction, *ptoken );
		}else{
		    state = PROC_DONE;
		}
		break;
	    case PROC_DONE:
	    default:
		goto proc_done;
	    }
	}

    proc_done:
        {
	    int normal;
		
            intbufNormalise( &fraction, NORMAL );    
	    // pad fraction to NORMAL size
	    
		
            PDEBUGL(1)( "decode threshold units %d fract %d\n", 
			intbufGetValue( &integer ), 
			intbufGetValue( &fraction) );
		
            normal = intbufGetValue( &integer )*NORMAL + 
		intbufGetValue( &fraction );
		     
            *value = acq32_volts2bits( 
		device, normal, 
		device->input_channels.vmin10*NORMAL/10, 
		device->input_channels.vmax10*NORMAL/10 );
	}
    }else{
        *value = STRTOL( token );
    }
    
    PDEBUG( "decode_threshold %ld %d\n", STRTOL(token), *value );
    return 0;
}

static int do_SetThreshold(
    struct file* filp,
    char** argv,
    int argc,
    int iarg
    )
{
    int iarg_orig = iarg;
    const char* verb = argv[iarg++];
    int ierrno = 0;

    HELP_PRINTF( MC_TH_CLEAR_ALL, "" );
    HELP_PRINTF( MC_TH_SELECT1, "" );
    HELP_PRINTF( MC_TH_SELECT, "" );
    HELP_PRINTF_RET( MC_TH_CHANNEL_SET, "" );
    
    if ( STREQ( verb, MC_TH_CLEAR_ALL ) ){
        ierrno = PDRVR(filp)->SetThreshold( PD(filp), THS_ALL, MAX16, MIN16 );
	
	if ( ierrno < 0 ){
	    APR_PRINTF( PD(filp), "ERROR: failed to clear all %d\n", ierrno );
	}else{
	    return iarg;
	}
    }else if ( STREQ(verb, MC_TH_SELECT1) || STREQ(verb, MC_TH_SELECT) ){
    
        short selector;
        static struct LKUP _lut[] = {
            { MC_TH_BILEVEL,         THS_ARM_DATA_SINGLE   },
	    { MC_TH_BILEVEL_MULTI,   THS_ARM_DATA_MULTI    },
	    { MC_TH_EDGE,            THS_ARM_EDGE_SINGLE   },
	    { MC_TH_EDGE_MULTI,      THS_ARM_EDGE_MULTI    },
	    { MC_TH_FAST_MULTI,      THS_ARM_FAST_MULTI    }  
        };
#define NEC (sizeof(_lut)/sizeof(struct LKUP))
    
        if ( iarg < argc ){
            if ( (selector = lkup( _lut, NEC, argv[iarg++] )) == -1	){
		APR_PRINTF( PD(filp), "ERROR:dont understand selector %s\n", argv[iarg-1] );
		return iarg_orig;
	    }
	}else{
	    selector = THS_ARM_DATA_SINGLE;
	}
        ierrno = PDRVR(filp)->SetThreshold( PD(filp), selector, 0, 0 );
	
	if ( ierrno < 0 ){
	    APR_PRINTF( PD(filp), "ERROR:failed to select threshold distributor %d\n",
			ierrno );
	}else{
	    return iarg;
	}
    }else if ( STREQ( verb, MC_TH_CHANNEL_SET ) ){
        if ( iarg+2 < argc ){
	    short ta, tb;
	    int cmax = PDEV(filp)->input_channels.nchannels;
	    int vrange = PDEV(filp)->input_channels.vmax10-
		PDEV(filp)->input_channels.vmin10;
	    int channel = STRTOL( argv[iarg++] );    // user spec 1..32
	    // NB driver spec 0..31

            if ( cmax == 0 ){
		APR_PRINTF( PD(filp), 
			    "ERROR: please try getAvailableChannels first\n" );
		return iarg;
	    }else if ( vrange == 0 ){
		APR_PRINTF( PD(filp), 
			    "ERROR: please try getVoltsRange first\n" );
		return iarg;
	    }else if ( !IN_RANGE( channel, 1, cmax ) ){
		APR_PRINTF( PD(filp), 
			    "ERROR: please specify channel in range %d .. %d\n",
			    1, cmax );
		return iarg;
	    }else if ( 
		(ierrno = decode_threshold( PDEV(filp), argv[iarg++], &ta )) ||
                (ierrno = decode_threshold( PDEV(filp), argv[iarg++], &tb )) ||
		(ierrno = PDRVR(filp)->SetThreshold(PD(filp),channel-1,ta,tb ))
		){
		APR_PRINTF( PD(filp), 
			    "ERROR: failed to set threshold %d\n", ierrno );
	    }else{
		return iarg;
	    }
	}else{
	    APR_PRINTF( PD(filp), 
			"ERROR: please declare <channel> <above> <below>\n" );
	}
    }
    
    return iarg_orig;
#undef NEC    
}    

struct FD_LUT {
    const char* func1;
    const char* func_alt;
    const char* dest[5];
};
struct FD_LUT sync_route_lut[] = {
    { DI(0), AICLK, { MI(0), MO(0), J5(0), PXI_TRIG(0), PXI_TRIG(6) } },
    { DI(1), AITRIG,{ MI(1), MO(1), J5(1), PXI_TRIG(1), PXI_TRIG(7) } },
    { DI(2), AOCLK, { MI(2), MO(2), J5(2), PXI_TRIG(2), PXI_STAR    } },
    { DI(3), AOTRIG,{ MI(3), MO(3), J5(3), PXI_TRIG(3), PXI_CLOCK10 } },
    { DI(4), DOCLK, { MI(4), MO(4), J3(4), PXI_TRIG(4) } },
    { DI(5), DOTRIG,{ MI(5), MO(5), J3(5), PXI_TRIG(5) } }
};
#define NKEYS (sizeof(sync_route_lut)/sizeof(struct FD_LUT))

static int do_SetSyncRoute_command(
    struct file* filp,    
    char** argv,    
    int argc,    
    int iarg
    )
    // command format is setChannel <channel> <g1> <g2> <exoutputte>

{
    const char* verb = argv[iarg];
    int iarg_orig = iarg;
    int funkey;

    HELP_PRINTF_RET( MC_SET_SYNC_ROUTE " DIx [dest1 .. dest2..]", "" );
    
    if ( !STREQ( argv[iarg], MC_SET_SYNC_ROUTE ) ){
        return iarg_orig;
    }
    if ( ++iarg >= argc ){
        ACQ32_PATH_READBUFFER_PRINTF( PD(filp), "ERROR:No function arg\n" );	        
	return iarg;
    }
    
// here with valid function    

    for ( funkey = 0; funkey != NKEYS; ++funkey ){
	
	char* func = argv[iarg];
	
	if ( STREQ( func, sync_route_lut[funkey].func1 )   ||
	     STREQ( func, sync_route_lut[funkey].func_alt )   ){

            int idest;
            unsigned dest_mask = 0;
            int gooddest = 0;
	    int ierrno;
			 			 
            for ( ++iarg; iarg < argc; ++iarg, gooddest = 0 ){
                for ( idest = 0; idest != 5; ++idest ){
                    if ( sync_route_lut[funkey].dest[idest] &&
			 STREQ(argv[iarg],sync_route_lut[funkey].dest[idest])){
                        dest_mask |= 1<<idest;
                        gooddest = 1;
                    }
                }
		
                if ( !gooddest ){
                    ACQ32_PATH_READBUFFER_PRINTF(
			PD(filp), 
			"ERROR:Incorrect dest arg %s\n", argv[iarg] );
                    return iarg;
                }
            }
			 
	    PDEV(filp)->sync_dest_masks[funkey] = dest_mask;
			
	    ierrno = PDRVR(filp)->SetSyncRoute( PD(filp), funkey, dest_mask );

            if ( ierrno < 0 ){
		ACQ32_PATH_READBUFFER_PRINTF( 
		    PD(filp), "ERROR:SetSyncRoute() failed: %d\n", ierrno );
	    }
	    return iarg;
	}
    }
	
    ACQ32_PATH_READBUFFER_PRINTF(
	PD(filp), "ERROR:Incorrect function arg %s\n", argv[iarg] );

    return iarg;
}

static int do_GetSyncRoute_command(
    struct file* filp,    
    char** argv,    
    int argc,    
    int iarg
    )
    // command format is setChannel <channel> <g1> <g2> <exoutputte>

{
    const char* verb = argv[iarg];
    int iarg_orig = iarg;

    int funkey;
    struct Acq32Device* pdev = PDEV(filp);
    
    HELP_PRINTF_RET( MC_GET_SYNC_ROUTE, "" );
    
    if ( !STREQ( argv[iarg++], MC_GET_SYNC_ROUTE ) ){
        return iarg_orig;
    }
    
// here with valid function    

#define DESTR( mask, ix, ibit ) \
   ((mask&(1<<ibit))?sync_route_lut[ix].dest[ibit]: "" )

    ACQ32_PATH_READBUFFER_PRINTF( PD(filp), "0\n" );
    
    for ( funkey = 0; funkey != NKEYS; ++funkey ){
	unsigned dest_mask = pdev->sync_dest_masks[funkey];
	    
	ACQ32_PATH_READBUFFER_PRINTF( PD(filp), "%s%d: %s, %s, %s, %s, %s\n",
				      "DI", funkey, 
				      DESTR( dest_mask, funkey, 0 ),
				      DESTR( dest_mask, funkey, 1 ),
				      DESTR( dest_mask, funkey, 2 ),
				      DESTR( dest_mask, funkey, 3 ),
				      DESTR( dest_mask, funkey, 4 )
	    );
    }
	
    return iarg;
}




static int do_streaming_command(
    struct file* filp,    
    char** argv,    
    int argc,    
    int iarg
    )
    // command format is stream <stride> <mean>
    // stride turns it off
{
    const char* verb = argv[iarg];
    int iarg_orig = iarg;


    if ( STREQ( verb, SC_SET_STREAMING ) ){
        if ( iarg+2 < argc ){
            int stride = ATOI( argv[++iarg] );
	    int mean   = ATOI( argv[++iarg] );
	    int npairs = 0;                  // max
	    int burst = 0;
	    if ( iarg+1 < argc ) {
		    npairs = ATOI( argv[++iarg] );
		    if ( iarg+1 <argc ){
			    burst = ATOI( argv[++iarg] );
		    }
	    }
	    

            PDEV(filp)->m_dpd.streaming = (mean?0x80000000:0)|stride;
            PDEV(filp)->m_dpd.subframe_id = 0; // reset subframe

            // start streaming from current pos to avoid "catch up"
            filp->f_pos = PDRVR(filp)->GetNumSamples( PD(filp) );

            PDEBUGL(3)( "do_streaming set %x\n", PDEV(filp)->m_dpd.streaming );

            PDRVR(filp)->SetStreaming( PD(filp), npairs, burst );

	    ++iarg;
        }else{
            ACQ32_PATH_READBUFFER_PRINTF( 
		PD(filp),
                "ERROR:" SC_SET_STREAMING "needs 2 args\n" );
        }
        return iarg;
    }

    return iarg_orig;
}    

#define MAXARGS 10
#define MAXCMDLINE 256



static ssize_t _read_command(
    struct file* filp, char* buf, size_t count, loff_t* posp
    )
{
//#define FN "_read_command() "
#define LBUFLEN 0x400
//    struct Acq32Device* p_dev = acq32_get_device_from_filp( filp );
    char lbuf[LBUFLEN];
    int nuser = 0;          // copied to user so far
    int nget;

    PDEBUGL(3)( " %d\n", count );

/*
 * normal action - retrieve response from CURRENT buffer 
 */
    while( nuser < count ) {
        nget = acq32_path_readbuffer_get( 
	    PD(filp), lbuf, MIN(LBUFLEN, count-nuser) );
        if ( nget == 0 ){
            break;
        }
        copy_to_user( buf+nuser, lbuf, nget );
        nuser += nget;
    }
   
    filp->f_pos += nuser;

    lbuf[nuser] = '\0';
    PDEBUGL(3)( " ends %s\n", nuser? lbuf: "EOF" );

    return nuser;
#undef LBUFLEN
//#undef FN
}

ssize_t acq32_status_read(
    struct file* filp, char* buf, size_t count, loff_t* posp
    )
{
//#define FN "acq32_status_read_command() "
    int nread;

    while( (nread = _read_command( filp, buf, count, posp )) == 0 ){
	interruptible_sleep_on( &PD(filp)->return_waitq );
	if ( signal_pending( current ) ){
	    return -EINTR;
	}
    }
    return nread;
//#undef FN
}

    
ssize_t acq32_master_read ( 
    struct file* filp, char* buf, size_t count, loff_t* posp 
    )
{
#define LBUFLEN 0x400
//    struct Acq32Device* p_dev = acq32_get_device_from_filp( filp );
    char lbuf[LBUFLEN];
    int nuser = 0;          // copied to user so far
    int nget;

    PDEBUGL(3)( "acq32_master_read %d\n", count );

/*
 * first time thru, flush any residual answers from previous paths
 */
    if ( !PD(filp)->flushed_device_read_buffer ){

        PDEBUGL(3) ( "acq32_master_read flush device\n" );
        while( nuser < count ){
            nget = acq32_device_readbuffer_get( 
		PDEV(filp), lbuf, MIN(LBUFLEN, count-nuser) );
            if ( nget == 0 ){
                PD(filp)->flushed_device_read_buffer = 1;
                PDEBUGL(3) ( "acq32_master_read flush done %d\n", nuser );
                break;
            }else{
                copy_to_user( buf+nuser, lbuf, nget );
                nuser += nget;
            }
        }

        filp->f_pos += nuser;    
    }

/*
 * normal action - retrieve response from CURRENT buffer 
 */
    if ( nuser < count ){
	nuser += _read_command( filp, buf+nuser, count-nuser, posp );
    }

    PDEBUGL(3)( "acq32_master_read ends %s\n", nuser? lbuf: "EOF" );

    return nuser;
#undef LBUFLEN
}

static int _write_command(
    struct file* filp, const char* buf, size_t count, loff_t* posp,
    decoder_func decoders[],
    int ndecoders
    )
{
//#define FN "_write_command() "
    char kbuf[MAXCMDLINE];
    char* argv[MAXARGS];
    int argc;
    char* next_command;

    count = MIN( MAXCMDLINE-1, count );

    copy_from_user( kbuf, buf, count );
    kbuf[count] = '\0';

    PRINTCMD( "acq32:<", kbuf );
    PDEBUGL(3)( "%s\n", kbuf );

    next_command = kbuf;

    while ( (argc = getargs( next_command, argv, MAXARGS, &next_command )) ){
        int iarg = 0;
        int idec = 0;

	APR_PRINTF( PD( filp ), ACQ32_ACK );

	while( iarg < argc ){
	    ASSERT( idec != ndecoders );    // bucket stops this
                
            iarg = decoders[idec++]( filp, argv, argc, iarg );
        }
	
        APR_PRINTF( PD(filp), "\n" );
    }

    PDEBUGL(3)( " ends return %d\n", next_command-kbuf );

    return next_command-kbuf;
//#undef FN
}


ssize_t acq32_data_write(
    struct file* filp, const char* buf, size_t count, loff_t* posp 
    )
{
    static decoder_func _DataDecoders[] = {
        do_format_command,
        do_seek_command,
        do_limit_command,
	do_streaming_command,
        do_bucket_command
    };
#define MAXDATADECODERS    (sizeof(_DataDecoders)/sizeof(decoder_func))

    return _write_command( filp, buf, count, posp,
			   _DataDecoders, MAXDATADECODERS );
}


ssize_t acq32_status_write (
    struct file* filp, const char* buf, size_t count, loff_t* posp 
    )
{
    static decoder_func _decoders[] = {
	do_WaitState,
        do_help,
        do_SetModeTriggeredContinuous,
        do_GetMemoryDepth,
        do_GetNumChannels,
        do_GetNumSamples,
        do_GetSetNumSamples,
        do_GetState,
        do_GetMode,
        do_GetChannelMask,
        do_GetInternalClock_command,
        do_GetDIO_command,
        do_GetInfo,
	do_GetSyncRoute_command,
	do_GetAvailableChannels,
	do_GetRange,
        do_bucket_command
    };
#define MAXDECODERS    (sizeof(_decoders)/sizeof(decoder_func))

    return _write_command( filp, buf, count, posp,
			   _decoders, MAXDECODERS );
#undef MAXDECODERS
}


ssize_t acq32_master_write(
    struct file* filp, const char* buf, size_t count, loff_t* posp 
    )
{
    static decoder_func _master_decoders[] = {
        do_help,
        do_SetAbort,
        do_SetArm,
        do_SetMode,
        do_SetModeGatedContinuous,
        do_SetModeTriggeredContinuous,
        do_GetMemoryDepth,
        do_GetNumChannels,
        do_GetNumSamples,
        do_GetSetNumSamples,
        do_GetState,
        do_GetMode,
        do_SetChannelMask,
        do_GetChannelMask,
        do_seek_command,
        do_limit_command,
        do_format_command,

        do_SetInternalClock_command,
        do_GetInternalClock_command,
	do_SetExternalClock_command,
        do_SetSampleTagging_command,

        // signal conditioning opts
        do_SetDIO_command,
        do_GetDIO_command,
        do_SetChannel_command,

        do_BigDump,
        do_GetInfo,

        do_SetDistributor_command,
        do_SetDataRoute_command,
	do_SetSyncRoute_command,
	do_GetSyncRoute_command,

        do_SetCal,

	    
	do_SetPhase,
	do_GetPhase,
	do_SetEvent,
	do_GetEvent,
	do_FireEvent,
	do_SetClock,    
	do_ClockNow,
        do_resetGUT,
	
        do_ReserveAO,
	do_SetUser,
	    
	do_SetThreshold,
	    
	do_GetAvailableChannels,
	do_GetRange,
	
        do_bucket_command
    };
#define MAXMASTERDECODERS    (sizeof(_master_decoders)/sizeof(decoder_func))

    return _write_command( filp, buf, count, posp,
			   _master_decoders, MAXMASTERDECODERS );
}














