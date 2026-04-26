/* ------------------------------------------------------------------------- */
/* file wavegen.c                                                            */
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

/*
 * WAV232CPCI-specific hooks
 *
 * minor coding:
 * minor{5:0} : channel (0..32}
 * minor{8:6} : track {0..7}
 */
#include <linux/kernel.h>
//#include <linux/interrupt.h>
#include <linux/pci.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/init.h>
#include <linux/timex.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>


#include <linux/proc_fs.h>

#ifndef EXPORT_SYMTAB
#define EXPORT_SYMTAB
#include <linux/module.h>
#endif

#include "local.h"

#include "acq200.h"
#include "acq200_hml.h"
#include "acq200_debug.h"

#include "acq32.h"        /* local definitions */


int wav232_open( struct inode* inode, struct file* file )
{  
	return acq200_bridge_open(inode, file);
}

ssize_t 
wav232_read ( struct file* file, char* buf, size_t count, loff_t* posp )
{
	return acq200_bridge_read_generic (
		file, buf, count, posp,HRD_WAVETRACK );
}

ssize_t 
wav232_write (struct file *file, const char *buf, size_t count, loff_t * posp)
// write data as if from debug port
{
	return acq200_bridge_write_generic (
		file, buf, count, posp, HRD_WAVETRACK );
}
int wav232_release( struct inode* inode, struct file* file)
{
	return acq200_bridge_release_generic(inode, file, HRD_WAVETRACK);
}


static void run_wavegen_mknod_helper(int major)
{

}

int __devinit
wavegen_driver_init(struct DevGlob *dg)
{
	static struct file_operations wave_out_fops = {
		.open = wav232_open,
		.release = wav232_release,
		.read = wav232_read,
		.write = wav232_write,
	};
	int rc;

	sprintf( dg->wave_dev.drv_name, "wav232out.%d", dg->idx );

	rc = register_chrdev(dg->wave_dev.major = 0, 
			     dg->wave_dev.drv_name, 
			     &wave_out_fops);

	if (rc < 0){
		err( "can't get major %d", dg->wave_dev.major );
		return rc;
	}else{
		dg->wave_dev.major = rc;
		info( "device major set %d\n", dg->wave_dev.major );
	}
	
	run_wavegen_mknod_helper( dg->wave_dev.major );
/* @@WORKTODO - what proc_entries ? 
	create_proc_entries();
*/
	return 0;
}

void wavegen_driver_remove(struct DevGlob *dg)
{
	unregister_chrdev(dg->wave_dev.major, dg->wave_dev.drv_name);
}
