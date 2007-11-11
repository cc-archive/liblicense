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
# Copyright (C) 2007 Peter Miller

SUBDIRS = .

INCLUDES = -I$(srcdir) -I$(top_srcdir)/xdgmime

lib_LTLIBRARIES = liblicense.la

liblicense_la_LDFLAGS = -version-info @LIBLICENSE_VERSION_INFO@

liblicense_la_CFLAGS = $(RAPTOR_CFLAGS)
liblicense_la_LIBADD = $(RAPTOR_LIBS) \
					   $(top_builddir)/xdgmime/libxdgmime.la \
					   $(SQLITE_LIBS)

if HAVE_SQLITE
licenses = licenses_cached.c
else
licenses = licenses_uncached.c
endif

#
# There is a natural tension between having a small number of files to
# deal with, and having one function per file so that (when statically
# linked) the linker can leave out everything not used and thus produce
# smaller executables.  It makes no difference for shared libraries.
#
liblicense_la_SOURCES = \
	license_chooser.c \
	license_get_creator.c \
	license_info.c \
	license_print_info.c \
	list.c \
	module_wrangler.c \
	read_license.c \
	system_default.c \
	write_license.c \
	$(licenses)

noinst_HEADERS = modules.h

liblicensedir=$(includedir)/@LIBLICENSE_INCLUDE_BASE@
liblicense_HEADERS = liblicense.h
