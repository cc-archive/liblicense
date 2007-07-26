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

SUBDIRS = . kde-licenseprops kcm-license

noinst_LTLIBRARIES = libkdelicensewidget.la
libkdelicensewidget_la_CPPFLAGS = $(QTCORE_CFLAGS) $(QTGUI_CFLAGS) $(KDE4_INCLUDES)
libkdelicensewidget_la_LDFLAGS = ../../liblicense/liblicense.la
libkdelicensewidget_la_LIBADD = $(QTCORE_LIBS) $(QTGUI_LIBS) $(KDE4_LIBS)

# I can only get this to be generated if I give it the extension cpp (instead of h that I want)
libkdelicensewidget_la_SOURCES = ui_license_chooser.cpp license_chooser.ui

ui_%.cpp: %.ui
	uic $< -o $@

CLEANFILES = ui_license_chooser.cpp
