import liblicense
import urllib

import gtk
import nautilus
from liblicense.gui_gtk import *

class LicensePropertyPage(nautilus.PropertyPageProvider):
    def __init__(self):
        pass

    def license_chosen(self, widget):
        license = self.box.get_license()
        if license:
            for f in self.files:
                liblicense.write(f,license)
        
    def get_property_pages(self, files):
        self.files = files

        self.files = filter(lambda f: f.get_uri_scheme() == 'file' and not f.is_directory(),self.files)
        self.files = map(lambda f: urllib.unquote(f.get_uri()[7:]),self.files)
        
        if len(self.files)==0:
            return

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
