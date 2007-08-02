#!/usr/bin/env python
# Creative Commons has made the contents of this file
# available under a CC-GNU-LGPL license:
#
# http://creativecommons.org/licenses/LGPL/2.1/
#
# A copy of the full license can be found as part of this
# distribution in the file COPYING.
# 
# You may use the liblicense software in accordance with the
# terms of that license. You agree that you are solely 
# responsible for your use of the liblicense software and you
# represent and warrant to Creative Commons that your use
# of the liblicense software will comply with the CC-GNU-LGPL.
#
# Copyright 2007, Creative Commons, www.creativecommons.org.
# Copyright 2007, Scott Shawcroft.

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
print "\tget_version: "+".".join(map(str,v))
print "\tget_permits:", get_permits(license)
print "\tget_prohibits:", get_prohibits(license)
print "\tget_requires:", get_requires(license)
print "\tverify_uri:", verify_uri(license)
print "\tget_attribute:", get_attribute(license,"http://purl.org/dc/elements/1.1/title",True)
print "\tget_licenses:", get_licenses()
print "\tget_licenses(juris):", get_licenses("za")

print "Test write_license python bindings:"
print "\twrite:",write("license_me.txt","creativecommons.org/licenses/by/2.5/au/")
print "\twrite(module):",write("license_me.txt","creativecommons.org/licenses/by/2.5/au/","stub.so")

print "Test read_license python bindings:"
print "\tread:",read("license_me.txt")
#print "\tmodule_read:",read("license_me.txt","exempi.o")

print "Test system_default:"
print "\tset_default:",set_default("creativecommons.org/licenses/by/2.5/au/")
print "\tget_default:",get_default()

print "Test module_wrangler:"
print "\tget_config_modules:",get_config_modules()
print "\tget_io_modules:",get_io_modules()
print "\tget_module_symbol: not bound"

print "Module information and capabilities:"
print_module_info();

print "What license(s) permit Distribution and DerivativeWorks without Attribution or Notice?"
attributes = ["http://creativecommons.org/ns#Distribution",
		"http://creativecommons.org/ns#DerivativeWorks",
		"http://creativecommons.org/ns#Attribution",
		"http://creativecommons.org/ns#Notice"]
chooser = LicenseChooser(None,attributes)

permits = chooser.attribute_flag("http://creativecommons.org/ns#Distribution") |  chooser.attribute_flag("http://creativecommons.org/ns#DerivativeWorks")
print "\t",chooser.get_licenses(permits=permits,requires=liblicense.UNSPECIFIED,prohibits=liblicense.UNSPECIFIED)

sys.exit(0)
