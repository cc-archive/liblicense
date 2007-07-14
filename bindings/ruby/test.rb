#!/usr/bin/env ruby

#
# Ruby/Liblicense Test Suite (of sorts)
#
# Copyright 2007, Creative Commons, www.creativecommons.org.
# Copyright 2007, Alex Roberts.
#

$LOAD_PATH.unshift "."
$LOAD_PATH.unshift "./lib"
require 'liblicense'

def run_test(l)
	puts "Name:      " + l.name
	puts "Version:   " + l.version.join(".")
	puts "Juris:     " + l.jurisdiction
	puts "URI:       " + l.uri
	puts "Permits:   " + l.permits.join("\n           ")
	puts "Prohibits: " + l.prohibits.join("\n           ")
	puts "Requires:  " + l.requires.join("\n           ")
end

l = Liblicense.new 'http://creativecommons.org/licenses/by/3.0/'
run_test l
puts "to_s:      " + l.to_s


puts "\n== Change uri =="

l.uri = 'http://creativecommons.org/licenses/by-sa/2.5/au/'
run_test l

