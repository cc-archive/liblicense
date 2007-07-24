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

#include <liblicense/liblicense.h>

#include <stdlib.h>
#include <stdio.h>

int main(int argc,char** argv) {
	ll_init();
	uri_t license = "http://creativecommons.org/licenses/by-nd/2.0/de/";
	juris_t j = ll_get_jurisdiction(license);
	printf("get_jurisdiction: '%s'\n",j);
	free(j);
	char* name = ll_get_name(license);
	printf("get_name: '%s'\n",name);
	free(name);
	
	int* version = ll_get_version(license);
	printf("get_version: %d.%d.%d\n",version[0],version[1],version[2]);
	free(version);
	
	uri_t* p = ll_get_prohibits(license);
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
	
	int b = ll_verify_uri(license);
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
