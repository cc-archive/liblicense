#include "system_licenses.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <raptor.h>
#include <dirent.h>

#define MAX_TRIPLES 5

int ll_init() {
	raptor_init();
	return 0;
}

int ll_stop() {
	raptor_finish();
	return 0;
}

//returns the jurisdiction for the given license.
//STUB
juris_t ll_get_jurisdiction(const uri_t uri) {
	return strdup("au");
}

// returns the available locales for the given license.
//STUB
locale_t* ll_get_locales(const uri_t u) {
	locale_t* result = (locale_t*) malloc(2*sizeof(locale_t));
	result[0] = strdup("en_US");
	result[1] = NULL;
	return result;
}

// returns the name of the license.
//STUB
char* ll_get_name(const uri_t u) {
	return strdup("Attribution");
}

// returns the version of the license.
//STUB
version_t ll_get_version(const uri_t u) {
	version_t result = (version_t) malloc(3*sizeof(int));
	result[0] = 3;
	result[1] = 0;
	result[2] = 0;
	return result;
}

// returns the notification string for the given url.
//STUB
char* ll_get_notification(const uri_t u) {
	return strdup("This work is licensed under the...");
}

// returns the notification string for the given uri and verification url.
//STUB
char* ll_get_verifiable_notification(const uri_t u, const char* v) {
	return strdup("This work... verify at blah.");
}

// returns whether or not the given uri is recognized by the system.
//STUB
int ll_verify_uri(const uri_t u) {
	return false;
}

uri_t _ll_filename_to_uri(const filename_t f) {
	uri_t result = (uri_t) malloc((strlen(f)-3)*sizeof(char));
	strncpy(result,f,(strlen(f)-4));
	result[strlen(f)-3-1]='\0';
	int i =0 ;
	while(result[i]!='\0') {
		if (result[i]=='_')
			result[i] = '/';
		++i;
	}
	return result;
}

filename_t _ll_uri_to_filename(const uri_t u) {
	char* tmp_u = strdup(u);
	char* result = (char*) malloc((strlen(LICENSE_DIR)+strlen(u)+4+1)*sizeof(char));
	result[0] = '\0';
	int i =0 ;
	while(tmp_u[i]!='\0') {
		if (tmp_u[i]=='/')
			tmp_u[i] = '_';
		++i;
	}
	result = strcat(result,LICENSE_DIR);
	result = strcat(result,tmp_u);
	result = strcat(result,".rdf");
	free(tmp_u);
	return result;
}

void _ll_triple_handler(void* user_data, const raptor_statement* triple) {
	attribute_search_t* search_data = (attribute_search_t*) user_data;
	// If subject is the license, otherwise its a nested attribute.
	if (strcmp(search_data->uri,(char*)triple->subject)==0)
		// Store value if wanted.
		if (strcmp(search_data->key,(char*)triple->predicate)==0 && search_data->num_values<MAX_TRIPLES) {
			search_data->values[search_data->num_values++] = strdup((char*)triple->object);
		}
}

char** ll_get_attribute(uri_t u,attribute_t a) {
	// create the helper struct
	attribute_search_t* helper = (attribute_search_t*) malloc(sizeof(attribute_search_t));
	helper->values = (char**) malloc(MAX_TRIPLES*sizeof(char*));
	helper->num_values = 0;
	helper->uri = u;
	helper->key = a;
	
	// Run the parser.
	raptor_parser* rdf_parser;
  rdf_parser = raptor_new_parser("rdfxml");
  raptor_set_statement_handler(rdf_parser, helper, _ll_triple_handler);
  unsigned char * fn =  (unsigned char *) _ll_uri_to_filename(u);
  printf("\t\tparsing: %s\n",fn);
  raptor_uri* fn_uri = raptor_new_uri(raptor_uri_filename_to_uri_string(fn));
  int worked = raptor_parse_file(rdf_parser, fn_uri, fn_uri);
  printf("\t\tparse result: %d\n",worked);
  raptor_free_parser(rdf_parser);
  
  // create the list to return
  char** result = (char**) malloc((helper->num_values+1)*sizeof(char*));
  result[helper->num_values+1] = NULL;
  int i;
  for (i=0;i<helper->num_values;++i)
  	result[i] = helper->values[i];
  
  // Free the helper struct
  free(helper->values);
  free(helper);
  
	return result;
}

int _ll_rdf_filter(const struct dirent * d) {
	return strstr(d->d_name,".rdf")!=NULL;
}

// returns a null-terminated list of all general licenses available.
uri_t* ll_get_all_licenses() {
  struct dirent **namelist;
  int n = scandir(LICENSE_DIR, &namelist, _ll_rdf_filter, alphasort);
	uri_t* result = (uri_t*) malloc(n*sizeof(uri_t));
	result[n]=NULL;
  int i;
  for (i=0;i<n;++i) {
    result[i] = _ll_filename_to_uri(namelist[i]->d_name);
    free(namelist[i]);
  }
	return result;
}

// returns a null-terminated list of all general licenses in a family.
//STUB
uri_t* ll_get_licenses(const juris_t j) {
	uri_t* result = (uri_t*) malloc(3*sizeof(uri_t));
	result[0] = strdup("http://creativecommons.org/licenses/by/2.5/ar/");
	result[1] = strdup("http://creativecommons.org/licenses/by-sa/2.5/ar/");
	result[2] = NULL;
	return result;
}

