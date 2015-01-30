##
## libz - zlib compression library
##

## Process this file with automake to produce Makefile.in

SUBDIRS =		
DIST_SUBDIRS =		msvc++

lib_LTLIBRARIES =	libz.la
include_HEADERS =	zlib.h zconf.h

libz_la_SOURCES =	adler32.c compress.c crc32.c gzio.c inflate.c  \
			uncompr.c  \
			deflate.c infblock.c infcodes.c inffast.c inftrees.c  \
			infutil.c trees.c zutil.c  \
			deflate.h infblock.h infcodes.h inffast.h inftrees.h  \
			infutil.h trees.h zutil.h  \
			inffixed.h zconf.h zlib.h

EXTRA_DIST =		README ChangeLog

libz_la_LDFLAGS =	-version-info 0:0:0

libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck

again:
	$(MAKE) clean
	$(MAKE)

.PHONY: again
