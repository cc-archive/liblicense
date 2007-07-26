AC_PREREQ(2.57)
AC_INIT([liblicense-kde],[0.2],[cc-devel@lists.ibiblio.org])
AM_INIT_AUTOMAKE([dist-bzip2])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL

AC_PROG_CXX

QTCORE_REQUIRED=4.0
QTGUI_REQUIRED=4.0

PKG_CHECK_MODULES(QTCORE,[
	QtCore >= $QTCORE_REQUIRED],
	[have_qtcore=yes] , [have_qtcore=no])
AC_SUBST(QTCORE_CFLAGS)
AC_SUBST(QTCORE_LIBS)

PKG_CHECK_MODULES(QTGUI,[
	QtGui >= $QTGUI_REQUIRED],
	[have_qtcore=yes] , [have_qtgui=no])
AC_SUBST(QTGUI_CFLAGS)
AC_SUBST(QTGUI_LIBS)

KDE_FIND_PATH(kde4-config, KDE4CONFIG, [], [echo "Error: kde4-config not found." && exit 1], [], prepend)

kde4_plugin_dir=`kde4-config --install module`
AC_SUBST(kde4_plugin_dir)

kde4_servicesdir=`kde4-config --install services`
AC_SUBST(kde4_servicesdir)

KDE4_INCLUDES=-I`$KDE4CONFIG --prefix`/include
KDE4_LIBS=-L`$KDE4CONFIG --prefix`/lib

AC_SUBST(KDE4_INCLUDES)
AC_SUBST(KDE4_LIBS)

AC_CONFIG_FILES([
	Makefile
	kde-licenseprops/Makefile
	kcm-license/Makefile
])

AC_CONFIG_HEADERS([config.h])

AC_OUTPUT
