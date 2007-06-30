pyexec_LTLIBRARIES = liblicense.la

AM_CPPFLAGS = $(PYTHON_INCLUDES)
AM_LDFLAGS = -module -avoid-version \
             -export-symbols-regex initliblicense \
             $(top_builddir)/liblicense.la

liblicense_la_SOURCES = liblicense.c
