#include "system_licenses.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <raptor.h>
#include <dirent.h>
#include <unistd.h>

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
juris_t ll_get_jurisdiction(const uri_t uri) {
	return ll_get_attribute(uri,"http://purl.org/dc/elements/1.1/coverage")[0];
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
char* ll_get_name(const uri_t u) {
	return ll_get_attribute(u,"http://purl.org/dc/elements/1.1/title")[0];
}

// returns the version of the license.
version_t ll_get_version(const uri_t u) {
	char* version = ll_get_attribute(u,"http://purl.org/dc/elements/1.1/hasVersion")[0];
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

void _ll_free_uri_t_list(uri_t* us) {
	int i =0;
	while(us[i]!=NULL) {
		free(us[i]);
		i++;
	}
	free(us);
}

// returns whether or not the given uri is recognized by the system.
int ll_verify_uri(const uri_t u) {
	uri_t* licenses = ll_get_all_licenses();
	int i =0;
	while(licenses[i]!=NULL) {
		if (strcmp(licenses[i],u)==0) {
			_ll_free_uri_t_list(licenses);
			return true;
		}
		i++;
	}
	_ll_free_uri_t_list(licenses);
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
	if (strcmp(search_data->subject,(char*)triple->subject)==0)
		// Store value if wanted.
		if ((search_data->predicate==NULL || strcmp(search_data->predicate,(char*)triple->predicate)==0)  //Any predicate or the given.
		  && search_data->num_values<MAX_TRIPLES // We have space for more results.
		  && (search_data->type==-1 || search_data->type==triple->object_type)) { //Any type or the given.
			search_data->values[search_data->num_values++] = strdup((char*)triple->object);
		}
}

attribute_search_t* _ll_get_triple(const uri_t u, const char* subject, const char* predicate, raptor_identifier_type type) {
	// create the helper struct
	attribute_search_t* helper = (attribute_search_t*) malloc(sizeof(attribute_search_t));
	helper->values = (char**) calloc(MAX_TRIPLES+1,sizeof(char*));
	helper->num_values = 0;
	helper->uri = strdup(u);
	helper->subject= strdup(subject);
	if (predicate!=NULL)
		helper->predicate = strdup(predicate);
	else
		helper->predicate = NULL;
	helper->type = type;
	char * fn = _ll_uri_to_filename(u);
	
	if(access(fn,R_OK)==0) {
		// Run the parser.
		raptor_parser* rdf_parser;
	  rdf_parser = raptor_new_parser("rdfxml");
	  raptor_set_statement_handler(rdf_parser, helper, _ll_triple_handler);
	  
	  raptor_uri* fn_uri = raptor_new_uri(raptor_uri_filename_to_uri_string(fn));
	  raptor_parse_file(rdf_parser, fn_uri, fn_uri);
	  raptor_free_parser(rdf_parser);
  }
  
  return helper;
}

// Frees a null terminated list of strings.
void _ll_free_char_list(char** list) {
	int i=0;
	while(list[i]!=NULL) {
		fflush(stdout);
		free(list[i]);
		i++;
	}
	free(list);
}

void _ll_free_attribute_search_t(attribute_search_t* ast) {
	free(ast->uri);
	free(ast->subject);
	if (ast->predicate!=NULL)
		free(ast->predicate);
	_ll_free_char_list(ast->values);
	free(ast);
}

char** ll_get_attribute(uri_t u,attribute_t a) {
	attribute_search_t* helper = _ll_get_triple(u,u,a,-1);
	
  // create the list to return
  char** result = (char**) malloc((helper->num_values+1)*sizeof(char*));
  result[helper->num_values] = NULL;
  int i;
  int further_search = false;
  int further_search_index = 0;
  for (i=0;i<helper->num_values;++i) {
  	if(strstr(helper->values[i],"gen")!=NULL) {
  		further_search = true;
  		further_search_index = i;
  	}
  	result[i] = strdup(helper->values[i]);
  }
  
  // Free the helper struct
  _ll_free_attribute_search_t(helper);

  if(further_search) {
  	helper = _ll_get_triple(u,result[further_search_index],NULL,RAPTOR_IDENTIFIER_TYPE_LITERAL);
  	result = (char**) malloc((helper->num_values+1)*sizeof(char*));
	  result[helper->num_values] = NULL;
	  int i;
	  for (i=0;i<helper->num_values;++i) {
	  	result[i] = strdup(helper->values[i]);
	  }
	  _ll_free_attribute_search_t(helper);
  }
  return result;
}

int _ll_rdf_filter(const struct dirent * d) {
	return strstr(d->d_name,".rdf")!=NULL;
}

// returns a null-terminated list of all general licenses available.
uri_t* ll_get_all_licenses() {
  struct dirent **namelist;
  int n = scandir(LICENSE_DIR, &namelist, _ll_rdf_filter, alphasort);
	uri_t* result = (uri_t*) malloc((n+1)*sizeof(uri_t));
	result[n]=NULL;
  int i;
  for (i=0;i<n;++i) {
    result[i] = _ll_filename_to_uri(namelist[i]->d_name);
    free(namelist[i]);
  }
	return result;
}

// returns a null-terminated list of all general licenses in a family.
uri_t* ll_get_licenses(const juris_t j) {
	uri_t* licenses = ll_get_all_licenses();
	int i=0;
	int keep=0;
	while(licenses[i]!=NULL) {
		if(strcmp(ll_get_jurisdiction(licenses[i]),j)==0)
			keep++;
		else
			licenses[i] = strdup("remove");
		i++;
	}
	uri_t* result = (uri_t*) malloc((keep+1)*sizeof(uri_t));
	result[keep+1]=NULL;
	i=0;
	int pos = 0;
	while(licenses[i]!=NULL) {
		if(strcmp(licenses[i],"remove")!=0)
			result[pos++] = strdup(licenses[i]);
		i++;
	}
	_ll_free_uri_t_list(licenses);
	return result;
}

