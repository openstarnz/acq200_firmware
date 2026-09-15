# Comment/uncomment the following line to disable/enable debugging
DEBUG = y

# Change it here or specify it on the "make" commandline
#INCLUDEDIR = /usr/include

ifeq ($(DEBUG),y)
  DEBFLAGS = -O -g # "-O" is needed to expand inlines
else
  DEBFLAGS = -O2
endif

DTINC = ../../../../include
DTLINC= ../../include

CPPFLAGS =  -Wall $(DEBFLAGS) -D__ACQ196__
#CPPFLAGS += -I$(INCLUDEDIR) -I${DTINC} -I${DTLINC}
#CPPFLAGS +=  -I${DTINC} -I${DTLINC}

#APPS=	llcontrol24 llcontrol26 channelize.196 flatline build_prams
APPS=	llcontrol26 channelize.196 flatline

TS=setup.clocks2 setup.clocksN aoramps README.* capture \
	RELEASE LICENSE Doxyfile fetchbs.m set.sync_2v setup.ao32

TS200=$(TS) $(APPS)

COREOBJS=llcontrol-core.o \
	llcontrol-syncECM-core.o \
	llcontrol-sync2v-core.o \
	llcontrol-sync2VAO32-core.o \
	llcontrol-sync2VRFM-core.o \
	llcontrol-acq216-core.o

LLOBJS=llcontrol.o $(COREOBJS) llif.o llprotocol.o x86timer.o
LLOBJS26=llcontrol.o $(COREOBJS) llif26.o llprotocol.o x86timer.o

TAROPTS="--exclude=CVS --exclude=REL"

ACQ200_SUPPORT=PRAMS CONSTS 
PACKLIST=Makefile *.[ch] $(TS) html
PACKLIST200=Makefile *.[ch] *.cpp $(TS200) $(ACQ200_SUPPORT)

DATECODE := $(shell date +%Y%m%d%H%M)
WEBDOC="dtacq@margum.pair.com:public_html/resources/LLCONTROL"

all: .depend  $(APPS)

doc:
	doxygen

release install: all
	cp $(APPS) ../../bin

depend .depend dep:
	$(CC) $(CPPFLAGS) -M *.c > .depend


llcontrol24: $(LLOBJS)
	$(LINK.c) -o $@ $^ -lpopt
	@rm -f llcontrol
	ln -s llcontrol24 llcontrol

llcontrol26: $(LLOBJS26)
	$(LINK.c) -o $@ $^ -lpopt
	@rm -f llcontrol
	ln -s llcontrol26 llcontrol

build_prams: build_prams.o
	$(LINK.cpp) -o $@ $^ -lpopt

timertest: timertest.o x86timer.o
	$(LINK.c) -o $@ $^ -lpopt
test:
	echo result:`date +%Y%m%d%H%M`

fetch:
	@cp ../../../../include/acq32busprot.h .
	@cp ../../../../include/local.h .
	@cp ../../include/acq32ioctl.h .


package: fetch
	@echo Building REL/llcontrol.$(DATECODE).tgz
	doxygen
	cp PRAMS/OCTAVE/fetchbs.m .
	cvs tag P$(DATECODE)
	(cd ..;tar cvzf LOWLATENCY/REL/llcontrol.$(DATECODE).tgz $(TAROPTS) \
		$(PACKLIST:%=LOWLATENCY/%))
	@echo Building REL/llcontrol.$(DATECODE).tgz COMPLETE

webdoc:
	rsync -avze ssh html/ $(WEBDOC)/html
	rsync -avze ssh README* $(WEBDOC)/	
package216:
	@echo Building REL/llcontrol.acq200.$(DATECODE).tgz
	(cd ..;tar cvzf LOWLATENCY/REL/llcontrol.acq200.$(DATECODE).tgz $(TAROPTS) \
		$(PACKLIST200:%=LOWLATENCY/%))
	@echo Building llcontrol.acq200.$(DATECODE).tgz done

clean:
	rm -f $(APPS)
	rm -f *.o
	rm -f .depend

distclean:
	@rm acq32busprot.h
	@rm local.h
	@rm acq32ioctl.h

ifeq (.depend,$(wildcard .depend))
include .depend
endif


