# Creative Commons has made the contents of this file
# available under a CC-GNU-LGPL license:
#
# http://creativecommons.org/licenses/LGPL/2.1/
#
# A copy of the full license can be found as part of this
# distribution in the file COPYING.
# 
# You may use the liblicense software in accordance with the
# terms of that license. You agree that you are solely 
# responsible for your use of the liblicense software and you
# represent and warrant to Creative Commons that your use
# of the liblicense software will comply with the CC-GNU-LGPL.
#
# Copyright 2007, Creative Commons, www.creativecommons.org.
# Copyright 2007, Jason Kivlighn.


INCLUDES=-I$(top_srcdir)/liblicense

if HAVE_EXEMPI
EXEMPI_MODULES = exempi.la sidecar_xmp.la

exempi_la_SOURCES = shared_xmp.h shared_xmp.c exempi.c
exempi_la_LDFLAGS = -no-undefined -module -avoid-version
exempi_la_LIBADD = $(EXEMPI_LIBS)
exempi_la_CFLAGS = $(EXEMPI_CFLAGS)

sidecar_xmp_la_SOURCES = shared_xmp.c sidecar_xmp.c
sidecar_xmp_la_LDFLAGS = -no-undefined -module -avoid-version
sidecar_xmp_la_LIBADD = $(EXEMPI_LIBS)
sidecar_xmp_la_CFLAGS = $(EXEMPI_CFLAGS)
endif

if HAVE_ID3LIB
ID3LIB_MODULES = id3.la

id3_la_SOURCES = id3.c
id3_la_LIBADD = $(ID3LIB_LIBS) 
id3_la_LDFLAGS = -no-undefined -module -avoid-version
endif

if HAVE_VORBIS
VORBIS_MODULES = vorbis.la

vorbis_la_SOURCES = vorbis.c vcedit.c vcedit.h
vorbis_la_LIBADD = $(VORBIS_LIBS) 
vorbis_la_CFLAGS = $(VORBIS_CFLAGS)
vorbis_la_LDFLAGS = -no-undefined -module -avoid-version
noinst_HEADERS = vcedit.h
endif

if HAVE_FLAC
FLAC_MODULES = flac.la

flac_la_SOURCES = flac.c
flac_la_LIBADD = $(LIBFLAC_LIBS) 
flac_la_CFLAGS = $(LIBFLAC_CFLAGS)
flac_la_LDFLAGS = -no-undefined -module -avoid-version
endif

if HAVE_LIBXML2
RAPTOR_MODULES = raptor.la

raptor_la_SOURCES = raptor.c
raptor_la_LDFLAGS = -no-undefined -module -avoid-version
raptor_la_LIBADD =  $(RAPTOR_LIBS) $(LIBXML2_LIBS) 
raptor_la_CFLAGS = $(RAPTOR_CFLAGS) $(LIBXML2_CFLAGS)
endif

if HAVE_TAGLIB
TAGLIB_MODULES = taglib.la

taglib_la_SOURCES = taglib.cpp
taglib_la_LDFLAGS = -no-undefined -module -avoid-version
taglib_la_LIBADD = $(TAGLIB_LIBS) 
taglib_la_CXXFLAGS = $(TAGLIB_CFLAGS)
endif

if HAVE_LIBGSF
LIBGSF_MODULES = gsf.la

gsf_la_SOURCES = gsf.c
gsf_la_LDFLAGS = -no-undefined -module -avoid-version
gsf_la_LIBADD = $(LIBGSF_LIBS) 
gsf_la_CFLAGS = $(LIBGSF_CFLAGS)
endif

AM_CPPFLAGS = -I$(top_builddir)/liblicense

libdir = @libdir@/liblicense/@VERSION@/io
lib_LTLIBRARIES = \
	$(EXEMPI_MODULES) \
	$(ID3LIB_MODULES) \
	$(VORBIS_MODULES) \
	$(FLAC_MODULES) \
	$(RAPTOR_MODULES) \
	$(TAGLIB_MODULES) \
	$(LIBGSF_MODULES) \
	stub.la

stub_la_SOURCES = stub.c
stub_la_LDFLAGS = -no-undefined -module -avoid-version
stub_la_LIBADD  = 
