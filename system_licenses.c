#include "system_licenses.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <raptor.h>
#include <dirent.h>
#include <unistd.h>
#include <locale.h>
#include <ctype.h>
#include "list.h"

#define MAX_TRIPLES 5

int ll_init() {
	raptor_init();
	setlocale(LC_ALL,"");
	return 0;
}

int ll_stop() {
	raptor_finish();
	return 0;
}

//returns the jurisdiction for the given license.
juris_t ll_get_jurisdiction(const uri_t uri) {
	juris_t* returned = ll_get_attribute(uri,"http://purl.org/dc/elements/1.1/coverage",false);
	if (returned==NULL)
		return NULL;
	else if (returned[0]==NULL) {
		ll_free_list(returned);
		return NULL;
	}
	juris_t result = strdup(returned[0]);
	ll_free_list(returned);
	return result;
}

// returns the name of the license.
char* ll_get_name(const uri_t u) {
	char** returned = ll_get_attribute(u,"http://purl.org/dc/elements/1.1/title",true);
	if (returned==NULL)
		return NULL;
	else if (returned[0]==NULL) {
		ll_free_list(returned);
		return NULL;
	}
	juris_t result = strdup(returned[0]);
	ll_free_list(returned);
	return result;
}

// returns the version of the license.
version_t ll_get_version(const uri_t u) {
	char** versions = ll_get_attribute(u,"http://purl.org/dc/elements/1.1/hasVersion",false);
	char* version;
	if (versions!=NULL && versions[0]!=NULL) {
		version = strdup(versions[0]);
		ll_free_list(versions);
	}
	else
		version = NULL;
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

char** ll_get_prohibits(const uri_t u) {
	return ll_get_attribute(u,"http://creativecommons.org/ns#prohibits",false);
}

char** ll_get_permits(const uri_t u) {
	return ll_get_attribute(u,"http://creativecommons.org/ns#permits",false);
}

char** ll_get_requires(const uri_t u) {
	return ll_get_attribute(u,"http://creativecommons.org/ns#requires",false);
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

attribute_search_t* _ll_get_triple(const uri_t u, const char* subject, const char* predicate, raptor_identifier_type type, int locale) {
	// create the helper struct
	attribute_search_t* helper = (attribute_search_t*) malloc(sizeof(attribute_search_t));
	helper->values = (char**) calloc(MAX_TRIPLES+1,sizeof(char*));
	helper->locales = (char**) calloc(MAX_TRIPLES+1,sizeof(char*));
	helper->num_values = 0;
	helper->uri = strdup(u);
	helper->subject= strdup(subject);
	if (locale)
		helper->locale = _ll_lowercase(setlocale(LC_ALL,NULL));
	else
		helper->locale = NULL;
	if (predicate!=NULL)
		helper->predicate = strdup(predicate);
	else
		helper->predicate = NULL;
	helper->type = type;
	char * fn = _ll_uri_to_filename(u);
	
	// BREAKS_WINDOWS
	if(access(fn,R_OK)==0) {
		// Run the parser.
		raptor_parser* rdf_parser;
	  rdf_parser = raptor_new_parser("rdfxml");
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

void _ll_free_attribute_search_t(attribute_search_t* ast) {
	free(ast->uri);
	free(ast->subject);
	if (ast->predicate!=NULL)
		free(ast->predicate);
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
	  result = (char**) malloc((ast->num_values+1)*sizeof(char*));
	  result[ast->num_values] = NULL;
	  i=0;
	  for (i=0;i<ast->num_values;++i) {
	  	result[i]=strdup(ast->values[i]);
	  }
	} else {
		result = (char**) calloc(2,sizeof(char*));
	  result[1] = NULL;
	  i=0;
	  int match = false;
	  for (i=0;i<ast->num_values;++i) {
	  	if (!match && strcmp(ast->locales[i],"x-default")==0) {
	  		result[0]=strdup(ast->values[i]);
	  	} else if (strcmp(ast->locales[i],ast->locale)==0) {
	  		result[0]=strdup(ast->values[i]);
	  		match = true;
	  	}
	  }
	}
	return result;
}

char** ll_get_attribute(uri_t u,attribute_t a, int locale) {
	attribute_search_t* helper = _ll_get_triple(u,u,a,-1,locale);
	
  
  char* further_search;
  
  char** result = _ll_build_list(helper, &further_search);
  // Free the helper struct
  _ll_free_attribute_search_t(helper);

  if(result==NULL) {
  	helper = _ll_get_triple(u,further_search,NULL,RAPTOR_IDENTIFIER_TYPE_LITERAL,locale);
  	result = _ll_build_list(helper, &further_search);
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
	int z=0;
	int keep=0;
	while(licenses[z]!=NULL) {
		printf("%s\n",licenses[z]);
		if(strcmp(ll_get_jurisdiction(licenses[z]),j)==0)
			keep++;
		else
			licenses[z] = strdup("remove");
		z++;
	}
	printf("%d\n",keep);
	uri_t* result = (uri_t*) malloc((keep+1)*sizeof(uri_t));
	result[keep+1]=NULL;
	z=0;
	int pos = 0;
	while(licenses[z]!=NULL && pos<keep) {
		if(strcmp(licenses[z],"remove")!=0)
			result[pos++] = strdup(licenses[z]);
		z++;
	}
	_ll_free_uri_t_list(licenses);
	return result;
}

