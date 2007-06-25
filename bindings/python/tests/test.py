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
print "\tget_licenses(juris):", get_licenses("ar");
sys.exit(0)
