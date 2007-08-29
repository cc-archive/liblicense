/*
 * Creative Commons has made the contents of this file
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
 * Copyright 2007, Scott Shawcroft.
 * Copyright (C) 2007 Peter Miller
 */

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

/**
 * returns the first element of a list and frees the rest.
 */
char* _ll_get_first(char** list) {
	char* result;

	if (list==NULL)
		return NULL;
	else if (list[0]==NULL) {
		ll_free_list(list);
		return NULL;
	}
	result = strdup(list[0]);
	ll_free_list(list);
	return result;
}

#define _(x) (x)

typedef struct {
	ll_juris_t jurisdiction;
	const char *name;
} jurisdiction_map_t;

jurisdiction_map_t jurisdiction_map[] = {
	{"unported",_("Unported")},
	{"ar",_("Argentina")},
	{"at",_("Austria")},
	{"au",_("Australia")},
	{"be",_("Belgium")},
	{"bg",_("Bulgaria")},
	{"br",_("Brazil")},
	{"ca",_("Canada")},
	{"ch",_("Switzerland")},
	{"cl",_("Chile")},
	{"cn",_("China")},
	{"co",_("Colombia")},
	{"de",_("Germany")},
	{"dk",_("Denmark")},
	{"es",_("Spain")},
	{"fi",_("Finland")},
	{"fr",_("France")},
	{"hr",_("Croatia")},
	{"hu",_("Hungary")},
	{"il",_("Israel")},
	{"in",_("India")},
	{"it",_("Italy")},
	{"jp",_("Japan")},
	{"kr",_("South Korea")},
	{"mk",_("Macedonia")},
	{"mt",_("Malta")},
	{"mx",_("Mexico")},
	{"my",_("Malaysia")},
	{"nl",_("Netherlands")},
	{"pe",_("Peru")},
	{"pl",_("Poland")},
	{"pt",_("Portugal")},
	{"scotland",_("UK Scotland")},
	{"se",_("Sweden")},
	{"si",_("Slovenia")},
	{"tw",_("Taiwan")},
	{"uk",_("UK Englands and Wales")},
	{"us",_("United States")},
	{"za",_("South Africa")},
	{0,0}
};

char *ll_jurisdiction_name(const ll_juris_t juris) {
	jurisdiction_map_t *entry = jurisdiction_map;
	while (entry->jurisdiction) {
		if (strcmp(juris,entry->jurisdiction) == 0) {
			return strdup(entry->name);
		}
		++entry;
	}

	return strdup(juris);
}

/**
 * returns the jurisdiction for the given license.
 */
ll_juris_t ll_get_jurisdiction(const ll_uri_t uri) {
	return _ll_get_first(ll_get_attribute(uri,"http://purl.org/dc/elements/1.1/coverage",false));
}

/**
 * returns the name of the license.
 */
char* ll_get_name(const ll_uri_t u) {
	return _ll_get_first(ll_get_attribute(u,"http://purl.org/dc/elements/1.1/title",true));
}

/**
 * returns the version of the license.
 */
ll_version_t ll_get_version(const ll_uri_t u) {
	char* version = _ll_get_first(ll_get_attribute(u,"http://purl.org/dc/elements/1.1/hasVersion",false));
	if (version) {
		int c;
		int divisions;
		int position;
		ll_version_t result;

		divisions = 1;
		for (c = 0; c < (int)strlen(version); ++c) {
			if (version[c] == '.') {
				++divisions;
			}
		}

		c = 0;
		position = 1;
		result = (ll_version_t) calloc(divisions+1,sizeof(int));
		result[0] = divisions;
		while(version!=NULL && version[c]!='\0' && position<3) {
			if (version[c]!='.') {
				result[position] *= 10;
				result[position] += atoi(&version[c]);
			} else {
				position++;
			}
			c++;
		}
		free(version);
		return result;
	}
	return NULL;
}

/**
 * returns a list of uris which the license prohibits
 */
char** ll_get_prohibits(const ll_uri_t u) {
	return ll_get_attribute(u,"http://creativecommons.org/ns#prohibits",false);
}

/**
 * returns a list of uris which the license permits
 */
char** ll_get_permits(const ll_uri_t u) {
	return ll_get_attribute(u,"http://creativecommons.org/ns#permits",false);
}

/**
 * returns a list of uris which the license requires
 */
char** ll_get_requires(const ll_uri_t u) {
	return ll_get_attribute(u,"http://creativecommons.org/ns#requires",false);
}

/**
 * converts a filename string to a uri
 */
ll_uri_t ll_filename_to_uri(const ll_filename_t f) {
	ll_uri_t result;
        int i;

        result = (ll_uri_t) malloc((strlen(f)-3+7)*sizeof(char));

	strcpy(result,"http://");
	strncpy(&result[7],f,(strlen(f)-4));
	result[strlen(f)-3+7-1]='\0';

	i = 0;
	while(result[i]!='\0') {
		if (result[i]=='_')
			result[i] = '/';
		++i;
	}
	return result;
}

/**
 * converts a uri string to a filename
 */
ll_filename_t ll_uri_to_filename(const ll_uri_t u) {
	char* tmp_u;
	char* result;
	int i;

	assert( strncmp(u,"http://",7) == 0 );

	tmp_u = strdup(&u[7]);
	result = (char*) malloc((strlen(LICENSE_DIR)+strlen(tmp_u)+4+1)*sizeof(char));
	result[0] = '\0';
	i =0 ;
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

/**
 * triple callback which stores the desired values in a struct.
 */
void _ll_triple_handler(void* user_data, const raptor_statement* triple) {
	ll_attribute_search_t* search_data = (ll_attribute_search_t*) user_data;
	if (strcmp(search_data->subject,(char*)triple->subject)==0) {
		/*
                 * Store value if wanted.
                 */
		if ((search_data->predicate==NULL || strcmp(search_data->predicate,(char*)triple->predicate)==0)  /* Any predicate or the given. */
		  && search_data->num_values<MAX_TRIPLES /*  We have space for more results. */
		  && (search_data->type == -1 || search_data->type == (int)triple->object_type) /* Any type or the given. */
		  && (search_data->locale==NULL || triple->object_literal_language==NULL || strcmp((char*)triple->object_literal_language,"x-default")==0 || strcmp((char*)triple->object_literal_language,search_data->locale)==0)) { /* locale stuff */
			search_data->values[search_data->num_values] = strdup((char*)triple->object);
			if (triple->object_literal_language!=NULL)
				search_data->locales[search_data->num_values++] = strdup((char*)triple->object_literal_language);
			else
				search_data->locales[search_data->num_values++] = strdup("x-default");
		}
	}
}

/**
 * converts a string to lowercase
 */
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
	char* new_string;
	char *codeset;
	char *territory;

	new_string = _ll_lowercase(string);

	codeset = strchr(new_string,'.');
	if (codeset) {
		*codeset = '\0';
	}

	territory = strchr(new_string,'_');
	if (territory) {
		if ( strncmp(new_string,territory+1,strlen(territory+1)) == 0 ) {
			*territory = '\0';
		} else {
			*territory = '-';
		}
	}
	return new_string;
}

/**
 * helper function which runs the parser
 */
ll_attribute_search_t* _ll_get_triple(const ll_uri_t u, const char* subject, const char* predicate, raptor_identifier_type type, int locale) {
	char *fn;

	/*
         * create the helper struct
         */
	ll_attribute_search_t* helper = (ll_attribute_search_t*) malloc(sizeof(ll_attribute_search_t));
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
	fn = ll_uri_to_filename(u);

	/*
         * BREAKS_WINDOWS
         */
	if (access(fn,R_OK)==0) {
          raptor_parser* rdf_parser;
	  unsigned char* fnu;
	  raptor_uri* fn_uri;

          /*
           * Run the parser.
           */
	  rdf_parser = raptor_new_parser("rdfxml");
	  if (rdf_parser==NULL) {
	  	fprintf(stderr,"New parser failed.\n");
	  	exit(1);
	  }
	  raptor_set_statement_handler(rdf_parser, helper, _ll_triple_handler);
	  fnu = raptor_uri_filename_to_uri_string(fn);
	  fn_uri = raptor_new_uri(fnu);
	  free(fnu);
	  raptor_parse_file(rdf_parser, fn_uri, fn_uri);
	  raptor_free_uri(fn_uri);
	  raptor_free_parser(rdf_parser);
  }
  free(fn);
  return helper;
}

/**
 * frees the search helper struct
 */
void _ll_free_attribute_search_t(ll_attribute_search_t* ast) {
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

/**
 * create the list to return from a attribute_search struct
 */
char** _ll_build_list(ll_attribute_search_t * ast, char** fsi) {
	int i;
	char** result;

	/*
         * check for generics
         */
	for (i=0;i<ast->num_values;++i) {
	  	if(strstr(ast->values[i],"genid")!=NULL) {
	  		*fsi = strdup(ast->values[i]);
	  		return NULL;
	  	}
	}

	/*
         * create the list to return
         */
	if (ast->locale==NULL) {
	  result = ll_new_list(ast->num_values);
	  i=0;
	  for (i=0;i<ast->num_values;++i) {
	  	result[i]=strdup(ast->values[i]);
	  }
	} else {
          int match;

          result = ll_new_list(1);
	  i=0;
	  match = false;
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

/**
 * calls the helper parser function to get nodes with a depth of 1 or 2
 */
char** ll_get_attribute(ll_uri_t u,ll_attribute_t a, int locale) {
  ll_attribute_search_t* helper;
  char* further_search;
  char** result;

  if ( strncmp(u,"http://",7) != 0 ) return NULL;

  helper = _ll_get_triple(u,u,a,-1,locale);

  further_search = NULL;

  result = _ll_build_list(helper, &further_search);
  /*
   * Free the helper struct
   */
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
