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
from rdflib import Namespace, RDF, URIRef, Literal

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

PO_DIR="http://cctools.svn.sourceforge.net/svnroot/cctools/i18n/trunk/i18n/"
LICENSE_FILE="http://cctools.svn.sourceforge.net/svnroot/cctools/license_xsl/trunk/licenses.xml"

NS_DC = Namespace("http://purl.org/dc/elements/1.1/")
NS_DCQ = Namespace("http://purl.org/dc/terms/")
NS_RDF = Namespace("http://www.w3.org/1999/02/22-rdf-syntax-ns#")

NS_CC = Namespace("http://creativecommons.org/ns#")

#x-default should be first in the list
xDefault = "en"
locales = [xDefault, "af","bg","ca","da","de","de_AT","de_CH","en_CA",
    "en_GB","en_US","eo","es","es_AR","es_CL","es_CO","es_MX","es_PE",
    "eu","fi","fr","fr_CA","fr_CH","fr_LU","gl","he","hr","hu","it","it_CH",
    "ja","ko","mk","ms","nl","nso","pl","pt","pt_PT","sl","st","sv",
    "zh","zh_TW","zu"]
    
conn = urlopen(LICENSE_FILE)
license_xml = xml.dom.minidom.parse(conn)

def latest_license( v, acc ):
    if v == None or acc[1] == "-" or float(acc[1]) > float(v[1]):
        return acc
    else:
        return v

all_uris = [version.getAttribute('uri') 
            for version in license_xml.getElementsByTagName('version')]

licenses = license_xml.getElementsByTagName('license')

print [license.getAttribute('id') for license in licenses]

for license in licenses:
    id = license.getAttribute('id')
    print "Processing '%s' licenses" % id
    jurisdictions = license.getElementsByTagName('jurisdiction')
    for jurisdiction in jurisdictions:
        jurisdiction_id = jurisdiction.getAttribute('id')
        versions = jurisdiction.getElementsByTagName('version')

        replacedByURI, _ = reduce(latest_license,[(version.getAttribute('uri'),version.getAttribute('id')) for version in versions])

        for version in versions:

            version_id = version.getAttribute('id')
            uri = version.getAttribute('uri')
            
            # Handle by-nd-nc -> by-nc-nd change
            if version_id == "1.0" and id == "by-nd-nc":
                replacedByURI = "http://creativecommons.org/licenses/by-nc-nd/2.0/"
                if jurisdiction_id != "-":
                    portedReplacement = replacedByURI + jurisdiction_id+"/"
                    if portedReplacement in all_uris:
                        replacedByURI = portedReplacement
                    else:
                        print "No ported replacement for:", uri
                        print "Using unported:",replacedByURI
            
            # Handle obsoleted licenses without 'by'
            if version_id == "1.0" and id in ["sa","nc","nd","nc-sa","nc-nd","nd-nc"]:
                if id == "nd-nc":
                    replacedId = "by-nc-nd"
                else:
                    replacedId = "by-"+id
                    
                replacedByURI = "http://creativecommons.org/licenses/"+replacedId+"/2.0/"
                if jurisdiction_id != "-":
                    portedReplacement = replacedByURI + jurisdiction_id+"/"
                    if portedReplacement in all_uris:
                        replacedByURI = portedReplacement
                    else:
                        print "No ported replacement for:", uri
                        print "Using unported:",replacedByURI
            
            
            store = Graph()
            store.bind("cc", "http://creativecommons.org/ns#")
            store.bind("dc", "http://purl.org/dc/elements/1.1/")

            store.load('%srdf' % uri)

            license_node = URIRef(uri)

            if version_id != "-":
                store.add((license_node, NS_DC['hasVersion'], 
                           Literal(version_id)))

            if jurisdiction_id != "-":
                store.add((license_node, NS_CC.jurisdiction,
                          URIRef("http://creativecommons.org/international/%s/" % jurisdiction_id)))

            if replacedByURI != uri:

                store.add((license_node, NS_DC['isReplacedBy'], 
                           URIRef(replacedByURI)))


            if id in ["GPL","LGPL"]:
                creator_str = "Free Software Foundation"
            elif id == "publicdomain":
                creator_str = None
            else:
                creator_str = "Creative Commons"

            if creator_str:
                store.add((license_node, NS_DC.creator, 
                           Literal(creator_str)))

            store.add((license_node, NS_DC.publisher, 
                       Literal("Creative Commons")))

            if jurisdiction_id != "-":

                basedOnURI = uri.rsplit("/",2)[0]+"/"

                store.add((license_node, NS_DC.isBasedOn, basedOnURI))

            print store.serialize(format="pretty-xml")
            break

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
            output_file.write(store.serialize(format="pretty-xml"))

            output_file.close()

print "Licenses output to licenses/"
