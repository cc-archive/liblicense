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
# Copyright 2007, Alex Roberts.

#rb_LTLIBRARIES = liblicense.la

AM_CPPFLAGS = $(RUBY_CFLAGS)
AM_LDFLAGS = $(RUBY_LDFLAGS) \
             $(RUBY_LIBS)    \
             $(top_builddir)/liblicense.la

liblicense_la_SOURCES = liblicense.c
liblicense_la_LDFLAGS = $(RUBY_LDFLAGS) -module -avoid-version
liblicense_la_LIBADD = $(RUBY_LIBS)
liblicense_la_CFLAGS = $(RUBY_CFLAGS)

libdir = $(RUBY_ARCHDIR)
lib_LTLIBRARIES = liblicense.la

rbdir = $(RUBY_LIBDIR)

rb_DATA = lib/liblicense.rb

EXTRA_DIST = license.h  \
             test.rb    \
             $(rb_DATA)
