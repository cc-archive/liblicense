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
#include <stdio.h>
#include <locale.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/**
 * initializes the library and its dependencies.
 */
int ll_init() {
	raptor_init();
	setlocale(LC_ALL,"");
	ll_init_modules();
	return 0;
}

/**
 * stops the library and its dependencies.
 */
int ll_stop() {
	raptor_finish();
	ll_stop_modules();
	return 0;
}

/**
 * helper which returns whether a file ands in .rdf
 */
int _ll_rdf_filter(const struct dirent * d) {
	return strstr(d->d_name,".rdf")!=NULL;
}

/**
 * returns a null-terminated list of all general licenses available.
 */
ll_uri_t* ll_get_all_licenses() {
  struct dirent **namelist;
  int n;
  ll_uri_t* result;
  int i;

  n = scandir(LICENSE_DIR, &namelist, _ll_rdf_filter, alphasort);
  if(n == -1) {
	  perror("ll_get_all_licenses");
	  return NULL;
  }
  result = (ll_uri_t*)calloc(n+1, sizeof(ll_uri_t));
  for (i=0;i<n;++i) {
    result[i] = ll_filename_to_uri(namelist[i]->d_name);
    free(namelist[i]);
  }
  free(namelist);
  return result;
}

/**
 * returns a null-terminated list of all general licenses in a family.
 */
ll_uri_t* ll_get_licenses(const ll_juris_t _j) {
	ll_juris_t j;
	ll_uri_t* licenses;
	int z;
	int pos;
	int keep;
	ll_uri_t* result;

	j = _j;
	if (j && strcmp(j,"unported") == 0) j = NULL;

	licenses = ll_get_all_licenses();
	if(licenses == NULL) {
		return NULL;
	}
	z = 0;
	keep = 0;
	while(licenses[z]!=NULL) {
		ll_juris_t tmp_j;
		ll_uri_t *successor;

		tmp_j = ll_get_jurisdiction(licenses[z]);
		successor = ll_get_attribute(licenses[z],
                                             LL_ATTRIBUTE_URI_REPLACED_BY,0);
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
	result = ll_new_list(keep);
	z=0;
	pos = 0;
	while(licenses[z]!=NULL && pos<keep) {
		if(strcmp(licenses[z],"remove")!=0)
			result[pos++] = strdup(licenses[z]);
		z++;
	}
	ll_free_list(licenses);
	return result;
}

ll_juris_t* ll_get_jurisdictions() {
	ll_uri_t* licenses;
	ll_juris_t* result;
	int count;
	ll_juris_t juris;
	int i;
        int len;

	licenses = ll_get_all_licenses();
	result = ll_new_list(50);
	count = 0;

	result[count++] = strdup("unported");

	len = ll_list_length(licenses);
	for (i=0; i<len; ++i) {
		juris = ll_get_jurisdiction(licenses[i]);
		if (!juris) continue;

		if (!ll_list_contains(result,juris)) {
			if (count < 50)
				result[count++] = juris;
			else {
				fprintf(stderr,"Hard-coded limit: not enough space for all jurisdictions\n");
				free(juris);
				break;
			}
		} else {
			free(juris);
		}
	}

	/*
         * Do an insertion sort.  We can get away with O(n^2) since we're guaranteed n<50
	 * This works well since it's done in-place.
	 * Keep the first element in the list alone; we want 'unported' first
	 */
	for (i = 2; i < count; ++i) {
                int j;

		for (j = i; ( j >= 2 ) && (strcmp(result[j],result[j-1]) < 0); --j ) {
			ll_juris_t tmp = result[j];
			result[j] = result[j-1];
			result[j-1] = tmp;
		}
	}

	return result;
}

/**
 * returns whether or not the given uri is recognized by the system.
 */
int ll_verify_uri(const ll_uri_t u) {
	ll_uri_t* licenses = ll_get_all_licenses();
	int result = ll_list_contains(licenses,u);
	ll_free_list(licenses);
	return result;
}
