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

dist_kde4_services_DATA = licenseprops.desktop

libdir = @kde4_plugin_dir@
lib_LTLIBRARIES = licenseprops.la
licenseprops_la_CPPFLAGS = $(QTCORE_CFLAGS) $(QTGUI_CFLAGS) $(KDE4_INCLUDES) -I$(top_builddir)/../../liblicense
licenseprops_la_LDFLAGS = -module -avoid-version \
             ../../../liblicense/liblicense.la
licenseprops_la_LIBADD = $(QTCORE_LIBS) $(QTGUI_LIBS) $(KDE4_LIBS)

licenseprops_la_SOURCES = moc_licenseprops.cpp licenseprops.cpp \
   moc_license_chooser.cpp license_chooser.cpp

# This rule lets GNU make create any moc_*.cpp from the equivalent *.h
moc_%.cpp: %.h
	moc $< -o $@

CLEANFILES = moc_licenseprops.cpp moc_license_chooser.cpp
