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

all: .depend  bdt

clean:
	rm -f *.o bdt .depend

release install: all
	cp bdt ../../bin

depend .depend dep:
	$(CC) $(CPPFLAGS) -M *.c > $@


bdt: bdt.o
	$(LINK.c) -o $@ $^ -lpopt

ifeq (.depend,$(wildcard .depend))
include .depend
endif

