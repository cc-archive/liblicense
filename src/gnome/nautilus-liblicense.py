import liblicense
import urllib

import gtk
import nautilus
from liblicense.gui_gtk import *

class LicensePropertyPage(nautilus.PropertyPageProvider):
    def __init__(self):
        pass

    def license_chosen(self, widget):
        liblicense.write(self.filename,self.box.get_license())
        
    def get_property_pages(self, files):
        if len(files) != 1:
            return
        
        file = files[0]
        if file.get_uri_scheme() != 'file':
            return

        if file.is_directory():
            return

        self.filename = urllib.unquote(file.get_uri()[7:])

        self.property_label = gtk.Label('License')
        self.property_label.show()
        
        license = liblicense.read(self.filename)
        self.box = LicenseWidget(license)
        self.box.connect("destroy",self.license_chosen)
        self.box.show()
        
        return nautilus.PropertyPage("NautilusPython::license",
                                     self.property_label, self.box),

class LicenseInfoProvider(nautilus.InfoProvider):
    def __init__(self):
        pass
    
    def update_file_info(self, f):
        if f.get_uri()[:7]=="file://":
            license = liblicense.read(f.get_uri()[7:].replace("%20"," "))
            if license:
                if "Creative Commons" in liblicense.get_attribute(license,"http://purl.org/dc/elements/1.1/creator",False):
                    f.add_emblem("cc")
                else:
                    f.add_emblem("licensed")
