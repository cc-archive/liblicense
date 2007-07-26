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

TESTS = test test_list test_default test_mw test_read test_info test_write

check_PROGRAMS = $(TESTS)

test_SOURCES = \
	test.c

test_list_SOURCES = \
	list_test.c

test_LDADD = $(top_builddir)/liblicense/liblicense.la

test_list_LDADD = $(top_builddir)/liblicense/liblicense.la

test_default_SOURCES = default_test.c
test_default_LDADD = $(top_builddir)/liblicense/liblicense.la

test_mw_SOURCES = mw_test.c
test_mw_LDADD = $(top_builddir)/liblicense/liblicense.la

test_read_SOURCES = read_test.c
test_read_LDADD = $(top_builddir)/liblicense/liblicense.la

test_info_SOURCES = info_test.c
test_info_LDADD = $(top_builddir)/liblicense/liblicense.la

test_write_SOURCES = write_test.c
test_write_LDADD = $(top_builddir)/liblicense/liblicense.la
