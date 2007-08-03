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

from urllib2 import *
import xml.dom.minidom
import xml.dom.ext
import sys, os
from shutil import copyfile

import polib

locale_map = {
	"country.ar": "Argentina",
	"country.at": "Austria",
	"country.au": "Australia",
	"country.be": "Belgium",
	"country.bg": "Bulgaria",
	"country.br": "Brazil",
	"country.ca": "Canada",
	"country.ch": "Switzerland",
	"country.cl": "Chile",
	"country.cn": "China",
	"country.co": "Colombia",
	"country.de": "Germany",
	"country.dk": "Denmark",
	"country.es": "Spain",
	"country.fi": "Finland",
	"country.fr": "France",
	"country.hr": "Croatia",
	"country.hu": "Hungary",
	"country.il": "Israel",
	"country.in": "India",
	"country.it": "Italy",
	"country.jp": "Japan",
	"country.kr": "South Korea",
	"country.mk": "Macedonia",
	"country.mt": "Malta",
	"country.mx": "Mexico",
	"country.my": "Malaysia",
	"country.nl": "Netherlands",
	"country.pe": "Peru",
	"country.pl": "Poland",
	"country.pt": "Portugal",
	"country.scotland": "UK Scotland",
	"country.se": "Sweden",
	"country.si": "Slovenia",
	"country.tw": "Taiwan",
	"country.uk": "UK Englands and Wales",
	"country.us": "United States",
	"country.za": "South Africa"
}

def find_entry_by_msgid( msgid, po ):
	i = 0
	for e in po:
		if e.msgid == msgid:
			return i
		i += 1
	return -1

if not(os.path.exists(
	os.path.join(os.path.dirname(__file__), 'po')
	)):

	os.mkdir('po')

PO_DIR="https://cctools.svn.sourceforge.net/svnroot/cctools/i18n/trunk/i18n/"

locales = open("LINGUAS","r")

for locale in locales.xreadlines():
	locale = locale.strip()
	copyfile("liblicense.pot","%s.po" % locale)
	out_po = polib.pofile("%s.po" % locale)
	try:
		in_po = polib.pofile("../i18n/icommons-%s.po" % locale)
	except:
		print "getting po file, %s, failed" % locale
	else:
		for entry in in_po:
			if locale_map.has_key(entry.msgid):
				matching_entry_index = find_entry_by_msgid( locale_map[entry.msgid], out_po )
				if matching_entry_index != -1:
					out_po.remove(out_po[matching_entry_index])
					out_po.insert(matching_entry_index,polib.POEntry(locale_map[entry.msgid],msgstr=entry.msgstr))
		out_po.save()

