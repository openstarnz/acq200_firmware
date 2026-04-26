/* ------------------------------------------------------------------------- */
/* acq200_bigbuf_alloc.c allocator to share bigbuf                           */
/* ------------------------------------------------------------------------- */
/*   Copyright (C) 2003 Peter Milne, D-TACQ Solutions Ltd
 *                      <Peter dot Milne at D hyphen TACQ dot com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of Version 2 of the GNU General Public License 
    as published by the Free Software Foundation;

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                */
/* ------------------------------------------------------------------------- */

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

#include "acq32.h"
#include "acq200_debug.h"

static struct {
	long base;
	long length;
	int ndevices;
	long inext;
} bb;


void acq200_bb_init( long base, long length, int ndevices )
{
	bb.base = base;
	bb.length = length;
	bb.ndevices = ndevices;
	bb.inext = 0;
}

void acq200_bb_close()
{

}

static void fill(struct IoMapping *mapping)
{
	int imap;
	int imax = mapping->len / sizeof (u32);

	for (imap = 0; imap != imax; ++imap){
		mapping->va[imap] = mapping->pa + imap*4;
	}
}
int acq200_bb_alloc( struct IoMapping *mapping, long length, char name[] )
{
	if ( bb.inext+length > bb.length ){
		return -ENOMEM;
	}
	mapping->pa = bb.base + bb.inext;
	mapping->len = length;
	strncpy( mapping->name, name, sizeof(mapping->name)-1 );
	mapping->va = ioremap(mapping->pa, mapping->len);

	fill(mapping);

	if (mapping->va == 0){
		return -ENOMEM;
	}
	bb.inext += length;
	return 0;
}

int acq200_bb_free(struct IoMapping* map)
{
	iounmap((void*)map->pa);
	return 0;
}

#define NMAPPINGS 16
#define NDATAMAPPING 15

#define MSG_MAPPING 15

struct BigBufPool {
	int reserved[NMAPPINGS];
	struct IoMapping bufs[NMAPPINGS];
	struct ReturnCommandHandler handlers[NMAPPINGS];
	wait_queue_head_t sleepq;
};


void acq200_bb_pool_create(struct Acq32Device *device)
{
	int ibuf;
	struct IoMapping *bb = &device->bigbuf.bb;
	struct BigBufPool *pool = (struct BigBufPool*)
		KMALLOC(sizeof(struct BigBufPool), GFP_KERNEL);
	int buflen = bb->len/NMAPPINGS;
	unsigned offset;

	memset(pool, 0, sizeof(struct BigBufPool));
	
	for ( ibuf = 0, offset = 0; 
	      ibuf != NMAPPINGS; ++ibuf, offset += buflen ){	

		struct IoMapping *buf = &pool->bufs[ibuf];

		buf->va = (u32*)((char*)bb->va + offset);
		buf->pa = offset;
		buf->len = buflen;
		sprintf( buf->name, "buf %d.%d", device->major, ibuf );
	}
	pool->reserved[MSG_MAPPING] = 1;
	init_waitqueue_head(&pool->sleepq);
	device->bigbuf.pool = pool;
}
void acq200_bb_pool_delete(struct Acq32Device *device)
{
	if ( device->bigbuf.pool ){
		KFREE(device->bigbuf.pool);
		device->bigbuf.pool = 0;
	}
}


struct IoMapping *acq200_bb_pool_alloc(
	struct Acq32Device *device, 
	struct ReturnCommandHandler **rch,
	int direction)
{
#define IMAX (NDATAMAPPING-1)
#define INCR(ibuf) (ibuf = ibuf==IMAX? 0: ibuf+1)

	static int ibuf = -1;
	struct BigBufPool *pool = device->bigbuf.pool;
	int ibuf1;

// NO reservation yet, just a simple one-after-the other ...
	for ( ibuf1 = INCR(ibuf); pool->reserved[ibuf]; INCR(ibuf) ){
		if ( ibuf == ibuf1 ){
			if ( interruptible_sleep_on_timeout(
				     &pool->sleepq, 1000) == 0 ){
				info( " TIMEOUT" );
				return 0;
			}
		}
	}

	pool->reserved[ibuf] = 1;
	if (rch) *rch = &pool->handlers[ibuf];
	return &pool->bufs[ibuf];
}
void acq200_bb_pool_free(struct Acq32Device *device, struct IoMapping *map)
{
	struct BigBufPool *pool = device->bigbuf.pool;
	int ibuf;

	for ( ibuf = 0; ibuf != NDATAMAPPING; ++ibuf ){
		if ( &pool->bufs[ibuf] == map ){
			pool->reserved[ibuf] = 0;
			wake_up_interruptible(&pool->sleepq);
			return;
		}
	}

	BUG();
}
