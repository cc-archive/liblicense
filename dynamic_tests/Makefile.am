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
# Copyright (C) 2007 Peter Miller

AM_CFLAGS = -I$(top_srcdir)/liblicense
AM_LDFLAGS= -llicense

THISDIR = $(PWD)

DEBUG_LL_MODULES_PATH=$(THISDIR)/_prefix/$(prefix)

TESTS_ENVIRONMENT = LL_IO_MODULE_DIR=$(DEBUG_LL_MODULES_PATH)/lib/liblicense/@VERSION@/io/ \
	LL_CONFIG_MODULE_DIR=$(DEBUG_LL_MODULES_PATH)/lib/liblicense/@VERSION@/config/ \
	LL_LICENSE_DIR=$(DEBUG_LL_MODULES_PATH)/share/liblicense/licenses/ \
	LD_LIBRARY_PATH=../liblicense/.libs/ \
	@VALGRIND@
TESTS = test test_list test_default test_mw test_read test_info test_write test_chooser test_predicate

all-local:
	mkdir -p _prefix
	(cd $(top_builddir)/modules ; DESTDIR=$(THISDIR)/_prefix make install)
	(cd $(top_builddir)/licenses ; DESTDIR=$(THISDIR)/_prefix make install)

clean-local:
	rm -fr _prefix
	rm -f license_me.xmp

check_PROGRAMS = $(TESTS)

test_predicate_SOURCES = \
	predicate_test.c

test_SOURCES = \
	test.c

test_list_SOURCES = \
	list_test.c

test_default_SOURCES = default_test.c

test_mw_SOURCES = mw_test.c

test_read_SOURCES = read_test.c

test_info_SOURCES = info_test.c

test_write_SOURCES = write_test.c

test_chooser_SOURCES = chooser_test.c