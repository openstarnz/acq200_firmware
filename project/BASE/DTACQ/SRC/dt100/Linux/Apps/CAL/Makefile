# Comment/uncomment the following line to disable/enable debugging
DEBUG = y

# Change it here or specify it on the "make" commandline
INCLUDEDIR = /usr/include


ifeq ($(DEBUG),y)
  DEBFLAGS = -O -g # "-O" is needed to expand inlines
else
  DEBFLAGS = -O2
endif

DTINC = ../../../../include
DTLINC= ../../include

CPPFLAGS =  -Wall $(DEBFLAGS)
CPPFLAGS += -I$(INCLUDEDIR) -I${DTINC} -I${DTLINC}

APPS = mean atweak mbox mmap diags MakeCalBin acq16tobin

all: .depend $(APPS)

release: all
	cp $(APPS) ../../bin


mean: mean.o
	$(LINK.c) -o $@ $^ -lpopt

atweak: atweak.o
	$(LINK.c) -o $@ $^ -lpopt

mbox: mbox.o
	$(LINK.c) -o $@ $^ -lpopt

diags: diags.o
	$(LINK.c) -o $@ $^ -lpopt

depend .depend dep:
	$(CC) $(CPPFLAGS) -M *.c > $@

mmap: mmap.o
	$(LINK.c) -o $@ $^ -lpopt

acq16tobin:
	echo no action needed for acq16tobin

clean: 
	rm -f $(APPS)
	rm -f *.o
	rm -f .depend

MakeCalBin: MakeCalBin.o
	$(LINK.c) -o $@ $^ -lpopt

ifeq (.depend,$(wildcard .depend))
include .depend
endif

