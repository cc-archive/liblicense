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

from rdflib.Graph import Graph
from rdflib import Namespace, RDF

from urllib2 import *
import xml.dom.minidom
import xml.dom.ext
import sys, os

if not(os.path.exists(
	os.path.join(os.path.dirname(__file__), 'licenses')
	)):

	os.mkdir('licenses')

header = """
Creative Commons has made the contents of this file
available under a CC-GNU-LGPL license:

 http://creativecommons.org/licenses/LGPL/2.1/

 A copy of the full license can be found as part of this
 distribution in the file COPYING

You may use the liblicense software in accordance with the
terms of that license. You agree that you are solely 
responsible for your use of the liblicense software and you
represent and warrant to Creative Commons that your use
of the liblicense software will comply with the CC-GNU-LGPL.

Copyright 2007, Creative Commons, www.creativecommons.org.

"""

PO_DIR="https://cctools.svn.sourceforge.net/svnroot/cctools/i18n/trunk/i18n/"
LICENSE_FILE="https://cctools.svn.sourceforge.net/svnroot/cctools/license_xsl/trunk/licenses.xml"

NS_DC = "http://purl.org/dc/elements/1.1/"
NS_DCQ = "http://purl.org/dc/terms/"
NS_RDF = "http://www.w3.org/1999/02/22-rdf-syntax-ns#"

NS_CC = Namespace("http://creativecommons.org/ns#")

#x-default should be first in the list
xDefault = "en"
locales = [xDefault, "af","bg","ca","da","de","de_AT","de_CH","en_CA",
	"en_GB","en_US","eo","es","es_AR","es_CL","es_CO","es_MX","es_PE",
	"eu","fi","fr","fr_CA","fr_CH","fr_LU","gl","he","hr","hu","it","it_CH",
	"ja","ko","kr","mk","ms","nl","nso","pl","pt","pt_PT","sl","st","sv",
	"zh","zh_TW","zu"]
	
conn = urlopen(LICENSE_FILE)
license_xml = xml.dom.minidom.parse(conn)

def latest_license( v, acc ):
	if v == None or acc[1] == "-" or float(acc[1]) > float(v[1]):
		return acc
	else:
		return v

licenses = license_xml.getElementsByTagName('license')
print [license.getAttribute('id') for license in licenses]
for license in licenses:
	id = license.getAttribute('id')
	print "Processing '%s' licenses" % id
	jurisdictions = license.getElementsByTagName('jurisdiction')
	for jurisdiction in jurisdictions:
		jurisdiction_id = jurisdiction.getAttribute('id')
		versions = jurisdiction.getElementsByTagName('version')
		#replacedByURI = versions[-1].getAttribute('uri')
		replacedByURI, _ = reduce(latest_license,[(version.getAttribute('uri'),version.getAttribute('id')) for version in versions])
		for version in versions:
			version_id = version.getAttribute('id')
			uri = version.getAttribute('uri')
			
			store = Graph()
			
			try:
				store.load('%srdf' % uri)
			except:
				print uri, "failed"
				pass
			else:
				doc = xml.dom.minidom.Document()
				doc.appendChild( doc.createComment(header) )
				
				rdf = doc.createElementNS(NS_RDF, "rdf:RDF")
				doc.appendChild( rdf )
				
				description = doc.createElementNS(NS_RDF, "rdf:Description")
				description.setAttributeNS(NS_RDF,"rdf:about",uri)
				rdf.appendChild( description )
				
				if version_id != "-":
					hasVersion = doc.createElementNS(NS_DC, "dc:hasVersion")
					hasVersion.appendChild( doc.createTextNode(version_id) )
					description.appendChild( hasVersion )
					
				if replacedByURI != uri:
					isReplacedBy = doc.createElementNS(NS_DC, "dc:isReplacedBy")
					isReplacedByURI = doc.createElementNS(NS_DCQ, "dcq:URI")
					isReplacedByURIValue = doc.createElementNS(NS_RDF,"rdf:value")
					isReplacedByURIValue.appendChild( doc.createTextNode(replacedByURI) )
					isReplacedByURI.appendChild( isReplacedByURIValue )
					isReplacedBy.appendChild( isReplacedByURI )
					description.appendChild( isReplacedBy )
				
				type = doc.createElementNS(NS_DC, "dc:type")
				type.appendChild( doc.createTextNode("License") )
				description.appendChild( type )
				
				identifier = doc.createElementNS(NS_DC, "dc:identifier")
				identifier.appendChild( doc.createTextNode(uri) )
				description.appendChild( identifier )
				
				if id in ["GPL","LGPL"]:
					creator_str = "Free Software Foundation"
				elif id == "publicdomain":
					creator_str = None
				else:
					creator_str = "Creative Commons"
					
				if creator_str:
					creator = doc.createElementNS(NS_DC, "dc:creator")
					creator.appendChild( doc.createTextNode(creator_str) )
					description.appendChild( creator )
					
				publisher = doc.createElementNS(NS_DC, "dc:publisher")
				publisher.appendChild( doc.createTextNode("Creative Commons") )
				description.appendChild( publisher )
				
				if jurisdiction_id != "-":
					coverage = doc.createElementNS(NS_DC, "dc:coverage")
					coverageCode = doc.createElementNS(NS_DCQ, "dcq:ISO3166")
					coverageCodeValue = doc.createElementNS(NS_RDF,"rdf:value")
					coverageCodeValue.appendChild( doc.createTextNode(jurisdiction_id) )
					coverageCode.appendChild( coverageCodeValue )
					coverage.appendChild( coverageCode )
					description.appendChild( coverage )
					
					basedOnURI = uri.rsplit("/",2)[0]+"/"
					isBasedOn = doc.createElementNS(NS_DC, "dc:isBasedOn")
					isBasedOnURI = doc.createElementNS(NS_DCQ, "dcq:URI")
					isBasedOnURIValue = doc.createElementNS(NS_RDF,"rdf:value")
					isBasedOnURIValue.appendChild( doc.createTextNode(basedOnURI) )
					isBasedOnURI.appendChild( isBasedOnURIValue )
					isBasedOn.appendChild( isBasedOnURI )
					description.appendChild( isBasedOn )
				
				rdfType = doc.createElementNS(NS_RDF, "rdf:type")
				rdfType.setAttributeNS(NS_RDF,"rdf:resource","http://creativecommons.org/ns#License")
				description.appendChild( rdfType )

				for license in store.subjects(RDF.type, NS_CC["License"]):
					for requires in store.objects(license, NS_CC["requires"]):
						element = doc.createElementNS(NS_CC, "requires")
						element.setAttributeNS(NS_RDF,"rdf:resource",str(requires))
						description.appendChild( element )
					for permits in store.objects(license, NS_CC["permits"]):
						element = doc.createElementNS(NS_CC, "permits")
						element.setAttributeNS(NS_RDF,"rdf:resource",str(permits))
						description.appendChild( element )
					for prohibits in store.objects(license, NS_CC["prohibits"]):
						element = doc.createElementNS(NS_CC, "prohibits")
						element.setAttributeNS(NS_RDF,"rdf:resource",str(prohibits))
						description.appendChild( element )
						
				
				translation_map = {}
				
				title = doc.createElementNS(NS_DC, "dc:title")
				title_alt = doc.createElementNS(NS_RDF, "rdf:Alt")
				title.appendChild( title_alt )
				description.appendChild(title)
				if id == "devnations":
					translation_map['msgid "util.Developing_Nations"'] = title_alt
				else:
					translation_map['msgid "licenses.pretty_%s"' % id] = title_alt
				
				dcDescription = doc.createElementNS(NS_DC, "dc:description")
				dcDescription_alt = doc.createElementNS(NS_RDF, "rdf:Alt")
				dcDescription.appendChild( dcDescription_alt )
				translation_map['msgid "char.%s_description"' % id] = dcDescription_alt
				
				for locale in locales:
					try:
						#conn = urlopen(PO_DIR+"icommons-%s.po" % locale)
						conn = open("i18n/icommons-%s.po" % locale,"r")
					except:
						print "getting po file, %s, failed" % locale
					else:
						lines = conn.readlines()
						i = 0
						while i < len(lines):
							line = lines[i].decode("utf8").strip()
							element = translation_map.get(line)
							if element:
								i += 1
								msgstr = lines[i].strip().lstrip('msgstr "').rstrip('"').replace("\\n"," ")
								li = doc.createElementNS(NS_RDF,"rdf:li")
								if locale == xDefault:
									li.setAttributeNS(xml.dom.XML_NAMESPACE,"xml:lang","x-default")
								else:
									li.setAttributeNS(xml.dom.XML_NAMESPACE,"xml:lang",locale.replace("_","-"))
								
								if creator_str == "Creative Commons" and line.find("description") == -1:
									msgstr = "Creative Commons - "+msgstr
								li.appendChild( doc.createTextNode(msgstr) )
								
								element.appendChild( li )
							i += 1
				
				if dcDescription_alt.hasChildNodes():
					description.appendChild(dcDescription)
				
				output = "licenses/%s.rdf" % uri.lstrip("http://").replace("/","_")
				output_file = open(output,"w")
				xml.dom.ext.PrettyPrint(doc,output_file)
				output_file.close()

print "Licenses output to licenses/"
