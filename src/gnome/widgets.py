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
        box.pack_start(self.pd)
        
        icon = gtk.Image()
        icon.set_from_file("/home/scott/.nautilus/python-extensions/pd.svg")
        icon.show()
        box.pack_start(icon)

        label = gtk.Label("Public Domain")
        label.show()
        box.pack_start(label)
        
        self.pack_start(box)
        
        # Allow Sharing
        box = gtk.HBox()
        box.show()
        
        self.ash = gtk.CheckButton()
        self.ash.show()
        box.pack_start(self.ash)
        
        icon = gtk.Image()
        icon.set_from_file("/home/scott/.nautilus/python-extensions/ash.svg")
        icon.show()
        box.pack_start(icon)
        
        label = gtk.Label("Allow Sharing")
        label.show()
        box.pack_start(label)
        
        self.pack_start(box)
        
        # Allow Remixing
        box = gtk.HBox()
        box.show()
        
        self.ar = gtk.CheckButton()
        self.ar.show()
        box.pack_start(self.ar)
        
        icon = gtk.Image()
        icon.set_from_file("/home/scott/.nautilus/python-extensions/ar.svg")
        icon.show()
        box.pack_start(icon)
        
        label = gtk.Label("Allow Remixing")
        label.show()
        box.pack_start(label)
        
        self.pack_start(box)
        
        # Prohibit Commercial Works
        box = gtk.HBox()
        box.show()
        
        self.pcw = gtk.CheckButton()
        self.pcw.show()
        box.pack_start(self.pcw)
        
        icon = gtk.Image()
        icon.set_from_file("/home/scott/.nautilus/python-extensions/pcw.svg")
        icon.show()
        box.pack_start(icon)
        
        label = gtk.Label("Prohibit Commercial Works")
        label.show()
        box.pack_start(label)
        
        self.pack_start(box)
        
        # Require Others to Share-Alike
        box = gtk.HBox()
        box.show()
        
        self.sa = gtk.CheckButton()
        self.sa.show()
        box.pack_start(self.sa)
        
        icon = gtk.Image()
        icon.set_from_file("/home/scott/.nautilus/python-extensions/sa.svg")
        icon.show()
        box.pack_start(icon)
        
        label = gtk.Label("Require Others to Share-Alike")
        label.show()
        box.pack_start(label)
        
        self.pack_start(box)
        
        # License
        box = gtk.HBox()
        box.show()
        
        label = gtk.Label("License:")
        label.show()
        box.pack_start(label)
        
        current_license = liblicense.read(filename)
        entry = gtk.Entry()
        entry.set_text(liblicense.get_name(current_license))
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
        entry = gtk.Entry()
        entry.set_text(current_license)
        entry.set_editable(True)
        entry.show()
        box.pack_start(entry)
        
        self.pack_start(box)
