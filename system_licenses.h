// Creative Commons has made the contents of this file
// available under a CC-GNU-GPL license:
//
// http://creativecommons.org/licenses/GPL/2.0/
//
// A copy of the full license can be found as part of this
// distribution in the file COPYING.
// 
// You may use the liblicense software in accordance with the
// terms of that license. You agree that you are solely 
// responsible for your use of the liblicense software and you
// represent and warrant to Creative Commons that your use
// of the liblicense software will comply with the CC-GNU-GPL.
//
// Copyright 2007, Creative Commons, www.creativecommons.org.
// Copyright 2007, Scott Shawcroft.

#ifndef LIBLICENSE_SYSTEM_LICENSES_H
#define LIBLICENSE_SYSTEM_LICENSES_H

#define LICENSE_DIR "../licenses/"
typedef char* juris_t;
typedef char* uri_t;
typedef char* locale_t;
typedef int* version_t;
typedef char* attribute_t;
typedef char* filename_t;

typedef struct {
	uri_t uri;
	char* subject;
	char* predicate;
	char* locale;
	int type;
	char** values;
	char** locales;
	int num_values;
} attribute_search_t;

int ll_init();
int ll_stop();

juris_t ll_get_jurisdiction(const uri_t);
char* ll_get_name(const uri_t);
version_t ll_get_version(const uri_t);
char** ll_get_prohibits(const uri_t);
char** ll_get_permits(const uri_t);
char** ll_get_requires(const uri_t);
int ll_verify_uri(const uri_t);
char** ll_get_attribute(uri_t,attribute_t,int);

uri_t* ll_get_all_licenses();
uri_t* ll_get_licenses(const juris_t);

#endif
