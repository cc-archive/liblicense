# Creative Commons has made the contents of this file
# available under a CC-GNU-GPL license:
#
# http://creativecommons.org/licenses/GPL/2.0/
#
# A copy of the full license can be found as part of this
# distribution in the file COPYING.
# 
# You may use the liblicense software in accordance with the
# terms of that license. You agree that you are solely 
# responsible for your use of the liblicense software and you
# represent and warrant to Creative Commons that your use
# of the liblicense software will comply with the CC-GNU-GPL.
#
# Copyright 2007, Creative Commons, www.creativecommons.org.
# Copyright 2007, Jason Kivlighn.

if HAVE_EXEMPI
EXEMPI_MODULES = exempi.la sidecar_xmp.la

exempi_la_SOURCES = exempi.c
exempi_la_LDFLAGS = -no-undefined -module -avoid-version
exempi_la_LIBADD = $(EXEMPI_LIBS)
exempi_la_CFLAGS = $(EXEMPI_CFLAGS)

sidecar_xmp_la_SOURCES = sidecar_xmp.c
sidecar_xmp_la_LDFLAGS = -no-undefined -module -avoid-version
sidecar_xmp_la_LIBADD = $(EXEMPI_LIBS)
sidecar_xmp_la_CFLAGS = $(EXEMPI_CFLAGS)
endif

libdir = @libdir@/liblicense/io
lib_LTLIBRARIES = \
	$(EXEMPI_MODULES) \
	stub.la

stub_la_SOURCES = stub.c
stub_la_LDFLAGS = -no-undefined -module -avoid-version
