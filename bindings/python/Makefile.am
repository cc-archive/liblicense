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

pkgpyexec_LTLIBRARIES = liblicense.la
pkgpython_PYTHON = __init__.py
AM_CPPFLAGS = $(PYTHON_INCLUDES) -I$(top_builddir)/liblicense
AM_LDFLAGS = -module -avoid-version \
             -export-symbols-regex initliblicense \
             $(top_builddir)/liblicense/liblicense.la

liblicense_la_SOURCES = liblicense.c

EXTRA_DIST = tests/test.py
