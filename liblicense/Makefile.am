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

lib_LTLIBRARIES = liblicense.la

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
	read_license.c \
	write_license.c \
	system_default.c \
	module_wrangler.c \
	license_info.c \
	license_chooser.c \
	license_print_info.c \
	$(licenses) \
	list.c

include_HEADERS = liblicense.h
