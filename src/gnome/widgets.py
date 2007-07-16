import gtk
import liblicense

class LicenseWidget(gtk.VBox):
    def __init__(self,filename):
        gtk.VBox.__init__(self)
        
        # Attribution
        box = gtk.HBox()
        box.show()
        
        self.by = gtk.CheckButton()
        self.by.show()
        box.pack_start(self.by,False,False,10)
        
        icon = gtk.Image()
        icon.set_from_file("by.svg")
        icon.show()
        box.pack_start(icon,False,False,10)

        label = gtk.Label("Attribution")
        label.show()
        box.pack_start(label)
        
        self.pack_start(box)
        
        # Allow Sharing
        box = gtk.HBox()
        box.show()
        
        self.ash = gtk.CheckButton()
        self.ash.show()
        box.pack_start(self.ash,False,False,10)
        
        icon = gtk.Image()
        icon.set_from_file("ash.svg")
        icon.show()
        box.pack_start(icon,False,False,10)
        
        label = gtk.Label("Allow Sharing")
        label.show()
        box.pack_start(label)
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
        icon.set_from_file("ar.svg")
        icon.show()
        box.pack_start(icon,False,False,10)
        
        label = gtk.Label("Allow Remixing")
        label.show()
        box.pack_start(label)
        self.ar_box = box
        self.pack_start(box)
        
        # Prohibit Commercial Works
        box = gtk.HBox()
        box.show()
        
        self.pcw = gtk.CheckButton()
        self.pcw.show()
        box.pack_start(self.pcw,False,False,10)
        
        icon = gtk.Image()
        icon.set_from_file("pcw.svg")
        icon.show()
        box.pack_start(icon,False,False,10)
        
        label = gtk.Label("Prohibit Commercial Works")
        label.show()
        box.pack_start(label)
        self.pcw_box = box
        self.pack_start(box)
        
        # Require Others to Share-Alike
        box = gtk.HBox()
        box.show()
        
        self.sa = gtk.CheckButton()
        self.sa.show()
        box.pack_start(self.sa,False,False,10)
        
        icon = gtk.Image()
        icon.set_from_file("sa.svg")
        icon.show()
        box.pack_start(icon,False,False,10)
        
        label = gtk.Label("Require Others to Share-Alike")
        label.show()
        box.pack_start(label)
        self.sa_box = box
        
        self.pack_start(box)
        
        # License
        box = gtk.HBox()
        box.show()
        
        label = gtk.Label("License:")
        label.show()
        box.pack_start(label)
        
        current_license = liblicense.read(filename)
        self.license = gtk.Entry()
        if current_license:
            version = liblicense.get_version(current_license)
            self.license.set_text(liblicense.get_name(current_license) + " " + str(version[0]) + "." + str(version[1]) + "." + str(version[2]))
        else:
            self.license.set_text("none")
        self.license.set_editable(False)
        self.license.set_has_frame(False)
        self.license.show()
        box.pack_start(self.license)
        
        self.pack_start(box)
        
        # URI
        box = gtk.HBox()
        box.show()
        
        label = gtk.Label("URI:")
        label.show()
        box.pack_start(label)
        
        self.update_checkboxes(current_license)
        self.uri = gtk.Entry()
        if current_license:
            self.uri.set_text(current_license)
        self.uri.set_editable(True)
        self.uri.show()
        box.pack_start(self.uri)
        self.label = label
        self.icon = icon
        self.pack_start(box)
        
        # get list of licenses
        l = liblicense.get_licenses()
        self.licenses = {}
        for license in l:
            if not self.licenses.has_key(self.license_flags(license)):
                self.licenses[self.license_flags(license)] = []
            self.licenses[self.license_flags(license)].append(license)
        
        # hook up license switcher
        self.ash.connect("toggled",self.update_license,0)
        self.ar.connect("toggled",self.update_license,2)
        self.pcw.connect("toggled",self.update_license,1)
        self.sa.connect("toggled",self.update_license,3)
        self.by.connect("toggled",self.update_license,4)
    
    def update_checkboxes(self,license):
        if license=="http://creativecommons.org/licenses/publicdomain/":
            self.pd.set_active(True)
            self.pd.toggled()
        else:
            if license:
                self.flags= list(self.license_flags(license))
            else:
                self.flags=[False,False,False,False,False]
            self.ash.set_active(self.flags[0])
            self.pcw.set_active(self.flags[1])
            self.ar.set_active(self.flags[2])
            self.ar.toggled()
            self.sa.set_active(self.flags[3])
            self.by.set_active(self.flags[4])

    def ar_toggled(self,button):
        self.sa_box.set_sensitive(button.get_active())
        if not button.get_active():
            self.sa.set_active(False)
            self.sa.toggled()
    
    def update_license(self,button,flag):
        self.flags[flag] = button.get_active()
        if self.licenses.has_key(tuple(self.flags)):
            print self.licenses[tuple(self.flags)]
            u = self.licenses[tuple(self.flags)][0]
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

if __name__=="__main__":
    import sys
    if len(sys.argv)<2:
        print "Please pass a test filename in."
        sys.exit(1)
    def close(window):
        sys.exit(0)
    window = gtk.Window()
    window.connect("destroy",close)
    window.set_title("liblicense widget test")
    widget = LicenseWidget(sys.argv[1])
    widget.show()
    window.add(widget)
    window.show()
    gtk.main()
