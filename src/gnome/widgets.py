import gtk
import liblicense

class LicenseWidget(gtk.VBox):
    def __init__(self,filename):
        gtk.VBox.__init__(self)
        
        # Public Domain
        box = gtk.HBox()
        box.show()
        
        self.pd = gtk.CheckButton()
        self.pd.show()
        self.pd.connect("toggled",self.pd_toggled)
        box.pack_start(self.pd,False,False,10)
        
        icon = gtk.Image()
        icon.set_from_file("pd.svg")
        icon.show()
        box.pack_start(icon,False,False,10)

        label = gtk.Label("Public Domain")
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
        entry = gtk.Entry()
        version = liblicense.get_version(current_license)
        entry.set_text(liblicense.get_name(current_license) + " " + str(version[0]) + "." + str(version[1]) + "." + str(version[2]))
        entry.set_editable(False)
        entry.set_has_frame(False)
        entry.show()
        box.pack_start(entry)
        
        self.pack_start(box)
        
        # URI
        box = gtk.HBox()
        box.show()
        
        label = gtk.Label("URI:")
        label.show()
        box.pack_start(label)
        
        current_license = liblicense.read(filename)
        self.update_checkboxes(current_license)
        entry = gtk.Entry()
        entry.set_text(current_license)
        entry.set_editable(True)
        entry.show()
        box.pack_start(entry)
        self.label = label
        self.icon = icon
        self.pack_start(box)
    
    def update_checkboxes(self,license):
        if license=="http://creativecommons.org/licenses/publicdomain/":
            self.pd.set_active(True)
        else:
            permits = liblicense.get_permits(license)
            self.ash.set_active("http://creativecommons.org/ns#Distribution" in permits)
            self.pcw.set_active("http://creativecommons.org/ns#CommercialUse" in liblicense.get_prohibits(license))
            self.ar.set_active("http://creativecommons.org/ns#DerivativeWorks" in permits)
            self.sa.set_active("http://creativecommons.org/ns#ShareAlike" in liblicense.get_requires(license))
    
    def pd_toggled(self,button):
        self.ash_box.set_sensitive(not button.get_active())
        self.ar_box.set_sensitive(not button.get_active())
        self.pcw_box.set_sensitive(not button.get_active())
        self.sa_box.set_sensitive(not button.get_active())

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
