#!/usr/bin/env python
import os, sys
print "Test system_license python bindings:"

# Add the build directory to the import path.
#base_path = "bindings/python/build"
#sys.path += (map(lambda path: os.getcwd()+"/"+base_path+"/"+path,filter(lambda path: path.startswith('tmp')==False, os.listdir(base_path))))

from liblicense import *
print "\tget_jurisdiction:",get_jurisdiction("creativecommons.org/licenses/by/2.5/au/")
print "\tget_locales:",get_locales("creativecommons.org/licenses/by/2.5/au/")
print "\tget_name:", get_name("creativecommons.org/licenses/by/2.5/au/")
v = get_version("creativecommons.org/licenses/by/2.5/au/")
print "\tget_version: "+str(v[0])+"."+str(v[1])+"."+str(v[2])
print "\tget_notification:", get_notification("creativecommons.org/licenses/by/2.5/au/")
print "\tget_notification(verifiable):",get_notification("creativecommons.org/licenses/by/2.5/au/", "http://www.mysite.com/bleh")
print "\tverify_uri:", verify_uri("creativecommons.org/licenses/by/2.5/au/")
print "\tget_attribute:", get_attribute("creativecommons.org/licenses/by/2.5/au/","permits")
print "\tget_licenses:", get_licenses()
print "\tget_licenses(juris):", get_licenses("ar")

print "Test write_license python bindings:"
print "\twrite:",write("license_me.txt","creativecommons.org/licenses/by/2.5/au/")
print "\twrite(module):",write("license_me.txt","creativecommons.org/licenses/by/2.5/au/","txt.h")

print "Test read_license python bindings:"
print "\tread:",read("license_me.txt")
print "\tmodule_read:",read("license_me.txt","txt.h")

print "Test system_default:"
print "\tset_default:",set_default("creativecommons.org/licenses/by/2.5/au/")
print "\tget_default:",get_default()
print "\tdefault_engines:",default_engines()

print "Test module_wrangler:"
print "\tget_config_modules:",get_config_modules()
print "\tget_io_modules:",get_io_modules()
print "\tget_module_symbol: not bound"
print "\tmodule_in_use:",module_in_use("gconf.o")
print "\tmodule_mime_types:",module_mime_types("nautilus.o")
sys.exit(0)
