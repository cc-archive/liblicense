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
# Contributors: Jason Kivlighn.
#               Nathan R. Yergler <nathan@creativecommons.org>.
#

from rdflib.Graph import Graph
from rdflib import Namespace, RDF, URIRef, Literal

from babel.messages.pofile import read_po

from urllib2 import *
import xml.dom.minidom
import xml.dom.ext
import sys, os

if not(os.path.exists(
    os.path.join(os.path.dirname(__file__), 'licenses')
    )):

    os.mkdir('licenses')

LICENSE_FILE="http://cctools.svn.sourceforge.net/svnroot/cctools/license_xsl/trunk/licenses.xml"

NS_DC = Namespace("http://purl.org/dc/elements/1.1/")
NS_DCQ = Namespace("http://purl.org/dc/terms/")
NS_RDF = Namespace("http://www.w3.org/1999/02/22-rdf-syntax-ns#")

NS_CC = Namespace("http://creativecommons.org/ns#")
  
def loadCatalogs(source_dir):
    """Load the translation catalogs and return a dictionary mapping
    the locale code to the PoFile object."""

    langs = {}
    
    for root, dirnames, filenames in os.walk(source_dir):
        for fn in filenames:
            if fn[-3:] == '.po':

                # figure out what locale this is based on pathname
                locale = root.split(os.sep)[-1]
                print 'loading catalog for %s...' % locale
                
                msg_catalog = read_po(
                    file(os.path.abspath(os.path.join(root, fn)), 'r'))
                
                langs[locale] = msg_catalog

    return langs

LOCALES = loadCatalogs(os.path.join(os.path.dirname(__file__), 'i18n'))
  
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


            if id == "devnations":
                title_str = "util.Developing_Nations"
            else:
                title_str = "licenses.pretty_%s" % id

            for locale in LOCALES:
                # add the dc:title nodes
                store.add((license_node, NS_DC['title'], 
                           Literal(LOCALES[locale][title_str].string, 
                                   lang=locale)
                           ))

                # add the dc:description nodes
                desc = LOCALES[locale]["char.%s_description" % id]
                if desc is not None:
                    store.add((license_node, NS_DC.description, 
                               Literal(desc.string, lang=locale)
                              ))

            output = "licenses/%s.rdf" % uri.lstrip("http://").replace("/","_")
            output_file = open(output,"w")
            output_file.write(store.serialize(format="pretty-xml"))

            output_file.close()

print "Licenses output to licenses/"
