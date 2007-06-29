TESTS = test test_list test_default test_mw

check_PROGRAMS = $(TESTS)

test_SOURCES = \
	test.c

test_list_SOURCES = \
	list_test.c

test_LDADD = $(top_builddir)/liblicense.la

test_list_LDADD = $(top_builddir)/liblicense.la

test_default_SOURCES = default_test.c
test_default_LDADD = $(top_builddir)/liblicense.la

test_mw_SOURCES = mw_test.c
test_mw_LDADD = $(top_builddir)/liblicense.la
