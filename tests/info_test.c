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

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void test_get_attribute_jurisdiction() {
	char * license;
	char * j;
	license = "http://creativecommons.org/licenses/by-nd/2.0/de/";
	j = ll_get_first(ll_get_attribute(license, LL_JURISDICTION, false));
	printf("get_jurisdiction: '%s'\n",j);
	assert (strcmp(j, "http://creativecommons.org/international/de/") == 0);
	free(j);
}

void test_get_attribute_jurisdiction_localized() {
	char * license;
	char * j;
	license = "http://creativecommons.org/licenses/by-nd/2.0/de/";
	j = ll_get_first(ll_get_attribute(license, LL_JURISDICTION, true));
	printf("get_jurisdiction: '%s'\n",j);
	assert (strcmp(j, "http://creativecommons.org/international/de/") == 0);
	free(j);
}

void test_get_attribute_name_whatever_lang() {
	const char * license;
	char * name;
	license = "http://creativecommons.org/licenses/by-nd/2.0/de/";
	name = ll_get_first(ll_get_attribute(license, LL_NAME, false));
	printf("get_name: '%s'\n",name);
	assert (strcmp(name, "Recoñecemento-SenObraDerivada") == 0); /* UTF-8 */
	free(name);
}

void test_get_attribute_name_system_lang() {
	const char * license;
	char * name;
	license = "http://creativecommons.org/licenses/by-nd/2.0/de/";
	name = ll_get_first(ll_get_attribute(license, LL_NAME, true));
	printf("get_name: '%s'\n",name);
	assert (strcmp(name, "Attribution-NoDerivs") == 0); /* UTF-8 */
	free(name);
}

void test_get_version() {
	int known_good_version[] = {2,2,0};
	const char * license;
	int* version;

	license = "http://creativecommons.org/licenses/by-nd/2.0/de/";
	
	version = ll_parse_version(ll_get_first (ll_get_attribute (license, LL_VERSION, false)));
	printf("get_version: ");
	assert (ll_int_arrays_equal(version, known_good_version));
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
}


int main(int argc,char** argv) {
	char * known_good_prohibits[] = {NULL};
	char * known_good_permits[] = {LL_DISTRIBUTION, LL_REPRODUCTION, NULL};
	char * known_good_requires[] = {LL_NOTICE, LL_ATTRIBUTION, NULL};
	char * known_good_bync_permits[] = {
	  LL_DISTRIBUTION, LL_REPRODUCTION, LL_DERIVATIVE_WORKS, NULL};
	
	ll_uri_t license;
	ll_uri_t other_license;
	ll_juris_t j;
	char* name;
	ll_uri_t* p;
	int b;

        (void)argc;
        (void)argv;
	ll_init();

	license = "http://creativecommons.org/licenses/by-nd/2.0/de/";

	test_get_attribute_jurisdiction();
	test_get_attribute_jurisdiction_localized();
	test_get_attribute_name_system_lang();
	test_get_attribute_name_whatever_lang();
	test_get_version();

	p = ll_get_attribute(license, LL_PROHIBITS, false);
	printf("get_prohibits: ");
	assert (ll_lists_equal(p, known_good_prohibits));
	ll_list_print(p);
	ll_free_list(p);

	p = ll_get_attribute(license, LL_PERMITS, false);
	printf("get_permits: ");
	assert (ll_lists_equal(p, known_good_permits));
	ll_list_print(p);
	ll_free_list(p);

	p = ll_get_attribute(license, LL_REQUIRES, false);
	printf("get_requires: ");
	assert (ll_lists_equal(p, known_good_requires));
	ll_list_print(p);
	ll_free_list(p);

	/* Adding a test for http://code.creativecommons.org/issues/issue78 */
	other_license = "http://creativecommons.org/licenses/by-nc/3.0/";
	p = ll_get_attribute(other_license, LL_PERMITS, false);
	assert (ll_lists_equal(p, known_good_bync_permits));

	b = ll_verify_uri(license);
	printf("verify_uri: %d\n",b);
	assert (b == 1);

	p = ll_get_all_licenses();
	printf("get_all_licenses: ");
	ll_list_print(p);
	assert (ll_list_contains(p, license));
	ll_free_list(p);

	p = ll_get_licenses("http://creativecommons.org/international/de/");
	printf("get_licenses: ");
	ll_list_print(p);
	assert (ll_list_contains(p, license));
	ll_free_list(p);

	ll_stop();
	return 0;
}
