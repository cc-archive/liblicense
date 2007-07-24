#! /usr/bin/env python
import liblicense
import widgets
import gtk

class DefaultLicense (gtk.Window):
    def __init__(self):
        gtk.Window.__init__(self)
        self.set_title("Default Content License")
        
        self.connect("destroy",self.destroy)
        
        box = gtk.VBox()
        box.show()
        
        self.add(box)
        
        #title
        title_box = gtk.HBox()
        icon = gtk.Image()
        icon.set_from_file("scales.svg")
        icon.show()
        title_box.pack_start(icon,False,False,5)
        
        label = gtk.Label("Default Content License")
        label.show()
        title_box.pack_start(label,False)
        title_box.show()
        box.pack_start(title_box)
        
        #widget
        frame = gtk.Frame()
        frame.show()
        
        self.default = liblicense.get_default()
        self.chooser = widgets.LicenseWidget(self.default)
        self.chooser.show()
        frame.add(self.chooser)
        box.pack_start(frame)
        
        #buttons
        bbox = gtk.HButtonBox()
        bbox.show()
        box.pack_start(bbox)
        bbox.set_layout(gtk.BUTTONBOX_END)
        
        app = gtk.Button(stock=gtk.STOCK_APPLY)
        app.show()
        app.connect("clicked",self.save)
        bbox.pack_end(app,False,False)
        
        reset = gtk.Button("Reset",gtk.STOCK_UNDO)
        reset.show()
        reset.connect("clicked",self.reset)
        bbox.pack_end(reset,False,False)
        
        can = gtk.Button(stock=gtk.STOCK_CANCEL)
        can.show()
        can.connect("clicked",self.destroy)
        bbox.pack_end(can,False,False)
    
    def destroy(self,widget):
        gtk.main_quit()
    
    def run(self):
        gtk.main()
    
    def save(self,widget):
        liblicense.set_default(self.chooser.get_license())
        self.destroy(widget)
    
    def reset(self,widget):
        self.chooser.set_license(self.default)
        

if __name__=="__main__":
    window = DefaultLicense()
    window.show()
    window.run()
