#!/usr/bin/env python
from distutils.core import setup, Extension
import os
print 
library = "/"+os.path.join(*os.getcwd().split("/")[:-2])+"/liblicense.a"
liblicense = Extension('liblicense',
                    sources = ['liblicense.c'], include_dirs=["../../"], extra_link_args=[library], libraries=['raptor'])

setup (name = 'liblicense',
       version = '0.0.1',
       description = 'This package provides python bindings to liblicense.',
       ext_modules = [liblicense])
