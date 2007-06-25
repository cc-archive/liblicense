#!/usr/bin/env python
import os, sys
print "Test liblicense python bindings:"

# Add the build directory to the import path.
base_path = "bindings/python/build"
sys.path += (map(lambda path: os.getcwd()+"/"+base_path+"/"+path,filter(lambda path: path.startswith('tmp')==False, os.listdir(base_path))))

from liblicense import *
print "\tget_jurisdiction:",get_jurisdiction("creativecommons.org/licenses/by/2.5/au/")
print "\tget_jurisdictions(short):",get_jurisdictions("attribution")
print "\tget_jurisdictions(bitcode):",get_jurisdictions(1)
print "\tget_locale:",get_locale("creativecommons.org/licenses/by/2.5/au/")
print "\tget_short_locales:",get_locales("attribution","au")
print "\tget_bitcode_locales:",get_locales(1,"au")
print "\tget_name:", get_name("creativecommons.org/licenses/by/2.5/au/")
v = get_version("creativecommons.org/licenses/by/2.5/au/")
print "\tget_version: "+str(v[0])+"."+str(v[1])+"."+str(v[2])
print "\tget_versions(short):",get_versions("attribution","au")
print "\tget_versions(bitcode):",get_versions(1,"au")
print "\tget_short:", get_short("creativecommons.org/licenses/by/2.5/au/")
print "\thas_flag:", has_flag(1,"creativecommons.org/licenses/by/2.5/au/")
print "\tfamily_flags: skipped"
print "\tfamily:", family("creativecommons.org/licenses/by/2.5/au/")
print "\tget_notification:", get_notification("creativecommons.org/licenses/by/2.5/au/")
print "\tget_notification(verifiable):",get_notification("creativecommons.org/licenses/by/2.5/au/", "http://www.mysite.com/bleh")
print "\tverify_uri:", verify_uri("creativecommons.org/licenses/by/2.5/au/")
print "\tget_license:", get_license("Creative Commons",1, "au","au")
print "\tget_all_licenses: skipped"
print "\tget_general_licenses: skipped"
print "\tget_families:",get_families()
sys.exit(0)
