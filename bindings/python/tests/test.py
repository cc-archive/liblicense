#!/usr/bin/env python
import os, sys
print "Test system_license python bindings:"

# Add the build directory to the import path.
#base_path = "bindings/python/build"
#sys.path += (map(lambda path: os.getcwd()+"/"+base_path+"/"+path,filter(lambda path: path.startswith('tmp')==False, os.listdir(base_path))))

from liblicense import *
license="http://creativecommons.org/licenses/by-nd/2.0/"
print "\tget_jurisdiction:",get_jurisdiction(license)
print "\tget_name:", get_name(license)
v = get_version(license)
print "\tget_version: "+str(v[0])+"."+str(v[1])+"."+str(v[2])
print "\tget_permits:", get_permits(license)
print "\tget_prohibits:", get_prohibits(license)
print "\tget_requires:", get_requires(license)
print "\tverify_uri:", verify_uri(license)
print "\tget_attribute:", get_attribute(license,"http://purl.org/dc/elements/1.1/title",True)
print "\tget_licenses:", get_licenses()
print "\tget_licenses(juris):", get_licenses("za")

print "Test write_license python bindings:"
print "\twrite:",write("license_me.txt","creativecommons.org/licenses/by/2.5/au/")
print "\twrite(module):",write("license_me.txt","creativecommons.org/licenses/by/2.5/au/","txt.h")

#print "Test read_license python bindings:"
#print "\tread:",read("license_me.txt")
#print "\tmodule_read:",read("license_me.txt","exempi.o")

print "Test system_default:"
print "\tset_default:",set_default("creativecommons.org/licenses/by/2.5/au/")
print "\tget_default:",get_default()
print "\tdefault_engines:",default_engines()

print "Test module_wrangler:"
print "\tget_config_modules:",get_config_modules()
print "\tget_io_modules:",get_io_modules()
print "\tget_module_symbol: not bound"
print "\tmodule_in_use:",module_in_use("gconf.o")
print "\tmodule_mime_types:",module_mime_types("exempi.o")
sys.exit(0)
