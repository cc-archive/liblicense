/* Creative Commons has made the contents of this file
 * available under a CC-GNU-LGPL license:
 *
 * http://creativecommons.org/licenses/LGPL/2.1/
 *
 * A copy of the full license can be found as part of this
 * distribution in the file COPYING.
 *
 * You may use the liblicense software in accordance with the
 * terms of that license. You agree that you are solely
 * responsible for your use of the liblicense software and you
 * represent and warrant to Creative Commons that your use
 * of the liblicense software will comply with the CC-GNU-LGPL.
 *
 * Copyright 2007, Creative Commons, www.creativecommons.org.
 * Copyright 2007, Jason Kivlighn.
 * Copyright (C) 2007 Peter Miller
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <raptor.h>

#include <libxml/tree.h>
#include <libxml/parser.h>

#include <liblicense.h>

void _raptor_init()
{
	raptor_init();
}

typedef struct {
	const char* subject;
	char** license;
} match_t;

void triple_handler(void* user_data, const raptor_statement* triple) {
	match_t *match = (match_t*)user_data;

	if (strcmp(match->subject,(char*)triple->subject)==0 &&
			(strcmp("http://creativecommons.org/ns#license",(char*)triple->predicate)==0 ||
			 strcmp("http://web.resource.org/cc/license",(char*)triple->predicate)==0)) {
		*match->license = malloc(sizeof(char)*(strlen((char*)triple->object)+1));
		strcpy(*match->license,(char*)triple->object);
	}
}

char* raptor_read( const char* filename , const ll_uri_t predicate)
{
	char *license;
	raptor_parser* rdf_parser;
	unsigned char* fnu;
	raptor_uri* fn_uri;
	match_t match;

	if (strcmp(predicate, LL_LICENSE) != 0) {
		return NULL; /* We only know License */
	}

	license = NULL;
	rdf_parser = raptor_new_parser("rdfxml");
	if (rdf_parser==NULL) {
		fprintf(stderr,"New parser failed.\n");
		return NULL;
	}

	raptor_set_feature(rdf_parser, RAPTOR_FEATURE_SCANNING, 1);

	fnu = raptor_uri_filename_to_uri_string(filename);
	match.subject = (char *)fnu;
	match.license = &license;
	raptor_set_statement_handler(rdf_parser, &match, triple_handler);

	fn_uri = raptor_new_uri(fnu);
	raptor_parse_file(rdf_parser, fn_uri, fn_uri);

	free(fnu);
	raptor_free_uri(fn_uri);
	raptor_free_parser(rdf_parser);

	return license;
}




typedef struct {
	raptor_serializer *serializer;
	char *old_license;
	int new_ns;
} helper_t;

int write_svg( xmlNode *root, xmlNode *rdf_element )
{
	xmlNode *cur_node = NULL;
	for (cur_node = root->children; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE && strcmp((char*)cur_node->name,"metadata") == 0) {
			for (cur_node = cur_node->children; cur_node; cur_node = cur_node->next) {
				if (strcmp((char*)cur_node->name,"RDF") == 0) {
					xmlNode *parent_node = cur_node->parent;
					xmlUnlinkNode(cur_node);
					xmlFreeNode(cur_node);
					return xmlAddChild(parent_node,rdf_element) != NULL;
				}
			}
		}
	}
	return 1;
}

int write_smil( xmlNode *root, xmlNode *rdf_element )
{
	xmlNode *cur_node = NULL;
	for (cur_node = root->children; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE && strcmp((char*)cur_node->name,"head") == 0) {
			for (cur_node = cur_node->children; cur_node; cur_node = cur_node->next) {
				if (cur_node->type == XML_ELEMENT_NODE && strcmp((char*)cur_node->name,"metadata") == 0) {
					for (cur_node = cur_node->children; cur_node; cur_node = cur_node->next) {
						if (strcmp((char*)cur_node->name,"RDF") == 0) {
							xmlNode *parent_node = cur_node->parent;
							xmlUnlinkNode(cur_node);
							xmlFreeNode(cur_node);
							return xmlAddChild(parent_node,rdf_element) != NULL;
						}
					}
				}
			}
		}
	}
	return 1;
}

void serialize_license( raptor_serializer *serializer, raptor_uri *license_uri, int new_ns )
{
	char **list, **curr;
	raptor_uri *permits_uri;
	raptor_uri *requires_uri;
	raptor_uri *prohibits_uri;

    char * u = raptor_uri_as_string(license_uri);
	raptor_statement license_triple;
	license_triple.subject=(void*)license_uri;
	license_triple.subject_type=RAPTOR_IDENTIFIER_TYPE_RESOURCE;
	license_triple.predicate=(void*)raptor_new_uri((const unsigned char*)"http://www.w3.org/1999/02/22-rdf-syntax-ns#type");
	license_triple.predicate_type=RAPTOR_IDENTIFIER_TYPE_RESOURCE;
	license_triple.object=(void*)raptor_new_uri((const unsigned char*)((new_ns)?"http://creativecommons.org/ns#License":"http://web.resource.org/cc/License"));
	license_triple.object_type=RAPTOR_IDENTIFIER_TYPE_RESOURCE;
	raptor_serialize_statement(serializer, &license_triple);

	raptor_free_uri((raptor_uri*)license_triple.predicate);
	raptor_free_uri((raptor_uri*)license_triple.object);

	permits_uri = raptor_new_uri((const unsigned char*)((new_ns)?"http://creativecommons.org/ns#permits":"http://web.resource.org/cc/permits"));
	curr = list = ll_get_attribute(u, LL_PERMITS, false);
	while (*curr) {
		raptor_statement rs;
		rs.subject = (void*)license_uri;
		rs.subject_type = RAPTOR_IDENTIFIER_TYPE_RESOURCE;
		rs.predicate = (void*)permits_uri;
		rs.predicate_type = RAPTOR_IDENTIFIER_TYPE_RESOURCE;
		rs.object = (void*)raptor_new_uri((const unsigned char*)*curr);
		rs.object_type = RAPTOR_IDENTIFIER_TYPE_RESOURCE;
		raptor_serialize_statement(serializer, &rs);

		raptor_free_uri((raptor_uri*)rs.object);

		curr++;
	}
	raptor_free_uri(permits_uri);
	ll_free_list(list);

	requires_uri = raptor_new_uri((const unsigned char*)((new_ns)?"http://creativecommons.org/ns#requires":"http://web.resource.org/cc/requires"));
	curr = list = ll_get_attribute(u, LL_REQUIRES, false);
	while (*curr) {
		raptor_statement rs;
		rs.subject = (void*)license_uri;
		rs.subject_type = RAPTOR_IDENTIFIER_TYPE_RESOURCE;
		rs.predicate = (void*)requires_uri;
		rs.predicate_type = RAPTOR_IDENTIFIER_TYPE_RESOURCE;
		rs.object = (void*)raptor_new_uri((unsigned char*)*curr);
		rs.object_type = RAPTOR_IDENTIFIER_TYPE_RESOURCE;
		raptor_serialize_statement(serializer, &rs);

		raptor_free_uri((raptor_uri*)rs.object);

		curr++;
	}
	raptor_free_uri(requires_uri);
	ll_free_list(list);

	prohibits_uri = raptor_new_uri((const unsigned char*)((new_ns)?"http://creativecommons.org/ns#prohibits":"http://web.resource.org/cc/prohibits"));
	curr = list = ll_get_attribute(u, LL_PROHIBITS, false);
	while (*curr) {
		raptor_statement rs;
		rs.subject=(void*)license_uri;
		rs.subject_type=RAPTOR_IDENTIFIER_TYPE_RESOURCE;
		rs.predicate=(void*)prohibits_uri;
		rs.predicate_type=RAPTOR_IDENTIFIER_TYPE_RESOURCE;
		rs.object=(void*)raptor_new_uri((unsigned char*)*curr);
		rs.object_type=RAPTOR_IDENTIFIER_TYPE_RESOURCE;
		raptor_serialize_statement(serializer, &rs);

		raptor_free_uri((raptor_uri*)rs.object);

		curr++;
	}
	raptor_free_uri(prohibits_uri);
	ll_free_list(list);
}

void serialize_triple(void* user_data, const raptor_statement* triple)
{
	helper_t *helper = (helper_t*)user_data;

	if (strcmp(triple->predicate,"http://creativecommons.org/ns#license") == 0) {
		helper->new_ns = 1;
		helper->old_license = malloc(sizeof(char)*(strlen((char*)triple->object)+1));
		strcpy(helper->old_license,(char*)triple->object);
	} else if (strcmp(triple->predicate,"http://web.resource.org/cc/license") == 0) {
		helper->new_ns = 0;
		helper->old_license = malloc(sizeof(char)*(strlen((char*)triple->object)+1));
		strcpy(helper->old_license,(char*)triple->object);
	} else if (helper->old_license == NULL || strcmp(triple->subject,helper->old_license) != 0) {
		raptor_serialize_statement(helper->serializer, triple);
	}
}

void declare_namespace(void* user_data, raptor_namespace *nspace)
{
	raptor_serialize_set_namespace_from_namespace((raptor_serializer*)user_data, nspace);
}

int raptor_write( const char* filename, const char* predicate, 
		  const char* license_uri_str )
{
	int ret;
	raptor_parser* rdf_parser;
	raptor_serializer* rdf_serializer;
	unsigned char *uri_string;
	raptor_uri *uri;
        raptor_uri *base_uri;
        raptor_uri *license_uri;
	helper_t helper;
	void *string;  /* destination for string */
	size_t length; /* length of constructed string */
	raptor_statement license_triple;
	xmlDoc *doc;
	xmlDoc *rdf_doc;
	xmlNode *root_element;
	xmlNode *rdf_element;
	xmlNode *cur_node;

	if (strcmp(predicate, LL_LICENSE) != 0) {
		return -LL_E_MODULE_WRITE_FAIL; /* We only know License */
	}

	ret = 0;
	rdf_parser = NULL;
	uri_string=raptor_uri_filename_to_uri_string(filename);
	uri=raptor_new_uri(uri_string);
	base_uri=raptor_uri_copy(uri);
	license_uri=raptor_new_uri((const unsigned char*)license_uri_str);

	rdf_parser=raptor_new_parser("rdfxml");
	rdf_serializer=raptor_new_serializer("rdfxml-abbrev");

	raptor_set_feature(rdf_parser, RAPTOR_FEATURE_SCANNING, 1);

        helper.serializer = rdf_serializer;
        helper.old_license = NULL;
        helper.new_ns = 1;
	raptor_set_statement_handler(rdf_parser, &helper, serialize_triple);
	raptor_set_namespace_handler(rdf_parser, rdf_serializer, declare_namespace);

	free(helper.old_license);

	raptor_serialize_start_to_string(rdf_serializer, base_uri, &string, &length);
	raptor_parse_file(rdf_parser, uri, base_uri);

	if (license_uri_str) {
		license_triple.subject=(void*)raptor_uri_copy(uri);
		license_triple.subject_type=RAPTOR_IDENTIFIER_TYPE_RESOURCE;
		license_triple.predicate=(void*)raptor_new_uri((const unsigned char*)((helper.new_ns)?"http://creativecommons.org/ns#license":"http://web.resource.org/cc/license"));
		license_triple.predicate_type=RAPTOR_IDENTIFIER_TYPE_RESOURCE;
		license_triple.object=(void*)raptor_uri_copy(license_uri);
		license_triple.object_type=RAPTOR_IDENTIFIER_TYPE_RESOURCE;
		raptor_serialize_statement(rdf_serializer, &license_triple);

		serialize_license(rdf_serializer, license_uri, helper.new_ns);
	}

	raptor_serialize_end(rdf_serializer);

	if (license_uri_str) {
		raptor_free_uri((raptor_uri*)license_triple.predicate);
		raptor_free_uri((raptor_uri*)license_triple.subject);
		raptor_free_uri((raptor_uri*)license_triple.object);
	}

	raptor_free_serializer(rdf_serializer);
	raptor_free_parser(rdf_parser);

	raptor_free_uri(base_uri);
	raptor_free_uri(uri);
	raptor_free_uri(license_uri);
	raptor_free_memory(uri_string);

	/*parse the file and get the DOM */
	doc = xmlReadFile(filename, NULL, 0);

	if (doc == NULL) {
			fprintf(stderr,"error: could not parse file %s\n", filename);
			return ret;
	}

	/*Get the root element node */
	root_element = xmlDocGetRootElement(doc);
	rdf_doc = xmlReadMemory(string, length, "noname.xml", NULL, 0);

	raptor_free_memory(string);

	rdf_element = xmlDocCopyNode(rdf_doc->children, doc, 1);

	cur_node = NULL;
	for (cur_node = root_element; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
			if (strcmp((char*)cur_node->name,"svg") == 0) {
				ret = write_svg(root_element, rdf_element);
				break;
			} else if (strcmp((char*)cur_node->name,"smil") == 0) {
				ret = write_smil(root_element, rdf_element);
				break;
			}
		}
	}

	xmlSaveFormatFileEnc(filename, doc, "UTF-8", 1);

	/*free the document */
	xmlFreeDoc(doc);
	xmlFreeDoc(rdf_doc);

	return ret;
}

const char * raptor_supported_predicates[] = {LL_LICENSE, NULL};
const char * raptor_mime_typesp[] = {"image/svg+xml",
				     "application/smil",
				     NULL};


LL_MODULE_DEFINE("raptor.so",
		 "Write licenses in RDF embedded in XML.",
		 "0.1",
		 LL_FEATURES_EMBED,
		 raptor_supported_predicates,
		 raptor_mime_typesp,
		 _raptor_init,
		 raptor_read,
		 raptor_write,
		 NULL /* no shutdown */);
