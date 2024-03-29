# -*- Autoconf -*-
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
# Copyright 2007, Jason Kivlighn.
# Copyright (C) 2007 Peter Miller
# Process this file with autoconf to produce a configure script.
# This file is part of Liblicense.

AC_PREREQ(2.57)
AC_INIT([liblicense],[0.8.1],[cc-devel@lists.ibiblio.org])
AM_INIT_AUTOMAKE([dist-bzip2])

dnl
dnl Libtool options
dnl
LIBTOOL_EXPORT_OPTIONS="-export-symbols-regex '^(ll|LL)_.*'"
AC_SUBST(LIBTOOL_EXPORT_OPTIONS)

dnl ---------------------------------------------------------------------------
dnl Advanced information about versioning:
dnl   * "Writing shared libraries" by Mike Hearn
dnl         http://navi.cx/~mike/writing-shared-libraries.html
dnl   * libtool.info chapter "Versioning"
dnl   * libtool.info chapter "Updating library version information"
dnl ---------------------------------------------------------------------------
dnl Versioning:
dnl  - CURRENT (Major):  Increment if the interface has changes. AGE is always
dnl                      *changed* at the same time.
dnl  - AGE (Micro):      Increment if any interfaces have been added; set to 0
dnl                      if any interfaces have been removed. Removal has
dnl                      precedence over adding, so set to 0 if both happened.
dnl                      It denotes upward compatibility.
dnl  - REVISION (Minor): Increment any time the source changes; set to
dnl                      0 if you incremented CURRENT.
dnl
dnl  To summarize. Any interface *change* increment CURRENT. If that interface
dnl  change does not break upward compatibility (ie it is an addition),
dnl  increment AGE, Otherwise AGE is reset to 0. If CURRENT has changed,
dnl  REVISION is set to 0, otherwise REVISION is incremented.
dnl ---------------------------------------------------------------------------
dnl 0.6.1 is the revision 1.0.0
dnl 0.7.0 is the revision 2.0.0
dnl 0.8 is the revision 3.0.0
AC_SUBST([LIBLICENSE_REVISION], [0])
AC_SUBST([LIBLICENSE_AGE],      [0])
AC_SUBST([LIBLICENSE_CURRENT],  [3])
AC_SUBST([LIBLICENSE_CURRENT_MIN],
         [`expr $LIBLICENSE_CURRENT - $LIBLICENSE_AGE`])
AC_SUBST([LIBLICENSE_VERSION_INFO],
         [$LIBLICENSE_CURRENT:$LIBLICENSE_REVISION:$LIBLICENSE_AGE])

dnl the major version. used for package names, etc
LIBLICENSE_MAJOR_VERSION=3.0
AC_SUBST(LIBLICENSE_MAJOR_VERSION)
LIBLICENSE_INCLUDE_BASE=liblicense-$LIBLICENSE_MAJOR_VERSION
AC_SUBST(LIBLICENSE_INCLUDE_BASE)

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL
AC_ARG_ENABLE(python,
			  AC_HELP_STRING([--disable-python], [disable Python binding]),,
			  [enable_python=yes])
if test "x$enable_python" = "xyes"; then
	AM_CHECK_PYTHON_HEADERS
	if test "x$PYTHON_INCLUDES" != "x"; then
		save_CPPFLAGS="$CPPFLAGS"
		CPPFLAGS="$CPPFLAGS $PYTHON_INCLUDES"
		AC_CHECK_HEADER([Python.h],
			[PYTHON_DIR=python],
			[enable_python="no (Python.h not found)"]
			)
		CPPFLAGS="$save_CPPFLAGS"
	else
		enable_python="no (Python not found)"
	fi
fi
AC_SUBST(PYTHON_DIR)

AC_ARG_ENABLE(ruby,
			  AC_HELP_STRING([--disable-ruby], [disable Ruby binding]),,
			  [enable_ruby=yes])

if test "x$enable_ruby" = "xyes"; then
  # All required to build Ruby binding
  # Inspired from Ruby's own "mkmf," not exactly clean, I know.
  AC_PATH_PROG(RUBY, ruby, none)
  if test "$RUBY" != "none"; then
	if "$RUBY" -r mkmf -e 'exit(have_func("rb_ary_push") ? 0 : 1)'; then
	  RUBY_DIR=ruby
	  RUBY_ARCHDIR=`"$RUBY" -rrbconfig -e 'puts Config::CONFIG[["sitearchdir"]]'`
	  RUBY_LIBDIR=`"$RUBY" -rrbconfig -e 'puts Config::CONFIG[["sitelibdir"]]'`
	  RUBY_LIBS=`"$RUBY" -rrbconfig -e 'print Config::CONFIG[["LIBS"]]; puts " -l" + Config::CONFIG[["RUBY_SO_NAME"]]'`
	  RUBY_CFLAGS=`"$RUBY" -rrbconfig -e 'print Config::CONFIG[["CFLAGS"]]; puts " -I" + Config::CONFIG[["archdir"]]'`
	  RUBY_LDFLAGS=`"$RUBY" -rrbconfig -e 'puts Config::CONFIG[["LDFLAGS"]]'`
	else
	  RUBY="none"
	  AC_MSG_WARN([The detected Ruby is too old, or no development files installed.])
	fi
  fi
fi
AC_SUBST(RUBY_DIR)
AC_SUBST(RUBY_ARCHDIR)
AC_SUBST(RUBY_LIBDIR)
AC_SUBST(RUBY_LIBS)
AC_SUBST(RUBY_CFLAGS)
AC_SUBST(RUBY_LDFLAGS)

CFLAGS="$CFLAGS"

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h string.h])

# xdgmime needs mmap
AC_CHECK_FUNCS(mmap)

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
   AC_ADD_CFLAGS([-Wall])
   AC_ADD_CFLAGS([-Wextra])
# FIXME: Enable this when upstream xdgmime's code compiles without warning
#   AC_ADD_CFLAGS([-Werror])
   AC_ADD_CFLAGS([-Wchar-subscripts])
   AC_ADD_CFLAGS([-Wnested-externs])
   AC_ADD_CFLAGS([-Wpointer-arith])
   AC_ADD_CFLAGS([-Wsign-compare])
   AC_ADD_CFLAGS([-Wshadow])
   AC_ADD_CFLAGS([-std=gnu89])
   AC_ADD_CFLAGS([-pedantic])
# Below is for dirent.h include
   AC_ADD_CFLAGS([-D_SVID_SOURCE])
   AC_ADD_CFLAGS([-Wdeclaration-after-statement])
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

AC_ARG_ENABLE(libxml2, AC_HELP_STRING([--disable-libxml2],[Disable XML embedders]),,[enable_libxml2=yes])
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

EXEMPI_REQUIRED=1.99.9

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
# check for libgsf
##################################################################

LIBGSF_REQUIRED=1.13

AC_ARG_ENABLE(gsf, AC_HELP_STRING([--disable-gsf], [Disable MS Office (Pre-2007) embedding]),,[enable_gsf=yes])
if test "x$enable_gsf" = "xyes"; then
	PKG_CHECK_MODULES(LIBGSF,[
		libgsf-1 >= $LIBGSF_REQUIRED],
		[have_libgsf=yes] , [have_libgsf=no])
	AC_SUBST(LIBGSF_CFLAGS)
	AC_SUBST(LIBGSF_LIBS)
else
	have_libgsf="no (disabled)"
fi
AM_CONDITIONAL(HAVE_LIBGSF, test "$have_libgsf" = "yes")
test "$have_libgsf" = "yes" && AC_DEFINE(HAVE_LIBGSF, [], [Define if we have libgsf])

##################################################################
# check for taglib
##################################################################
AC_PROG_CXX

TAGLIB_REQUIRED=1.3

AC_ARG_ENABLE(taglib, AC_HELP_STRING([--disable-taglib], [Disable TagLib embedding]),,[enable_taglib=yes])
if test "x$enable_taglib" = "xyes"; then
	PKG_CHECK_MODULES(TAGLIB,[
		taglib >= $TAGLIB_REQUIRED],
		[have_taglib=yes] , [have_taglib=no])
	AC_SUBST(TAGLIB_CFLAGS)
	AC_SUBST(TAGLIB_LIBS)
else
	have_taglib="no (disabled)"
fi

AM_CONDITIONAL(HAVE_TAGLIB, test "$have_taglib" = "yes")
test "$have_taglib" = "yes" && AC_DEFINE(HAVE_TAGLIB, [], [Define if we have taglib])

##################################################################
# check for sqlite3
##################################################################
AC_PROG_CXX

SQLITE_REQUIRED=3.3

AC_ARG_ENABLE(sqlite, AC_HELP_STRING([--disable-sqlite], [Disable sqlite3 caching]),,[enable_sqlite=yes])
if test "x$enable_sqlite" = "xyes"; then
	PKG_CHECK_MODULES(SQLITE,[
		sqlite3 >= $SQLITE_REQUIRED],
		[have_sqlite=yes] , [have_sqlite=no])
	AC_SUBST(SQLITE_CFLAGS)
	AC_SUBST(SQLITE_LIBS)
else
	have_sqlite="no (disabled)"
fi

AM_CONDITIONAL(HAVE_SQLITE, test "$have_sqlite" = "yes")
test "$have_sqlite" = "yes" && AC_DEFINE(HAVE_SQLITE, [], [Define if we have sqlite3])

##################################################################
# detect valgrind
##################################################################
AC_CHECK_PROG(VALGRIND, valgrind, valgrind)
dnl don't need that YET
dnl AC_CHECK_HEADERS(valgrind/memcheck.h)
if test "x$VALGRIND" = "x"; then
        AC_MSG_WARN([Valgrind is missing. make check will run without])
fi
if test "x$VALGRIND" != "x"; then
dnl due to stupid m4 I had to double the [ and ] in the regexp
        vg_ver=`valgrind --version | sed -e  's/.*-\([[0-9]]\)\.[[0-9]].*/\1/'`
        VALGRIND="$VALGRIND --tool=memcheck --leak-check=yes --num-callers=10"
        if test "$vg_ver" != "3" ; then
           VALGRIND="$VALGRIND --logfile-fd=-1"
        fi
fi
AC_SUBST(VALGRIND)


##################################################################
# define paths to licenses and modules
##################################################################
AS_AC_EXPAND(DATADIR, $datadir)
AS_AC_EXPAND(LIBDIR, $libdir)
AC_DEFINE_UNQUOTED([LICENSE_DIR],["${DATADIR}/liblicense/licenses/"],[Location of license rdf files.])
AC_DEFINE_UNQUOTED(LIBLICENSE_IO_MODULE_DIR,"${LIBDIR}/liblicense/${VERSION}/io/",[Location of liblicense io modules.])
AC_DEFINE_UNQUOTED([LIBLICENSE_CONFIG_MODULE_DIR],["${LIBDIR}/liblicense/${VERSION}/config/"],[Location of liblicense config modules.])

USE_NLS=yes
AC_CHECK_PROGS(GMSGFMT, gmsgfmt msgfmt, gmsgfmt)
GETTEXT_PACKAGE=$PACKAGE
DATADIRNAME=share
AC_SUBST(USE_NLS)
AC_SUBST(GETTEXT_PACKAGE)
AC_SUBST(DATADIRNAME)

AC_CONFIG_FILES([
	po/Makefile
	liblicense.pc
	Makefile
	liblicense/Makefile
	licenses/Makefile
	modules/Makefile
	modules/io/Makefile
	modules/config/Makefile
	bindings/Makefile
	bindings/python/Makefile
	bindings/ruby/Makefile
	tests/Makefile
	xdgmime/Makefile
	utils/Makefile
	icons/Makefile
	dynamic_tests/Makefile
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
	enable sqlite3 caching: 		$enable_sqlite
	enable ruby bindings:			$enable_ruby
	enable python bindings: 		$enable_python

I/O Modules:
	exempi:					$have_exempi
	sidecar:				$have_exempi
	id3:					$have_id3lib
	vorbis:					$have_vorbis
	FLAC:					$have_flac
	raptor:					$have_libxml2
	taglib:					$have_taglib
	gsf:					$have_libgsf
"
