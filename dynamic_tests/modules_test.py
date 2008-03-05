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
# Copyright 2007, Jason Kivlighn.

import liblicense as ll

import os
from shutil import copyfile

all_modules = ll.get_io_modules()
untested_modules = ll.get_io_modules()
failed_modules = []

def call_read( filename, module ):
	if module:
		return ll.read(filename,module)
	else:
		return ll.read(filename)

def call_write( filename, license, module ):
	if module:
		return ll.write(filename,license,module)
	else:
		return ll.write(filename,license)

def flex_module( filename, module=None ):
	if module:
		print "Testing %s (module: %s)" % (filename,module)
		if module not in all_modules:
			print "\tSkipping: module not installed"
	else:
		print "Testing %s (module: auto)" % filename

	copyfile("content/"+filename, filename)
	
	call_write(filename,None,module)
	uri = call_read(filename,module)
	if uri != None:
		if uri == "":
			print "1) Warning: empty license field exists"
		else:
			print "1) Didn't clear.  URI is: %s" % uri
			failed_modules.append(module)
			return False
	
	call_write(filename,"http://creativecommons.org/licenses/by/1.0/",module)
	
	uri = call_read(filename,module)
	if uri != "http://creativecommons.org/licenses/by/1.0/":
		print "Read or write failed"
		failed_modules.append(module)
		return False
	
	call_write(filename,None,module)
	uri = call_read(filename,module)
	if uri != None:
		if uri == "":
			print "2) Warning: empty license field exists"
		else:
			print "2) Didn't clear.  URI is: %s" % uri
			failed_modules.append(module)
			return False
	
	#Try removing a license on a file without a license, after one has been set before
	call_write(filename,None,module)
	uri = call_read(filename,module)
	if uri != None:
		if uri == "":
			print "3) Warning: empty license field exists"
		else:
			print "3) Didn't clear.  URI is: %s" % uri
			failed_modules.append(module)
			return False

	call_write(filename,"http://creativecommons.org/licenses/by/3.0/",module)
	uri = call_read(filename,module)
	if uri != "http://creativecommons.org/licenses/by/3.0/":
		print "Read or write failed after clearing"
		failed_modules.append(module)
		return False
	
	copyfile(filename, "tested-"+filename)
	os.unlink(filename)
	try:
		os.unlink(filename.split(".",1)[0]+".xmp")
	except: pass

	if module in untested_modules:
		untested_modules.remove(module)

	return True

flex_module("test.txt","sidecar_xmp.so")
flex_module("test.txt")
flex_module("test.pdf","exempi.so")
flex_module("test.pdf")
flex_module("test.png","exempi.so")
flex_module("test.png")
flex_module("test.jpg","exempi.so")
flex_module("test.jpg")
flex_module("test.tif","exempi.so")
flex_module("test.tif")
flex_module("test.mp3","id3.so")
flex_module("test.mp3")
flex_module("test.flac","flac.so")
flex_module("test.flac","taglib.so")
flex_module("test.flac")
flex_module("test.ogg","vorbis.so")
flex_module("test.ogg","taglib.so")
flex_module("test.ogg")
flex_module("test.wav","exempi.so")
flex_module("test.wav")
flex_module("test.doc","gsf.so")
flex_module("test.doc")
flex_module("test.svg","raptor.so")
flex_module("test.svg")
flex_module("test.pdf","exempi.so")
flex_module("test.pdf")

print

print "Untested modules:"
print untested_modules

print "Failed modules:"
print failed_modules