import liblicense
import urllib

import gtk
import nautilus
from widgets import *

class LicensePropertyPage(nautilus.PropertyPageProvider):
    def __init__(self):
        pass
    def license_chosen(self, widget):
        uri = self.licenses[widget.get_active()][0]
        liblicense.write(self.filename,uri)
        
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
        
        self.box = LicenseWidget(self.filename)
        self.box.show()

        """self.hbox = gtk.HBox(0, False)
        self.hbox.show()

        label = gtk.Label('License:')
        label.show()
        self.hbox.pack_start(label)
        
        current_license = liblicense.read(self.filename)
        self.licenses = gtk.ListStore(str,str)
        current=None
        for l in liblicense.get_licenses():
            i = self.licenses.append((l,liblicense.get_name(l)))
            if l==current_license:
                current=i
        self.value_box = gtk.ComboBox(self.licenses)
        cell = gtk.CellRendererText()
        self.value_box.pack_start(cell, True)
        self.value_box.add_attribute(cell, 'text', 1)
        if current:
            self.value_box.set_active_iter(current)
        self.value_box.connect("changed",self.license_chosen)
        self.hbox.pack_start(self.value_box)

        self.value_box.show() """
        
        return nautilus.PropertyPage("NautilusPython::license",
                                     self.property_label, self.box),
