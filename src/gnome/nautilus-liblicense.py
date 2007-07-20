import liblicense
import urllib

import gtk
import nautilus
from widgets import *

class LicensePropertyPage(nautilus.PropertyPageProvider):
    def __init__(self):
        pass

    def license_chosen(self, widget):
        liblicense.write(self.filename,self.license)
    
    def new_license(self,widget,license):
        self.license = license
        print license
        
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
        
        self.license = liblicense.read(self.filename)
        self.box = LicenseWidget(self.license)
        self.box.connect("destroy",self.license_chosen)
        self.box.connect("changed",self.new_license)
        self.box.show()
        
        return nautilus.PropertyPage("NautilusPython::license",
                                     self.property_label, self.box),
