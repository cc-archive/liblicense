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

dist_kde4_services_DATA = liblicense.desktop

libdir = @kde4_plugin_dir@
lib_LTLIBRARIES = kcm_liblicense.la
kcm_liblicense_la_CPPFLAGS = $(QTCORE_CFLAGS) $(QTGUI_CFLAGS) $(KDE4_INCLUDES)  -I$(top_builddir)/../../liblicense
kcm_liblicense_la_LDFLAGS = -module -avoid-version \
             ../../../liblicense/liblicense.la
kcm_liblicense_la_LIBADD = $(QTCORE_LIBS) $(QTGUI_LIBS) $(KDE4_LIBS)

kcm_liblicense_la_SOURCES = moc_kcm_liblicense.cpp kcm_liblicense.cpp \
  moc_license_chooser.cpp license_chooser.cpp

# This rule lets GNU make create any moc_*.cpp from the equivalent *.h
moc_%.cpp: %.h
	moc $< -o $@

CLEANFILES = moc_kcm_liblicense.cpp moc_license_chooser.cpp
