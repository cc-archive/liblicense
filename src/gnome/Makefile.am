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
# Copyright 2007, Scott Shawcroft.
if HAVE_PYGTK
pkgpython_PYTHON = gui_gtk.py
bin_SCRIPTS = default-content-license

iconsdir = $(pkgdatadir)/icons

icons_DATA = ar.svg \
             ash.svg \
             by.svg \
             pcw.svg \
             sa.svg \
             scales.svg \
             icons/unported.png \
             icons/us.png \
             icons/uk.png
Applicationsdir = $(datadir)/applications
Applications_DATA = default-content-license.desktop

EXTRA_DIST = $(icons_DATA) $(Applications_DATA)

logodir = $(datadir)/pixmaps
logo_DATA = scales.svg
endif

if HAVE_NP
pynautilusdir = $(libdir)/nautilus/extensions-1.0/python/
pynautilus_SCRIPTS = nautilus-liblicense.py
endif
