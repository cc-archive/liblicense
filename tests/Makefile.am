TESTS = test test_list test_default test_mw test_read

check_PROGRAMS = $(TESTS)

test_SOURCES = \
	test.c

test_list_SOURCES = \
	list_test.c

test_LDADD = $(top_builddir)/liblicense.la $(top_builddir)/xdgmime/libxdgmime.la

test_list_LDADD = $(top_builddir)/liblicense.la $(top_builddir)/xdgmime/libxdgmime.la

test_default_SOURCES = default_test.c
test_default_LDADD = $(top_builddir)/liblicense.la $(top_builddir)/xdgmime/libxdgmime.la

test_mw_SOURCES = mw_test.c
test_mw_LDADD = $(top_builddir)/liblicense.la $(top_builddir)/xdgmime/libxdgmime.la

test_read_SOURCES = read_test.c
test_read_LDADD = $(top_builddir)/liblicense.la $(top_builddir)/xdgmime/libxdgmime.la
