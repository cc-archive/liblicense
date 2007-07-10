# -*- Autoconf -*-
# Creative Commons has made the contents of this file
# available under a CC-GNU-GPL license:
#
# http://creativecommons.org/licenses/GPL/2.0/
#
# A copy of the full license can be found as part of this
# distribution in the file COPYING.
# 
# You may use the liblicense software in accordance with the
# terms of that license. You agree that you are solely 
# responsible for your use of the liblicense software and you
# represent and warrant to Creative Commons that your use
# of the liblicense software will comply with the CC-GNU-GPL.
#
# Copyright 2007, Creative Commons, www.creativecommons.org.
# Copyright 2007, Jason Kivlighn.
# Process this file with autoconf to produce a configure script.
# This file is part of Liblicense.

AC_PREREQ(2.57)
AC_INIT([liblicense],[0.0.1],[cc-devel@])
AM_INIT_AUTOMAKE([dist-bzip2])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL

AM_CHECK_PYTHON_HEADERS
if test "x$PYTHON_INCLUDES" != "x"; then
  PYTHON_DIR=python
fi
AC_SUBST(PYTHON_DIR)

CFLAGS="$CFLAGS"

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h string.h])

dnl Library Checks
RAPTOR_REQUIRED=1.0

PKG_CHECK_MODULES(RAPTOR, [ raptor >= $RAPTOR_REQUIRED ])
AC_SUBST(RAPTOR_CFLAGS)
AC_SUBST(RAPTOR_LIBS)

####################################################################
# Compiler warning checks
####################################################################
AC_ARG_ENABLE(warnings,
              AC_HELP_STRING([--disable-warnings], [disable GCC warnings]),,
              [enable_warnings=yes])

if test "x$enable_warnings" = "xyes"; then
   CFLAGS="\
   -Wall \
   -Wchar-subscripts \
   -Wnested-externs \
   -Wpointer-arith \
   -Wsign-compare \
   $CFLAGS"
fi

####################################################################
# Compiler generate debug code
####################################################################


AC_ARG_ENABLE([debug_code],
              AC_HELP_STRING([--enable-debug-code], [build with debug symbols]),,
              [enable_debug_code=no])

if test "x$enable_debug_code" = "xyes"; then
	CFLAGS="-g $CFLAGS"
fi

##################################################################
# check for libxml2
##################################################################

LIBXML2_REQUIRED=0.6

AC_ARG_ENABLE(libxml2, AC_HELP_STRING([--disable-libxml2],[Disable HTML/XML embedders]),,[enable_libxml2=yes])
if test "x$enable_libxml2" = "xyes"; then
	PKG_CHECK_MODULES(LIBXML2, [
		libxml-2.0 >= $LIBXML2_REQUIRED],
		[have_libxml2=yes] , [have_libxml2=no])
	AC_SUBST(LIBXML2_CFLAGS)
	AC_SUBST(LIBXML2_LIBS)
else
	have_libxml2="no (disabled)"
fi
AM_CONDITIONAL(HAVE_LIBXML2, test "$have_libxml2" = "yes")
test "$have_libxml2" = "yes" && AC_DEFINE(HAVE_LIBXML2, [], [Define if we have libxml2])

##################################################################
# check for exempi
##################################################################

EXEMPI_REQUIRED=1.99.3

AC_ARG_ENABLE(xmp, AC_HELP_STRING([--disable-xmp], [Disable XMP extraction]),,[enable_xmp=yes])
if test "x$enable_xmp" = "xyes"; then
	PKG_CHECK_MODULES(EXEMPI,[
		exempi-2.0 >= $EXEMPI_REQUIRED],
		[have_exempi=yes] , [have_exempi=no])
	AC_SUBST(EXEMPI_CFLAGS)
	AC_SUBST(EXEMPI_LIBS)
else
	have_exempi="no (disabled)"
fi
AM_CONDITIONAL(HAVE_EXEMPI, test "$have_exempi" = "yes")
test "$have_exempi" = "yes" && AC_DEFINE(HAVE_EXEMPI, [], [Define if we have exempi])

##################################################################
# check for id3lib
##################################################################

AC_ARG_ENABLE(id3, AC_HELP_STRING([--disable-id3], [Disable ID3 embedding]),,[enable_id3=yes])
if test "x$enable_id3" = "xyes"; then
	AC_CHECK_LIB(id3, ID3Tag_New,
			[AC_CHECK_HEADERS(id3.h,
			[
			have_id3lib=yes
			],[have_id3lib=no])],
			[have_id3lib=no])
	ID3LIB_LIBS="-lid3"
	AC_SUBST(ID3LIB_LIBS)
else
	have_id3lib="no (disabled)"
fi
AM_CONDITIONAL(HAVE_ID3LIB, test "$have_id3lib" = "yes")
test "$have_id3lib" = "yes" && AC_DEFINE(HAVE_ID3LIB, [], [Define if we have id3lib])

##################################################################
# check for FLAC
##################################################################

AC_ARG_ENABLE(FLAC, AC_HELP_STRING([--disable-FLAC], [Disable FLAC embedding]),,[enable_FLAC=yes])
if test "x$enable_FLAC" = "xyes"; then
	AM_PATH_LIBFLAC
	if test "x$no_libFLAC" = "x" ; then
		have_flac="yes"
	else
		have_flac="no"
	fi
else
	have_flac="no (disabled)"
fi
AM_CONDITIONAL(HAVE_FLAC, test "$have_flac" = "yes")
test "$have_flac" = "yes" && AC_DEFINE(HAVE_FLAC, [], [Define if we have FLAC])

##################################################################
# check for vorbis
##################################################################

VORBIS_REQUIRED=1.0

AC_ARG_ENABLE(vorbis, AC_HELP_STRING([--disable-vorbis], [Disable Ogg Vorbis embedding]),,[enable_vorbis=yes])
if test "x$enable_vorbis" = "xyes"; then
	PKG_CHECK_MODULES(VORBIS,[
		vorbisfile >= $VORBIS_REQUIRED],
		[have_vorbis=yes] , [have_vorbis=no])
	AC_SUBST(VORBIS_CFLAGS)
	AC_SUBST(VORBIS_LIBS)
else
	have_vorbis="no (disabled)"
fi

AM_CONDITIONAL(HAVE_VORBIS, test "$have_vorbis" = "yes")
test "$have_vorbis" = "yes" && AC_DEFINE(HAVE_VORBIS, [], [Define if we have vorbisfile])

##################################################################
# define paths to licenses and modules
##################################################################
AS_AC_EXPAND(DATADIR, $datadir)
AS_AC_EXPAND(LIBDIR, $libdir)
AC_DEFINE_UNQUOTED([LICENSE_DIR],["${DATADIR}/liblicense/licenses/"],[Location of license rdf files.])
AC_DEFINE_UNQUOTED(LIBLICENSE_IO_MODULE_DIR,"${LIBDIR}/liblicense/io/",[Location of liblicense io modules.])
AC_DEFINE_UNQUOTED([LIBLICENSE_CONFIG_MODULE_DIR],["${LIBDIR}/liblicense/config/"],[Location of liblicense config modules.])

AC_CONFIG_FILES([
	Makefile
	licenses/Makefile
	modules/Makefile
	modules/io/Makefile
	modules/config/Makefile
	bindings/Makefile
	bindings/python/Makefile
	tests/Makefile
	xdgmime/Makefile
	utils/Makefile
])

AC_CONFIG_HEADERS([config.h])

AC_OUTPUT
#Output the registry.  Its hackish because the registry itsself is and it'll
# change soon.
rm -f registry
if test "x$have_vorbis" = "xyes"
then
	echo "vorbis.so audio/x-vorbis+ogg application/ogg audio/x-vorbis" >> registry
fi

if test "x$have_exempi" = "xyes"
then
	echo "exempi.so image/jpeg image/png image/tiff application/pdf video/x-msvideo video/quicktime audio/x-wav" >> registry
	echo "sidecar_xmp.so" >> registry
fi

if test "x$have_id3lib" = "xyes"
then
	echo "id3.so audio/mpeg" >> registry
fi
if test "x$have_flac" = "xyes"
then
	echo "flac.so audio/x-flac" >> registry
fi
if test "x$have_libxml2" = "xyes"
then
	echo "raptor.so image/svg+xml" >> registry
fi
echo "stub.so" >> registry

echo "
Liblicense-$VERSION:

	prefix:					${prefix}
	source code location:			${srcdir}
	compiler:				${CC}
	enable gcc warnings:			$enable_warnings
	build with debug symbols:		$enable_debug_code

I/O Modules:
	exempi:					$have_exempi
	sidecar:				$have_exempi
	id3:					$have_id3lib
	vorbis:					$have_vorbis
	FLAC:					$have_flac
	raptor:					$have_libxml2
"
