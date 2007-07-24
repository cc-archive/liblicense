import gtk
import gobject
import liblicense

class LicenseWidget(gtk.VBox):
    __gsignals__ = {"changed":(gobject.SIGNAL_RUN_FIRST,gobject.TYPE_NONE,(gobject.TYPE_STRING,))}
    def __init__(self,license):
        icon_size=48
        icon_padding = 4
        gtk.VBox.__init__(self)
        
        self.set_spacing(5)
        # Attribution
        box = gtk.HBox()
        box.show()
        
        self.by = gtk.CheckButton()
        self.by.show()
        box.pack_start(self.by,False,False,10)
        
        icon = gtk.Image()
        icon.set_from_pixbuf(gtk.gdk.pixbuf_new_from_file_at_size("by.svg", icon_size, icon_size))
        icon.show()
        box.pack_start(icon,False,False,icon_padding)

        label = gtk.Label("Attribution")
        label.show()
        box.pack_start(label,False,False,10)
        
        self.pack_start(box)
        
        # Allow Sharing
        box = gtk.HBox()
        box.show()
        
        self.ash = gtk.CheckButton()
        self.ash.show()
        box.pack_start(self.ash,False,False,10)
        
        icon = gtk.Image()
        icon.set_from_pixbuf(gtk.gdk.pixbuf_new_from_file_at_size("ash.svg", icon_size, icon_size))
        icon.show()
        box.pack_start(icon,False,False,icon_padding)
        
        label = gtk.Label("Allow Sharing")
        label.show()
        box.pack_start(label,False,False,10)
        self.ash_box = box
        self.pack_start(box)
        
        # Allow Remixing
        box = gtk.HBox()
        box.show()
        
        self.ar = gtk.CheckButton()
        self.ar.show()
        self.ar.connect("toggled",self.ar_toggled)
        box.pack_start(self.ar,False,False,10)
        
        icon = gtk.Image()
        icon.set_from_pixbuf(gtk.gdk.pixbuf_new_from_file_at_size("ar.svg", icon_size, icon_size))
        icon.show()
        box.pack_start(icon,False,False,icon_padding)
        
        label = gtk.Label("Allow Remixing")
        label.show()
        box.pack_start(label,False,False,10)
        self.ar_box = box
        self.pack_start(box)
        
        # Prohibit Commercial Works
        box = gtk.HBox()
        box.show()
        
        self.pcw = gtk.CheckButton()
        self.pcw.show()
        box.pack_start(self.pcw,False,False,10)
        
        icon = gtk.Image()
        icon.set_from_pixbuf(gtk.gdk.pixbuf_new_from_file_at_size("pcw.svg", icon_size, icon_size))
        icon.show()
        box.pack_start(icon,False,False,icon_padding)
        
        label = gtk.Label("Prohibit Commercial Works")
        label.show()
        box.pack_start(label,False,False,10)
        self.pcw_box = box
        self.pack_start(box)
        
        # Require Others to Share-Alike
        box = gtk.HBox()
        box.show()
        
        self.sa = gtk.CheckButton()
        self.sa.show()
        box.pack_start(self.sa,False,False,10)
        
        icon = gtk.Image()
        icon.set_from_pixbuf(gtk.gdk.pixbuf_new_from_file_at_size("sa.svg", icon_size, icon_size))
        icon.show()
        box.pack_start(icon,False,False,icon_padding)
        
        label = gtk.Label("Require Others to Share-Alike")
        label.show()
        box.pack_start(label,False,False,10)
        self.sa_box = box
        
        self.pack_start(box)
        
        # License and URI
        hbox = gtk.HBox()
        hbox.show()
        
        box = gtk.VBox()
        box.show()
        hbox.pack_start(box,False,False,5)
        
        label = gtk.Label("License:")
        label.show()
        box.pack_start(label)
        
        label = gtk.Label("Jurisdiction:")
        label.show()
        box.pack_start(label)
        
        label = gtk.Label("URI:")
        label.show()
        box.pack_start(label)
        
        box = gtk.VBox()
        box.show()
        
        #license
        self.license = gtk.Entry()
        self.license.set_editable(False)
        self.license.set_has_frame(False)
        self.license.show()
        box.pack_start(self.license)
        
        #jurisdiction
        self.jurisdictions = gtk.ListStore(str,str,gtk.gdk.Pixbuf)
        self.jurisdiction = gtk.ComboBox(self.jurisdictions)
        cell = gtk.CellRendererPixbuf()
        self.jurisdiction.pack_start(cell, False)
        self.jurisdiction.add_attribute(cell, 'pixbuf', 2)
        cell = gtk.CellRendererText()
        self.jurisdiction.pack_start(cell, True)
        self.jurisdiction.add_attribute(cell, 'text', 1)
        self.jurisdictions.append(["","Unported",gtk.gdk.pixbuf_new_from_file_at_size("icons/unported.png",27,16)])
        self._j = []
        self._j.append(None)
        self.jurisdictions.append(["uk","United Kingdom",gtk.gdk.pixbuf_new_from_file_at_size("icons/uk.png",27,16)])
        self._j.append("uk")
        self.jurisdictions.append(["us","United States",gtk.gdk.pixbuf_new_from_file_at_size("icons/us.png",27,16)])
        self._j.append("us")
        
        self.jurisdiction.show()
        
        box.pack_start(self.jurisdiction,False,False,0)
        
        self.uri = gtk.Entry()
        self.uri.set_editable(True)
        self.uri.show()
        box.pack_start(self.uri)
        self.label = label
        self.icon = icon
        hbox.pack_start(box)
        self.pack_start(hbox)
        
        self.set_license(license)
        
        # hook up license switcher
        self.ash.connect("toggled",self.checkbox_toggled,0)
        self.ar.connect("toggled",self.checkbox_toggled,2)
        self.pcw.connect("toggled",self.checkbox_toggled,1)
        self.sa.connect("toggled",self.checkbox_toggled,3)
        self.by.connect("toggled",self.checkbox_toggled,4)
        
        self.jurisdiction.connect("changed",self.update_jurisdiction)
    
    def get_licenses(self):
        if self.jurisdiction.get_active()<=0:
            l = liblicense.get_licenses()
        else:
            l = liblicense.get_licenses(self.jurisdictions[self.jurisdiction.get_active()][0])
        self.licenses = {}
        for license in l:
            if not self.licenses.has_key(self.license_flags(license)):
                self.licenses[self.license_flags(license)] = []
            self.licenses[self.license_flags(license)].append(license)
    
    def update_checkboxes(self,license):
        if license:
            self.current_flags= list(self.license_flags(license))
        else:
            self.current_flags=[False,False,False,False,False]
        self.ash.set_active(self.current_flags[0])
        self.pcw.set_active(self.current_flags[1])
        self.ar.set_active(self.current_flags[2])
        self.ar.toggled()
        self.sa.set_active(self.current_flags[3])
        self.by.set_active(self.current_flags[4])

    def ar_toggled(self,button):
        self.sa_box.set_sensitive(button.get_active())
        if not button.get_active():
            self.sa.set_active(False)
            self.sa.toggled()
    
    def checkbox_toggled(self,button,flag):
        self.current_flags[flag] = button.get_active()
        self.update_license()
    
    def update_license(self):
        if self.licenses.has_key(tuple(self.current_flags)):
            u = self.licenses[tuple(self.current_flags)][0]
            self.emit("changed",u)
            self.uri.set_text(u)
            v = liblicense.get_version(u)
            self.license.set_text(liblicense.get_name(u) + " " + str(v[0]) + "." + str(v[1]) + "." + str(v[2]))
        else:
            self.uri.set_text("")
            self.license.set_text("none")
    
    def license_flags(self,license):
        permits = liblicense.get_permits(license)
        requires = liblicense.get_requires(license)
        return ("http://creativecommons.org/ns#Distribution" in permits,
                "http://creativecommons.org/ns#CommercialUse" in liblicense.get_prohibits(license),
                "http://creativecommons.org/ns#DerivativeWorks" in permits,
                "http://creativecommons.org/ns#ShareAlike" in requires,
                "http://creativecommons.org/ns#Attribution" in requires)
    
    def update_jurisdiction(self, widget):
        self.get_licenses()
        self.update_license()
    
    def set_license(self, license):
        if license:
            version = liblicense.get_version(license)
            self.license.set_text(liblicense.get_name(license) + " " + str(version[0]) + "." + str(version[1]) + "." + str(version[2]))
            self.uri.set_text(license)
            if self.jurisdiction.get_active()==-1 or self._j[self.jurisdiction.get_active()]!=liblicense.get_jurisdiction(license):
                self.get_licenses()
                
            self.jurisdiction.set_active(self._j.index(liblicense.get_jurisdiction(license)))
        else:
            self.license.set_text("none")
            if self.jurisdiction.get_active()!=0:
                self.get_licenses()
            self.jurisdiction.set_active(0)
        self.update_checkboxes(license)
    
    def get_license(self):
        return self.uri.get_text()

if __name__=="__main__":
    import sys
    if len(sys.argv)<2:
        print "Please pass a test filename in."
        sys.exit(1)
    def close(window):
        sys.exit(0)
    def changed(widget,uri):
        print "changed: " + uri
    window = gtk.Window()
    window.connect("destroy",close)
    window.set_title("liblicense widget test")
    widget = LicenseWidget(liblicense.read(sys.argv[1]))
    widget.connect("changed",changed)
    widget.show()
    window.add(widget)
    window.show()
    gtk.main()
