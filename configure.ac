# -*- Autoconf -*-
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
])

AC_CONFIG_HEADERS([config.h])

AC_OUTPUT

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
"
