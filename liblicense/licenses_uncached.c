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
#include <stdio.h>
#include <locale.h>
#include <ctype.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// initializes the library and its dependencies.
int ll_init() {
	raptor_init();
	setlocale(LC_ALL,"");
	ll_init_modules();
	return 0;
}

// stops the library and its dependencies.
int ll_stop() {
	raptor_finish();
	ll_stop_modules();
	return 0;
}

// helper which returns whether a file ands in .rdf
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
    result[i] = ll_filename_to_uri(namelist[i]->d_name);
    free(namelist[i]);
  }
  free(namelist);
	return result;
}

// returns a null-terminated list of all general licenses in a family.
uri_t* ll_get_licenses(const juris_t j) {
	uri_t* licenses = ll_get_all_licenses();
	int z=0;
	int keep=0;
	while(licenses[z]!=NULL) {
		juris_t tmp_j = ll_get_jurisdiction(licenses[z]);
		uri_t *successor = ll_get_attribute(licenses[z],"http://purl.org/dc/elements/1.1/isReplacedBy",0);
		if(((!tmp_j && !j) || (tmp_j && j && strcmp(tmp_j,j)==0)) && successor[0]==NULL)
			keep++;
		else {
			free(licenses[z]);
			licenses[z] = strdup("remove");
		}
		ll_free_list(successor);
		free(tmp_j);
		z++;
	}
	uri_t* result = ll_new_list(keep);
	z=0;
	int pos = 0;
	while(licenses[z]!=NULL && pos<keep) {
		if(strcmp(licenses[z],"remove")!=0)
			result[pos++] = strdup(licenses[z]);
		z++;
	}
	ll_free_list(licenses);
	return result;
}

// returns whether or not the given uri is recognized by the system.
int ll_verify_uri(const uri_t u) {
	uri_t* licenses = ll_get_all_licenses();
	int result = ll_list_contains(licenses,u);
	ll_free_list(licenses);
	return result;
}
