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
# Copyright 2007, Scott Shawcroft.
# Copyright 2007, Jason Kivlighn.


INCLUDES = -I$(top_srcdir)/liblicense

UTILS = license

bin_PROGRAMS = $(UTILS)

license_SOURCES = \
	license.c

license_LDADD = $(top_builddir)/liblicense/liblicense.la $(top_builddir)/xdgmime/libxdgmime.la

check:
	./license_test
