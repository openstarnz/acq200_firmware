/* acq200.h */



#ifndef __ACQ200_H__
#define __ACQ200_H__

#ifdef __KERNEL__
int acq200_get_bigbuf_resource( struct resource* resource );
#endif


/*
 * device minor #'s (nothing to do with h/w => remove
 */
#define FIFO_DIAGS_DEVICE 0
#define FIFO_RO_DEVICE 1
#define FIFO_WO_DEVICE 2
#define FIFO_RW_DEVICE 3
#define FIFO_RO_DEVBUF 5    /* Read only, fill bigbuf first */
#define FIFO_WO_DEVBUF 6    /* Write only, fill bigbuf first */
#define FIFO_RO_DEVBUF_TEST 15
#define FIFO_WO_DEVBUF_TEST 16

/*
 * main module should define ACQ200_FPGA_REGS, void* regs mapping
 */

#define ACQ200_FPGA_REG_BAR  0
#define ACQ200_FPGA_FIFO_BAR 1


/*
 * MU shared defs for driver comms
 */

#define ACQ200_MAILBOX_0         0x10
#define ACQ200_ODR               0x2c
#define ACQ200_OISR              0x30
#define ACQ200_OIMR              0x34
#define ACQ200_IQP               0x40
#define ACQ200_OQP               0x44

#define ACQ200_OISR_OM_0         0x01  /* not used */
#define ACQ200_OISR_OM_1         0x02  /* not used */
#define ACQ200_OISR_ODI          0x04  /* we use this one */
#define ACQ200_OISR_OPQI         0x08  
#define ACQ200_OISR_PCI_INTA     0x10  /* not obvious ?? */


struct DevGlob {
	int idx;
	struct Acq32Device *acq32_device;
	struct pci_dev *pci_dev;

	struct logical_dev {
		int major;
		int open_count;
		char drv_name[32];
	} bridge_dev, wave_dev;
};

struct Acq200Path {
	struct DevGlob* dg;
	int last_direction_hack;
};

#define AP(filp) ((struct Acq200Path*)PD(filp)->subclass)
#define DG(file) (AP(file)->dg)

#define LBUF(file) (PD(file)->scratch)
#define LBUFLEN    2048

/* Note: PD(file) casts file->private_data to (struct Acq32Path *), so
 * it is not an lvalue under modern GCC.  Assign through private_data
 * directly. */
#define ACQ200_ON_OPEN(inode, file) \
	do { \
		(file)->private_data = \
			acq200_makePathDescriptor((inode)->i_rdev); \
		if (PD(file) == 0) \
			return -ENODEV; \
		DG(file)->bridge_dev.open_count++; \
	} while (0)

#define ACQ200_ON_CLOSE(file) \
	do { \
		if (PD(file) != 0) { \
			acq200_freePathDescriptor(PD(file)); \
			DG(file)->bridge_dev.open_count--; \
			(file)->private_data = NULL; \
		} \
		return 0; \
	} while (0)

int wavegen_driver_init(struct DevGlob *dg);
void wavegen_driver_remove(struct DevGlob *dg);

struct Acq32Path* acq200_makePathDescriptor(dev_t minor);
void acq200_freePathDescriptor(struct Acq32Path *path);


int acq200_bridge_open (
	struct inode *inode, struct file *file
	);
int acq200_bridge_release_generic (
	struct inode *inode, struct file *file, int hrd_code
	);

ssize_t acq200_bridge_write_generic (
	struct file *file, const char *buf, size_t count, loff_t * posp,
	int hrd_code);

ssize_t acq200_bridge_read_generic (
	struct file* file, char* buf, size_t count, loff_t* posp,
	int hrd_code);
#endif /* #define __ACQ200_H__ */
