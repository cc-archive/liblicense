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
#include <stdio.h>

int main(int argc,char** argv) {
	ll_uri_t license;
	ll_juris_t j;
	char* name;
	int* version;
	ll_uri_t* p;
	int b;

        (void)argc;
        (void)argv;
	ll_init();
	license = "http://creativecommons.org/licenses/by-nd/2.0/de/";
    j = ll_get_first(ll_get_attribute(license, LL_ATTRIBUTE_URI_JURISDICTION, false));
	printf("get_jurisdiction: '%s'\n",j);
	free(j);
	name = ll_get_name(license);
	printf("get_name: '%s'\n",name);
	free(name);

	version = ll_get_version(license);
	printf("get_version: ");
	if (version) {
		int i;
		for (i=1; i<=version[0]; ++i) {
			if (i!=1) printf(".");
			printf("%d",version[i]);
		}
		printf("\n");
		free(version);
	} else {
		printf("unversioned\n");
	}

	p = ll_get_prohibits(license);
	printf("get_prohibits: ");
	ll_list_print(p);
	ll_free_list(p);

	p = ll_get_permits(license);
	printf("get_permits: ");
	ll_list_print(p);
	ll_free_list(p);

	p = ll_get_requires(license);
	printf("get_requires: ");
	ll_list_print(p);
	ll_free_list(p);

	b = ll_verify_uri(license);
	printf("verify_uri: %d\n",b);

	p = ll_get_all_licenses();
	printf("get_all_licenses: ");
	ll_list_print(p);
	ll_free_list(p);

	p = ll_get_licenses("us");
	printf("get_licenses: ");
	ll_list_print(p);
	ll_free_list(p);

	ll_stop();
	return 0;
}
