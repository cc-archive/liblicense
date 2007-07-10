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

SUBDIRS = xdgmime . licenses modules bindings tests utils

libdir = @libdir@/liblicense
lib_LTLIBRARIES = liblicense.la

liblicense_la_CFLAGS = $(RAPTOR_CFLAGS)
liblicense_la_LIBADD = $(RAPTOR_LIBS) \
											 xdgmime/libxdgmime.la
liblicense_la_SOURCES = \
	read_license.c \
	write_license.c \
	system_default.c \
	module_wrangler.c \
	system_licenses.c \
	list.c

include_HEADERS = liblicense.h

registrydir=@libdir@/liblicense/io
registry_DATA=registry
