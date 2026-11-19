/*
 * compat.h - kernel version differences for the acq32/acq200 drivers.
 *
 * Every LINUX_VERSION_CODE test lives here so the rest of the driver reads as
 * one source tree.  Where a modern API also exists on 2.6.32 the driver calls
 * it directly and this header has nothing to say about it.
 *
 * Built and run on: 2.6.32 (EL6), 6.1, 6.12, 6.14.  Version boundaries below
 * are the kernel release that introduced or removed the API; kernels between
 * the tested ones have not been built.
 */

#ifndef _ACQ32_COMPAT_H_
#define _ACQ32_COMPAT_H_

#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/time.h>
#include <linux/proc_fs.h>
#include <linux/dma-mapping.h>
#include <linux/pci.h>
#include <asm/uaccess.h>

/* ------------------------------------------------------------------------- */
/* Annotations removed in 3.8                                                */
/* ------------------------------------------------------------------------- */

#ifndef __devinit
#define __devinit
#endif
#ifndef __devinitdata
#define __devinitdata
#endif
#ifndef __devexit
#define __devexit
#endif
#ifndef __devexit_p
#define __devexit_p(x) x
#endif

/* ------------------------------------------------------------------------- */
/* struct file -> inode                                                      */
/* ------------------------------------------------------------------------- */

/* f_dentry was removed in 3.19, file_inode() appeared in 3.9 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,9,0)
#define acq32_file_inode( filp )	file_inode( filp )
#else
#define acq32_file_inode( filp )	( (filp)->f_dentry->d_inode )
#endif

#define acq32_file_rdev( filp )		( acq32_file_inode( filp )->i_rdev )

/* ------------------------------------------------------------------------- */
/* access_ok lost its type argument in 5.0                                   */
/* ------------------------------------------------------------------------- */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,0,0)
#define acq32_access_ok( type, addr, size )	access_ok( addr, size )
#else
#define acq32_access_ok( type, addr, size )	access_ok( type, addr, size )
#endif

/* ------------------------------------------------------------------------- */
/* Time of day                                                               */
/* ------------------------------------------------------------------------- */

/*
 * struct timeval and do_gettimeofday() are gone from the kernel.  Seconds and
 * microseconds since the epoch, as they always were.
 */
struct acq32_timeval {
	long tv_sec;
	long tv_usec;
};

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,17,0)
static inline void acq32_gettimeofday( struct acq32_timeval* tv )
{
	struct timespec64 ts;

	ktime_get_real_ts64( &ts );
	tv->tv_sec  = ts.tv_sec;
	tv->tv_usec = ts.tv_nsec / 1000;
}
#else
static inline void acq32_gettimeofday( struct acq32_timeval* tv )
{
	struct timeval now;

	do_gettimeofday( &now );
	tv->tv_sec  = now.tv_sec;
	tv->tv_usec = now.tv_usec;
}
#endif

/*
 * -Werror=date-time is on from 4.4; the build stamp in /proc/.acq32 is
 * deliberate.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,4,0)
#define ACQ32_BUILD_STAMP_BEGIN \
	_Pragma( "GCC diagnostic push" ) \
	_Pragma( "GCC diagnostic ignored \"-Wdate-time\"" )
#define ACQ32_BUILD_STAMP_END \
	_Pragma( "GCC diagnostic pop" )
#else
#define ACQ32_BUILD_STAMP_BEGIN
#define ACQ32_BUILD_STAMP_END
#endif

/* ------------------------------------------------------------------------- */
/* interruptible_sleep_on() family                                           */
/* ------------------------------------------------------------------------- */

/*
 * The sleep_on functions were removed from the kernel.  These do what the old
 * ones did: queue on the wait queue, sleep, dequeue.  The window between the
 * caller's last look at its condition and prepare_to_wait() is as open as it
 * ever was - a wake_up in that window is lost - so callers must keep tolerating
 * a timeout.  Returns the jiffies left, as before.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,0,0)
static inline long acq32_interruptible_sleep_on_timeout(
	wait_queue_head_t* q, long timeout )
{
	DEFINE_WAIT( wait );

	prepare_to_wait( q, &wait, TASK_INTERRUPTIBLE );
	timeout = schedule_timeout( timeout );
	finish_wait( q, &wait );
	return timeout;
}

static inline void acq32_interruptible_sleep_on( wait_queue_head_t* q )
{
	DEFINE_WAIT( wait );

	prepare_to_wait( q, &wait, TASK_INTERRUPTIBLE );
	schedule();
	finish_wait( q, &wait );
}

#define interruptible_sleep_on_timeout( q, timeout ) \
	acq32_interruptible_sleep_on_timeout( q, timeout )
#define interruptible_sleep_on( q ) \
	acq32_interruptible_sleep_on( q )
#endif

/* ------------------------------------------------------------------------- */
/* Bus addresses                                                             */
/* ------------------------------------------------------------------------- */

/*
 * virt_to_bus() is gone from x86.  PHASE 2 (DMA API): this assumes no IOMMU
 * translation and the x86 direct mapping, i.e. bus address == physical
 * address.  Remove it together with the callers when the buffers move to
 * dma_alloc_coherent().
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,0,0)
#define virt_to_bus( addr )	virt_to_phys( addr )
#endif

/* ------------------------------------------------------------------------- */
/* DMA mask                                                                  */
/* ------------------------------------------------------------------------- */

/* pci_set_dma_mask() and friends are gone; dma_set_mask_and_coherent() is 3.13 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0)
#define acq32_dma_set_mask_and_coherent( dev, mask ) \
	dma_set_mask_and_coherent( dev, mask )
#else
static inline int acq32_dma_set_mask_and_coherent( struct device* dev, u64 mask )
{
	int rc = dma_set_mask( dev, mask );

	if ( !rc ){
		rc = dma_set_coherent_mask( dev, mask );
	}
	return rc;
}
#endif

/* ------------------------------------------------------------------------- */
/* mmap                                                                      */
/* ------------------------------------------------------------------------- */

/* vm_flags is read-only from 6.3 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6,3,0)
#define vm_flags_set( vma, flags )	( (vma)->vm_flags |= (flags) )
#endif

/*
 * The vm_operations fault method changed shape in 4.11 (the vma moved into the
 * vm_fault) and again in 4.17 (vm_fault_t).  impl is
 *     int impl( struct vm_area_struct* vma, unsigned long address );
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0)
#define ACQ32_DEFINE_VMA_FAULT( name, impl ) \
static vm_fault_t name( struct vm_fault* vmf ) \
{ \
	return impl( vmf->vma, vmf->address ); \
}
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(4,11,0)
#define ACQ32_DEFINE_VMA_FAULT( name, impl ) \
static int name( struct vm_fault* vmf ) \
{ \
	return impl( vmf->vma, vmf->address ); \
}
#else
#define ACQ32_DEFINE_VMA_FAULT( name, impl ) \
static int name( struct vm_area_struct* vma, struct vm_fault* vmf ) \
{ \
	return impl( vma, (unsigned long)vmf->virtual_address ); \
}
#endif

/* ------------------------------------------------------------------------- */
/* /proc                                                                     */
/* ------------------------------------------------------------------------- */

/*
 * The /proc entries are produced by handlers with the old read_proc contract:
 * format into buf (one page), set *eof, return the length.  The text each one
 * generates is the interface scripts parse, so there is one handler per entry
 * and both paths below call it.
 *
 * create_proc_read_entry() was removed in 3.10; from there the handler is
 * wrapped in a seq_file, and in a proc_ops from 5.6.
 */
typedef int (*acq32_read_proc_fn)(
	char* buf, char** start, off_t offset, int len, int* eof, void* data );

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0)

#define acq32_create_proc_read_entry( name, handler ) \
	create_proc_read_entry( name, 0, NULL, handler, 0 )

#else

#include <linux/seq_file.h>

static inline void* acq32_pde_data( struct inode* inode )
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,17,0)
	return pde_data( inode );
#else
	return PDE_DATA( inode );
#endif
}

static inline int acq32_proc_show( struct seq_file* m, void* unused )
{
	acq32_read_proc_fn handler = m->private;
	char* page = (char*)__get_free_page( GFP_KERNEL );
	char* start = NULL;
	int eof = 0;
	int len;

	if ( !page ){
		return -ENOMEM;
	}
	len = handler( page, &start, 0, 0, &eof, NULL );
	seq_write( m, page, len );
	free_page( (unsigned long)page );
	return 0;
}

/*
 * Some handlers consume state as they print (the .inst buffer is emptied), so
 * show() must run once: the seq_file buffer is sized so the one page a handler
 * may fill can never overflow it and trigger a retry.
 */
static inline int acq32_proc_open( struct inode* inode, struct file* file )
{
	return single_open_size( file, acq32_proc_show,
				 acq32_pde_data( inode ), 2*PAGE_SIZE );
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,6,0)
static inline void* acq32_proc_create_data(
	const char* name, acq32_read_proc_fn handler )
{
	static const struct proc_ops ops = {
		.proc_open    = acq32_proc_open,
		.proc_read    = seq_read,
		.proc_lseek   = seq_lseek,
		.proc_release = single_release,
	};
	return proc_create_data( name, 0, NULL, &ops, handler );
}
#else
static inline void* acq32_proc_create_data(
	const char* name, acq32_read_proc_fn handler )
{
	static const struct file_operations ops = {
		.owner   = THIS_MODULE,
		.open    = acq32_proc_open,
		.read    = seq_read,
		.llseek  = seq_lseek,
		.release = single_release,
	};
	return proc_create_data( name, 0, NULL, &ops, handler );
}
#endif

#define acq32_create_proc_read_entry( name, handler ) \
	acq32_proc_create_data( name, handler )

#endif

#endif	/* _ACQ32_COMPAT_H_ */
