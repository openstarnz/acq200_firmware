###############################################################################
#
#  File:
#
#  $RCSfile: Makefile,v $
#
#  Copyright (C) 2001 D-TACQ Solutions Ltd
#  not to be used without owner's permission
#
#  Description:
#
#  $Id: Makefile,v 1.15 2003/06/22 12:43:37 pgm Exp $
#  $Log: Makefile,v $
#  Revision 1.15  2003/06/22 12:43:37  pgm
#  htstream2 in makefile
#
#  Revision 1.14  2003/06/02 20:51:26  pgm
#  HTSTREAM V2 single thread good
#
#  Revision 1.13  2003/05/09 07:37:54  pgm
#  compiles on RH8
#
#  Revision 1.12  2003/05/08 10:21:00  pgm
#  simplify link step
#
#  Revision 1.11  2003/05/08 09:56:46  pgm
#  *** empty log message ***
#
#  Revision 1.10  2003/05/08 09:56:06  pgm
#  remove extra link phase
#
###############################################################################

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
#CPPFLAGS += -I$(INCLUDEDIR) -I${DTINC} -I${DTLINC}
CPPFLAGS += -I${DTINC} -I${DTLINC}

APPS=	htstream htstream2
OBJS=   htprotocol.o htif.o mbox.o \
	sinkfactory.o simplesinks.o fftsink.o

# this is all cribbed from fftw - thanks guys

LIBTOOL=libtool

LIBS= -lpopt -lm

ADLIBDIR=/usr/local/lib

#LDADD=$(ADLIBDIR)/librfftw.la $(ADLIBDIR)/libfftw.la

CFLAGS = -O6 -fomit-frame-pointer -Wall -W -Wcast-qual -Wpointer-arith -Wcast-align -pedantic -fno-schedule-insns -fschedule-insns2 -malign-double -fstrict-aliasing -mcpu=pentium
LDFLAGS =
CCLD = "ls -l;$(CC)"
#CCLD = "echo PGM stubbed this line:"
LINK = $(LIBTOOL) --mode=link $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@

all: .depend  $(APPS)

realfft: realfft.o
	$(LINK.c) -o $@ $^ -lrfftw -lfftw -lm

release install: all
	cp $(APPS) ../../bin

depend .depend dep:
	$(CC) $(CPPFLAGS) -M *.c* > .depend


htstream.orig: $(OBJS)
	$(LINK) $(LDFLAGS) $(OBJS) $(LDADD) $(LIBS)

htstream: htstream.o $(OBJS)
	g++ $(LDFLAGS) htstream.o $(OBJS) $(LDADD) $(LIBS) -o $@

htstream2: htstream2.o $(OBJS) mthread.o
	g++ $(LDFLAGS) $< $(OBJS) mthread.o  $(LDADD) $(LIBS) -lpthread -o $@


package:
	cp ../../../../include/acq32busprot.h .
	cp ../../../../include/local.h .
	cp ../../include/acq32ioctl.h .
	tar cvzf /tmp/htstream.tgz Makefile *.[ch] *.cpp *.proj
	rm acq32busprot.h
	rm local.h
	rm acq32ioctl.h
        
clean:
	rm -f $(APPS)
	rm -f *.o
	rm -f .depend


mthread_test: mthread.o mthread_test.o
	g++ $(LDFLAGS) mthread_test.o mthread.o -lpthread $(LIBS) -o $@

ifeq (.depend,$(wildcard .depend))
include .depend
endif


