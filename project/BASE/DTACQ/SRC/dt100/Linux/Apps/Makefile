# Makefile for Apps


all install release clean:
# HTSTREAM disabled: its Sink API takes `unsigned` for pointer args
# (sink.h:46 and every override); g++ errors on the 64-bit truncation.
# Uncomment once the Sink API is ported to uintptr_t.
#	(cd HTSTREAM; make $@)
	(cd LOWLATENCY; make $@)
	(cd BIGDUMPTEST; make $@)
	(cd CAL;make $@)
	(cd DUMP; make $@)
	(cd FMU; make $@)
	(cd MISC-TEST; make $@)
