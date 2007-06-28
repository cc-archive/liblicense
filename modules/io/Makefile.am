if HAVE_EXEMPI
lib_LTLIBRARIES = \
	exempi.la \
	sidecar_xmp.la

exempi_la_SOURCES = exempi.c
exempi_la_LDFLAGS = -no-undefined -module -avoid-version
exempi_la_LIBADD = $(EXEMPI_LIBS)
exempi_la_CFLAGS = $(EXEMPI_CFLAGS)

sidecar_xmp_la_SOURCES = sidecar_xmp.c
sidecar_xmp_la_LDFLAGS = -no-undefined -module -avoid-version
sidecar_xmp_la_LIBADD = $(EXEMPI_LIBS)
sidecar_xmp_la_CFLAGS = $(EXEMPI_CFLAGS)
endif
