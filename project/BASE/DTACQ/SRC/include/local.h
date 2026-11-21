#define pgm_getchar getchar
#ifdef __arm

// KLUDGE time - arm works with rev1.8

/*****************************************************************************
 *
 * File: local.h
 *
 * $RCSfile: local.h,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: local.h,v 1.19.4.4 2006/01/15 11:21:54 pgm Exp $
 * $Log: local.h,v $
 * Revision 1.19.4.4  2006/01/15 11:21:54  pgm
 * *** empty log message ***
 *
 * Revision 1.19.4.3  2003/10/30 14:13:46  pgm
 * *** empty log message ***
 *
 * Revision 1.19.4.2  2003/10/04 17:31:19  pgm
 * RH9 switch for pesky remap vma
 *
 * Revision 1.19.4.1  2003/09/08 21:12:06  pgm
 * *** empty log message ***
 *
 * Revision 1.19  2002/11/03 21:41:49  pgm
 * *** empty log message ***
 *
 * Revision 1.17  2002/03/14 12:03:48  pgm
 * compiled on benbecula
 *
 * Revision 1.16  2002/01/04 17:35:32  pgm
 * LINUX_NEW_PCI th now 2.2.16
 *
 * Revision 1.15  2001/12/21 21:35:47  pgm
 * *** empty log message ***
 *
 * Revision 1.14  2001/12/21 21:32:46  pgm
 * LINUX_NEW_PCI
 *
 * Revision 1.13  2001/08/19 19:23:02  pgm
 * Alpha builds
 *
 * Revision 1.12  2001/08/12 08:30:04  pgm
 * user LED control
 *
 * Revision 1.11  2001/04/14 20:37:51  pgm
 * CPCI routing coded, needs testing
 *
 * Revision 1.10  2001/04/10 19:14:19  pgm
 * fixed for arm too
 *
 * Revision 1.9  2001/04/10 19:11:01  pgm
 * kludge to enable arm+Linux from same version
 *
 * Revision 1.8  2001/03/18 17:38:44  pgm
 * builds with max 2 warnings
 *
 * Revision 1.7  2001/02/16 21:38:41  pgm
 * update from arm side
 *
 * Revision 1.6  2001/02/16 21:37:21  pgm
 * update from arm side
 *
 * Revision 1.5  2000/07/01 07:26:46  pgm
 * keep this one, it works better
 *
 * Revision 1.3  1999/10/28 13:36:39  pgm
 * assign bits in priority order
 *
 * Revision 1.2  1999/10/16 07:24:12  pgm
 * *** empty log message ***
 *
 * Revision 1.1  1999/10/01 09:36:32  pgm
 * *** empty log message ***
 *
 *
\*****************************************************************************/



#ifndef _LOCAL_H_
#define _LOCAL_H_


/*
 * String Handling
 */

#ifndef __KERNEL__
#include <string.h>
#endif

#define STREQ( s1, s2 )        (strcmp( s1, s2 ) == 0)
#define STREQN( s1, s2, n )    (strncmp( s1, s2, n ) == 0) 
#define STREQNL( s1, s2 )      (strncmp( s1, s2, strlen(s2) ) == 0)

#define ATOL( s1 )    (unsigned)strtoul( s1, NULL, 0 )


/*
 * Range Checking and simple math
 */

#define IN_RANGE(xx,ll,rr)      ((xx)>=(ll)&&(xx)<=(rr))
#define CLAMP(xx,ll,rr)         ((xx) = (xx)<(ll)? (ll): (xx)>(rr)? (rr): (xx))
#define SWAP(aa,bb,tt)  ( tt = aa, aa = bb, bb = tt )

#define MAX(aa,bb)      ((aa)>=(bb)?(aa):(bb))
#define MIN(aa,bb)      ((aa)<=(bb)?(aa):(bb))


#define ABS(aa)                 ((aa)>0?(aa):-(aa))
#define SQR(aa)                 ((aa)*(aa))
#define SIGN(aa)                ((aa)>=0? 1: -1)


/*
 * boolean values
 */

#ifndef OK
#define OK      0
#endif
#ifndef ERR
#define ERR    -1
#endif
#ifndef FALSE
#define FALSE   0
#endif
#ifndef TRUE
#define TRUE    1
#endif
#ifndef ON
#define ON      1
#endif
#ifndef OFF
#define OFF     0
#endif

extern int P_debug;

#define PRINTF    if ( P_debug ) printf
#define PRINTFL(n) if ( P_debug > n ) printf
#define PRINTID	PRINTF( __FILE__ ":%d\n", __LINE__ )

#define ST_PRINTF    if ( P_debug<0) printf

#define SUCCESS   1
#define FAIL      (!SUCCESS)



extern void makeAssertion( char* file, int line );

#define ARM_ASSERT( cond ) \
	if (!(cond)) makeAssertion( __FILE__, __LINE__ )


/*
 * useful constants
 */

#define KB    0x400
#define MB    0x100000


#define WORKTODO (-1)
#ifdef __arm

#ifndef NO_PGM_PRINTF

extern int pgm_printf( char fmt[], ... );
extern int pgm_fgets( char buf[], int maxbuf );

#define printf			pgm_printf
#define fflush( stream )
#define fgets( a, b, c )	pgm_fgets( a, b )

#endif // NO_PGM_PRINTF

#endif			// __arm

typedef unsigned       u32;
typedef unsigned short u16;
typedef unsigned char  u8;

#endif    /* _LOCAL_H_ */


#else // __arm ... the else is all but arm

/*****************************************************************************
 *
 * File: local.h
 *
 * $RCSfile: local.h,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: local.h,v 1.19.4.4 2006/01/15 11:21:54 pgm Exp $
 * $Log: local.h,v $
 * Revision 1.19.4.4  2006/01/15 11:21:54  pgm
 * *** empty log message ***
 *
 * Revision 1.19.4.3  2003/10/30 14:13:46  pgm
 * *** empty log message ***
 *
 * Revision 1.19.4.2  2003/10/04 17:31:19  pgm
 * RH9 switch for pesky remap vma
 *
 * Revision 1.19.4.1  2003/09/08 21:12:06  pgm
 * *** empty log message ***
 *
 * Revision 1.19  2002/11/03 21:41:49  pgm
 * *** empty log message ***
 *
 * Revision 1.17  2002/03/14 12:03:48  pgm
 * compiled on benbecula
 *
 * Revision 1.16  2002/01/04 17:35:32  pgm
 * LINUX_NEW_PCI th now 2.2.16
 *
 * Revision 1.15  2001/12/21 21:35:47  pgm
 * *** empty log message ***
 *
 * Revision 1.14  2001/12/21 21:32:46  pgm
 * LINUX_NEW_PCI
 *
 * Revision 1.13  2001/08/19 19:23:02  pgm
 * Alpha builds
 *
 * Revision 1.12  2001/08/12 08:30:04  pgm
 * user LED control
 *
 * Revision 1.11  2001/04/14 20:37:51  pgm
 * CPCI routing coded, needs testing
 *
 * Revision 1.10  2001/04/10 19:14:19  pgm
 * fixed for arm too
 *
 * Revision 1.9  2001/04/10 19:11:01  pgm
 * kludge to enable arm+Linux from same version
 *
 * Revision 1.3  1999/10/28 13:36:39  pgm
 * assign bits in priority order
 *
 * Revision 1.2  1999/10/16 07:24:12  pgm
 * *** empty log message ***
 *
 * Revision 1.1  1999/10/01 09:36:32  pgm
 * *** empty log message ***
 *
 *
\*****************************************************************************/

#ifndef _LOCAL_H_
#define _LOCAL_H_


#ifdef __KERNEL__
#include <linux/version.h>
/* newer kernels define MIN/MAX here: they must be seen before the fallbacks below */
#include <linux/kernel.h>

#ifndef VERSION_CODE
#  define VERSION_CODE(vers,rel,seq) ( ((vers)<<16) | ((rel)<<8) | (seq) )
#endif

/* only allow 2.0.x and 2.1.y */
/*
#if LINUX_VERSION_CODE > VERSION_CODE(2,2,16)
#define LINUX_NEW_PCI
#endif
*/
#if LINUX_VERSION_CODE > VERSION_CODE(2,4,18)
#define RH9
#endif
/*
 * String Handling
 */


#if (LINUX_VERSION_CODE < 0x020600)
#error NO KERNEL_26
#include <asm/current.h>    // this reaches the parts for ALPHA
#endif
#endif /* __KERNEL__ */



#ifndef __KERNEL__
#include <string.h>
#endif

#define STREQ( s1, s2 )        (strcmp( s1, s2 ) == 0)
#define STREQN( s1, s2, n )    (strncmp( s1, s2, n ) == 0) 
#define STREQNL( s1, s2 )      (strncmp( s1, s2, strlen(s2) ) == 0)


/*
 * Range Checking and simple math
 */

#define IN_RANGE(xx,ll,rr)      ((xx)>=(ll)&&(xx)<=(rr))
#define CLAMP(xx,ll,rr)         ((xx) = (xx)<(ll)? (ll): (xx)>(rr)? (rr): (xx))
#define SWAP(aa,bb,tt)  ( tt = aa, aa = bb, bb = tt )

#ifndef MAX
#define MAX(aa,bb)      ((aa)>=(bb)?(aa):(bb))
#endif
#ifndef MIN
#define MIN(aa,bb)      ((aa)<=(bb)?(aa):(bb))
#endif


#define ABS(aa)                 ((aa)>=0?(aa):-(aa))
#define SQR(aa)                 ((aa)*(aa))
#define SIGN(aa)                ((aa)>=0? 1: -1)


/*
 * boolean values
 */

#ifndef OK
#define OK      0
#endif
#ifndef ERR
#define ERR    -1
#endif
#ifndef FALSE
#define FALSE   0
#endif
#ifndef TRUE
#define TRUE    1
#endif
#ifndef ON
#define ON      1
#endif
#ifndef OFF
#define OFF     0
#endif


/*
 * Printing Debugs
 */

#undef PDEBUG             /* undef it, just in case */

#ifndef NDEBUG
#  ifdef __KERNEL__
/* This one if debugging is on, and kernel space */
#    define PDEBUG(fmt, args...) printk( KERN_DEBUG "acq32: " fmt, ## args)
#  else
/* This one for user space */
#    define PDEBUG(fmt, args...) fprintf(stderr, fmt, ## args)
#  endif
#else
#  define PDEBUG(fmt, args...) /* not debugging: nothing */
#endif

#undef PDEBUGG
#define PDEBUGG(fmt, args...) /* nothing: it's a placeholder */


/*
 * useful constants
 */

#define KB    0x400
#define MB    0x100000

/*
 * Assertions
 */

#ifndef NDEBUG
#   ifdef __KERNEL__

/*
 * spins after assert. NOT ideal. we really want to die
 * would kill_proc( current->pid, SIG_QUIT, 0 ) do the biz?
 */
#       define ASSERT( cond ) \
                   if ( !(cond) ){ \
                       PDEBUG( "ASSERTION: %s %d", __FILE__, __LINE__ );\
                       while(1) schedule();\
                   }
#   else
#       define ASSERT( cond ) \
            if ( !(cond) ){ \
                fprintf( stderr, "ASSERTION: %s %d",__FILE__,__LINE__ );\
                exit( 1 );\
            }
#   endif
#else
#   define ASSERT( cond )
#endif // NDEBUG

#define WORKTODO (-1)

#endif    /* _LOCAL_H_ */



#endif //_arm












