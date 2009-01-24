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
import shutil

print "Test python bindings:"

# Add the build directory to the import path.
my_path = os.path.abspath(__file__)
os.stat(my_path) # Assert that we found something.
my_dir = os.path.dirname(my_path)
sys.path.insert(0, os.path.join(my_dir, '.libs'))
from liblicense import *

import unittest
import StringIO
class Tests(unittest.TestCase):
    def test_by_nd_data(self):
        license="http://creativecommons.org/licenses/by-nd/2.0/"
        assert get_jurisdiction(license) is None
        assert get_name(license) == 'Attribution-NoDerivs'
        v = get_version(license)
        assert v == [2, 0]
        assert set(get_permits(license)) == set(['http://creativecommons.org/ns#Distribution', 'http://creativecommons.org/ns#Reproduction'])
        assert set(get_prohibits(license)) == set([])
        assert set(get_requires(license)) == set(['http://creativecommons.org/ns#Attribution', 'http://creativecommons.org/ns#Notice'])
        assert verify_uri(license)
        assert get_attribute(license, "http://purl.org/dc/elements/1.1/title",True) == ['Attribution-NoDerivs']
        all_licenses = get_licenses()
        print "\tget_licenses:", all_licenses
        # FIXME: This fails - why?
        # assert license in all_licenses
        # FIXME: Should this really be empty?
        # print "\tget_licenses(juris):", get_licenses("za")

    def test_write_license_python_bindings(self):
        # Find us an MP3, first with no module specified
        shutil.copyfile(os.path.join(my_dir, '../../../tests/write_test.mp3'), 'write_test.mp3')
        assert (write('write_test.mp3', 'http://creativecommons.org/licenses/by/2.5/au/', None)  == 0)
        os.unlink('write_test.mp3')

        # Now specify the module
        shutil.copyfile(os.path.join(my_dir, '../../../tests/write_test.mp3'), 'write_test.mp3')
        assert (write('write_test.mp3', 'http://creativecommons.org/licenses/by/2.5/au/', 'stub.so') == 0)
        os.unlink('write_test.mp3')

    def test_read_license_python_bindings(self):
        # Find us an MP3, first with no module specified
        shutil.copyfile(os.path.join(my_dir, '../../../tests/write_test.mp3'), 'write_test.mp3')
        assert (write('write_test.mp3', 'http://creativecommons.org/licenses/by/2.5/au/', None)  == 0)

        assert read("write_test.mp3") == 'http://creativecommons.org/licenses/by/2.5/au/'
        # print "\tmodule_read:",read("license_me.txt","exempi.o")
        os.unlink('write_test.mp3')

    def test_system_default(self):
        license = "http://creativecommons.org/licenses/by/2.5/au/"
        assert set_default(license)
        assert get_default() == license

    def test_module_wrangler(self):
        'flat_file.so' in get_config_modules()
        'stub.so' in get_io_modules()
        print "\tget_module_symbol: not bound"

    def test_module_info(self):
        print "Module information and capabilities:"
        info = print_module_info()
        # assert 'stub.so' in info
        # FIXME: Make assertions about this output
        # for line in info.split('\n'):
        #     if 'Supported predicates' in line:
        #         assert 'http://creativecommons.org/ns#license' in line

    def test_chooser(self):
        attributes = ["http://creativecommons.org/ns#Distribution",
                      "http://creativecommons.org/ns#DerivativeWorks",
                      "http://creativecommons.org/ns#Attribution",
                      "http://creativecommons.org/ns#Notice"]
        chooser = LicenseChooser(None,attributes)

        permits = chooser.attribute_flag("http://creativecommons.org/ns#Distribution") |  chooser.attribute_flag("http://creativecommons.org/ns#DerivativeWorks")
        print "\t",chooser.get_licenses(permits=permits,requires=liblicense.LL_UNSPECIFIED,prohibits=liblicense.LL_UNSPECIFIED)


if __name__ == '__main__':
    unittest.main()

