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

import liblicense
import urllib

import gtk
import nautilus
from liblicense.gui_gtk import *

class LicenseInfoProvider(nautilus.InfoProvider):
    def __init__(self):
        pass
    
    def update_file_info(self, f):
        if f.get_uri()[:7]=="file://":
            license = liblicense.read(urllib.unquote(f.get_uri()[7:]))
            if license:
                if "Creative Commons" in liblicense.get_attribute(license,"http://purl.org/dc/elements/1.1/creator",False):
                    f.add_emblem("cc")
                else:
                    f.add_emblem("licensed")

class LicensePropertyPage(nautilus.PropertyPageProvider):
    def __init__(self):
        pass

    def license_chosen(self, widget):
        license = self.box.get_license()
        if license and self.seen:
            for f in self.files:
                liblicense.write(f,license)
    
    def i_see_you(self,widget,event):
        self.seen = self.seen or widget.props.visible

    def get_property_pages(self, files):
        self.files = files

        self.files = filter(lambda f: f.get_uri_scheme() == 'file' and not f.is_directory(),self.files)
        self.files = map(lambda f: urllib.unquote(f.get_uri()[7:]),self.files)
        
        if len(self.files)==0:
            return
        
        self.seen = False
        self.property_label = gtk.Label('License')
        self.property_label.show()
        
        if len(files) == 1:
            license = liblicense.read(self.files[0])
            if license==None:
                license = liblicense.get_default()
        else:
            license = None
        self.box = LicenseWidget(license)
        self.box.connect("destroy",self.license_chosen)
        
        #when the by checkbox is exposed we know the whole layout is.
        self.box.by.connect("expose-event",self.i_see_you)

        self.box.show()
        
        return nautilus.PropertyPage("NautilusPython::license",
                                     self.property_label, self.box),
