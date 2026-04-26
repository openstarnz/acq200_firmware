# Drivers/Makefile - kbuild-style build for Linux 2.6.32+ (CentOS 6)
#
# Builds two kernel modules:
#   acq32-drv.ko  - ACQ32/ACQ196 PCI DAQ
#   acq200-drv.ko - ACQ200 bridge + DAQ
#
# Plus userspace tools: reset-driver, acq32_debug, acqcmd, irq, acq2sh
#
# Usage:
#   make           - build modules + userspace apps
#   make modules   - build kernel modules only
#   make apps      - build userspace tools only
#   make clean
#
# Override KDIR if not building against the running kernel:
#   make KDIR=/usr/src/kernels/2.6.32-754.el6.x86_64

ifneq ($(KERNELRELEASE),)
# ============================================================
# kbuild context (invoked by the kernel's top-level Makefile)
# ============================================================

obj-m += acq32-drv.o
obj-m += acq200-drv.o

ccflags-y := \
	-I$(src)/../../../include \
	-I$(src)/../include \
	-I$(src)/../../include \
	-DLINUX_NEW_PCI

# Files shared between both modules (compiled identically — no per-module defines)
acq32_shared_objs := \
	acq32_core.o \
	acq32_simul.o \
	acq32_utils.o \
	acq32_command.o \
	acq32_fungen.o \
	acq200_bigbuf_alloc.o \
	debug.o \
	strtol.o

acq32-drv-y := $(acq32_shared_objs) acq32_mmap.o acq32.o acq32_specific.o

acq200-drv-y := $(acq32_shared_objs) acq200_module.o acq200.o wavegen.o

# acq200_module.c is a thin wrapper that #includes acq32.c with -DACQ200.
# Only this object needs the define; the rest of the shared sources have no
# ACQ200 conditionals (verified at port time).
CFLAGS_acq200_module.o := -DACQ200

else
# ============================================================
# user-invocation context
# ============================================================

KDIR ?= /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

# Userspace apps — plain C, no kernel headers
APP_CFLAGS := -O2 -Wall \
	-I../../../include \
	-I../include \
	-I../../include
APP_LDLIBS_POPT := -lpopt

APPS = reset-driver acq32_debug acqcmd irq acq2sh

DTLBIN  = ../bin
MODULES = ../modules

MODS = \
	acq32-drv.ko \
	acq200-drv.ko \
	acq32-mknod.pl \
	acq32.load \
	acq200.load \
	cmdline_lookup \
	getBoards.pm \
	acq32-init.pl

.PHONY: all modules modules_install apps clean install release

all: modules apps

modules:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

modules_install:
	$(MAKE) -C $(KDIR) M=$(PWD) modules_install

apps: $(APPS)

reset-driver: reset-driver.c
	$(CC) $(APP_CFLAGS) -o $@ $< $(APP_LDLIBS_POPT)

acqcmd: acqcmd.c
	$(CC) $(APP_CFLAGS) -o $@ $< $(APP_LDLIBS_POPT)

acq2sh: acq2sh.c
	$(CC) $(APP_CFLAGS) -o $@ $< $(APP_LDLIBS_POPT)

acq32_debug: acq32_debug.c
	$(CC) $(APP_CFLAGS) -o $@ $<

irq: irq.c
	$(CC) $(APP_CFLAGS) -o $@ $<

bigread: bigread.c
	$(CC) $(APP_CFLAGS) -o $@ $< $(APP_LDLIBS_POPT)

install release: all
	@mkdir -p $(DTLBIN) $(MODULES)
	cp $(APPS) $(DTLBIN)
	cp $(MODS) $(MODULES)

clean:
	-$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f $(APPS) bigread *.o *.ko *.mod.c *.mod.o *.symvers *.order *~ core .depend
	rm -rf .tmp_versions

endif
