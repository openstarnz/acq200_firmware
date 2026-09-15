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

APPS=	acq32command acq32fetch tv is-ramp 2hex 2volts dt100d

all: .depend  $(APPS)

release install: all
	cp $(APPS) ../../bin

depend .depend dep:
	$(CC) $(CPPFLAGS) -M *.c > $@


acq32command: acq32command.o
	$(LINK.c) -o $@ $^ -lpopt

acq32fetch: acq32fetch.o
	$(LINK.c) -o $@ $^ -lpopt -lm

tv: tv.o
	$(LINK.c) -o $@ $^ -lpopt

is_ramp: is_ramp.o
	$(LINK.c) -o $@ $^ -lpopt

2volts: 2volts.o
	$(LINK.c) -o $@ $^ -lpopt

clean:
	rm -f $(APPS) *.o .depend


ifeq (.depend,$(wildcard .depend))
include .depend
endif


