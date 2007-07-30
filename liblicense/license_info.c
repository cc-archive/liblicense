// Creative Commons has made the contents of this file
// available under a CC-GNU-LGPL license:
//
// http://creativecommons.org/licenses/LGPL/2.1/
//
// A copy of the full license can be found as part of this
// distribution in the file COPYING.
// 
// You may use the liblicense software in accordance with the
// terms of that license. You agree that you are solely 
// responsible for your use of the liblicense software and you
// represent and warrant to Creative Commons that your use
// of the liblicense software will comply with the CC-GNU-LGPL.
//
// Copyright 2007, Creative Commons, www.creativecommons.org.
// Copyright 2007, Scott Shawcroft.

#include "liblicense.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <raptor.h>
#include <dirent.h>
#include <unistd.h>
#include <locale.h>
#include <ctype.h>
#include <assert.h>

#define MAX_TRIPLES 5

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// returns the first element of a list and frees the rest.
char* _ll_get_first(char** list) {
	if (list==NULL)
		return NULL;
	else if (list[0]==NULL) {
		ll_free_list(list);
		return NULL;
	}
	char* result = strdup(list[0]);
	ll_free_list(list);
	return result;
}

//returns the jurisdiction for the given license.
juris_t ll_get_jurisdiction(const uri_t uri) {
	return _ll_get_first(ll_get_attribute(uri,"http://purl.org/dc/elements/1.1/coverage",false));
}

// returns the name of the license.
char* ll_get_name(const uri_t u) {
	return _ll_get_first(ll_get_attribute(u,"http://purl.org/dc/elements/1.1/title",true));
}

// returns the version of the license.
version_t ll_get_version(const uri_t u) {
	char* version = _ll_get_first(ll_get_attribute(u,"http://purl.org/dc/elements/1.1/hasVersion",false));
	int c = 0;
	int position = 0;
	version_t result = (version_t) calloc(3,sizeof(int));
	while(version!=NULL && version[c]!='\0' && position<3) {
		if (version[c]!='.') {
			result[position] *= 10;
			result[position] += atoi(&version[c]);
		} else {
			position++;
		}
		c++;			
	}
	if (version!=NULL)
		free(version);
	return result;
}

// returns a list of uris which the license prohibits
char** ll_get_prohibits(const uri_t u) {
	return ll_get_attribute(u,"http://creativecommons.org/ns#prohibits",false);
}

// returns a list of uris which the license permits
char** ll_get_permits(const uri_t u) {
	return ll_get_attribute(u,"http://creativecommons.org/ns#permits",false);
}

// returns a list of uris which the license requires
char** ll_get_requires(const uri_t u) {
	return ll_get_attribute(u,"http://creativecommons.org/ns#requires",false);
}

// converts a filename string to a uri
uri_t ll_filename_to_uri(const filename_t f) {
	uri_t result = (uri_t) malloc((strlen(f)-3+7)*sizeof(char));

	strcpy(result,"http://");
	strncpy(&result[7],f,(strlen(f)-4));
	result[strlen(f)-3+7-1]='\0';

	int i =0 ;
	while(result[i]!='\0') {
		if (result[i]=='_')
			result[i] = '/';
		++i;
	}
	return result;
}

// converts a uri string to a filename
filename_t ll_uri_to_filename(const uri_t u) {
	assert( strncmp(u,"http://",7) == 0 );

	char* tmp_u = strdup(&u[7]);
	char* result = (char*) malloc((strlen(LICENSE_DIR)+strlen(tmp_u)+4+1)*sizeof(char));
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

// triple callback which stores the desired values in a struct.
void _ll_triple_handler(void* user_data, const raptor_statement* triple) {
	attribute_search_t* search_data = (attribute_search_t*) user_data;
	if (strcmp(search_data->subject,(char*)triple->subject)==0) {
		// Store value if wanted.
		if ((search_data->predicate==NULL || strcmp(search_data->predicate,(char*)triple->predicate)==0)  //Any predicate or the given.
		  && search_data->num_values<MAX_TRIPLES // We have space for more results.
		  && (search_data->type==-1 || search_data->type==triple->object_type) //Any type or the given.
		  && (search_data->locale==NULL || triple->object_literal_language==NULL || strcmp((char*)triple->object_literal_language,"x-default")==0 || strcmp((char*)triple->object_literal_language,search_data->locale)==0)) { //locale stuff
			search_data->values[search_data->num_values] = strdup((char*)triple->object);
			if (triple->object_literal_language!=NULL)
				search_data->locales[search_data->num_values++] = strdup((char*)triple->object_literal_language);
			else
				search_data->locales[search_data->num_values++] = strdup("x-default");
		}
	}
}

// converts a string to lowercase
char* _ll_lowercase(char* string) {
	int pos;
	char* new_string = strdup(string);
	pos=0;
	while(string[pos]!='\0') {
		new_string[pos] = tolower(string[pos]);
		pos++;
	}
	return new_string;
}

/* Normalizes a locale to compare against the xml:lang attribute */
char* _ll_normalize_locale(char* string) {
	char* new_string = _ll_lowercase(string);

	char *codeset = strchr(new_string,'.');
	if (codeset) {
		*codeset = '\0';
	}

	char *territory = strchr(new_string,'_');
	if (territory) {
		if ( strncmp(new_string,territory+1,strlen(territory+1)) == 0 ) {
			*territory = '\0';
		} else {
			*territory = '-';
		}
	}
	return new_string;
}

// helper function which runs the parser
attribute_search_t* _ll_get_triple(const uri_t u, const char* subject, const char* predicate, raptor_identifier_type type, int locale) {
	// create the helper struct
	attribute_search_t* helper = (attribute_search_t*) malloc(sizeof(attribute_search_t));
	helper->values = ll_new_list(MAX_TRIPLES);
	helper->locales = ll_new_list(MAX_TRIPLES);
	helper->num_values = 0;
	helper->uri = strdup(u);
	helper->subject= strdup(subject);
	if (locale)
		helper->locale = _ll_normalize_locale(setlocale(LC_ALL,NULL));
	else
		helper->locale = NULL;
	if (predicate!=NULL)
		helper->predicate = strdup(predicate);
	else
		helper->predicate = NULL;
	helper->type = type;
	char * fn = ll_uri_to_filename(u);
	
	// BREAKS_WINDOWS
	if(access(fn,R_OK)==0) {
		// Run the parser.
		raptor_parser* rdf_parser;
	  rdf_parser = raptor_new_parser("rdfxml");
	  if (rdf_parser==NULL) {
	  	fprintf(stderr,"New parser failed.\n");
	  	exit(1);
	  }
	  raptor_set_statement_handler(rdf_parser, helper, _ll_triple_handler);
	  unsigned char* fnu = raptor_uri_filename_to_uri_string(fn);
	  raptor_uri* fn_uri = raptor_new_uri(fnu);
	  free(fnu);
	  raptor_parse_file(rdf_parser, fn_uri, fn_uri);
	  raptor_free_uri(fn_uri);
	  raptor_free_parser(rdf_parser);
  }
  free(fn);
  return helper;
}

// frees the search helper struct
void _ll_free_attribute_search_t(attribute_search_t* ast) {
	free(ast->uri);
	free(ast->subject);
	if (ast->predicate!=NULL)
		free(ast->predicate);
	if (ast->locale!=NULL)
		free(ast->locale);
	ll_free_list(ast->locales);
	ll_free_list(ast->values);
	free(ast);
}
// create the list to return from a attribute_search struct
char** _ll_build_list(attribute_search_t * ast, char** fsi) {
	// check for generics
	int i;
	for (i=0;i<ast->num_values;++i) {
	  	if(strstr(ast->values[i],"genid")!=NULL) {
	  		*fsi = strdup(ast->values[i]);
	  		return NULL;
	  	}
	}
	char** result;
	// create the list to return
	if (ast->locale==NULL) {
	  result = ll_new_list(ast->num_values);
	  i=0;
	  for (i=0;i<ast->num_values;++i) {
	  	result[i]=strdup(ast->values[i]);
	  }
	} else {
		result = ll_new_list(1);
	  i=0;
	  int match = false;
	  for (i=0;i<ast->num_values;++i) {
	  	if (!match && strcmp(ast->locales[i],"x-default")==0) {
	  		if (result[0]!=NULL)
					free(result[0]);
	  		result[0]=strdup(ast->values[i]);
	  	} else if (strcmp(ast->locales[i],ast->locale)==0) {
				if (result[0]!=NULL)
					free(result[0]);
	  		result[0]=strdup(ast->values[i]);
	  		match = true;
	  	}
	  }
	}
	return result;
}
// calls the helper parser function to get nodes with a depth of 1 or 2
char** ll_get_attribute(uri_t u,attribute_t a, int locale) {
	if ( strncmp(u,"http://",7) != 0 ) return NULL;

	attribute_search_t* helper = _ll_get_triple(u,u,a,-1,locale);

  char* further_search = NULL;
  
  char** result = _ll_build_list(helper, &further_search);
  // Free the helper struct
  _ll_free_attribute_search_t(helper);

  if(result==NULL) {
  	helper = _ll_get_triple(u,further_search,NULL,RAPTOR_IDENTIFIER_TYPE_LITERAL,locale);
  	result = _ll_build_list(helper, &further_search);
	  _ll_free_attribute_search_t(helper);
  }
  if (further_search!=NULL)
  	free(further_search);
  return result;
}