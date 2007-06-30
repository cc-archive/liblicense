SUBDIRS = . xdgmime licenses modules bindings tests utils

libdir = @libdir@/liblicense
lib_LTLIBRARIES = liblicense.la

liblicense_la_CFLAGS = $(RAPTOR_CFLAGS)
liblicense_la_LIBADD = $(RAPTOR_LIBS)
liblicense_la_SOURCES = \
	read_license.c \
	write_license.c \
	system_default.c \
	module_wrangler.c \
	system_licenses.c \
	list.c
