#include "system_licenses.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//returns the jurisdiction for the given license.
juris_t ll_get_jurisdiction(const uri_t uri) {
	return strdup("au");
}

// returns the available locales for the given license.
locale_t* ll_get_locales(const uri_t u) {
	locale_t* result = (locale_t*) malloc(2*sizeof(locale_t));
	result[0] = strdup("en_US");
	result[1] = NULL;
	return result;
}

// returns the name of the license.
char* ll_get_name(const uri_t u) {
	return strdup("Attribution");
}

// returns the version of the license.
version_t ll_get_version(const uri_t u) {
	version_t result = (version_t) malloc(3*sizeof(int));
	result[0] = 3;
	result[1] = 0;
	result[2] = 0;
	return result;
}

// returns the notification string for the given url.
char* ll_get_notification(const uri_t u) {
	return strdup("This work is licensed under the...");
}

// returns the notification string for the given uri and verification url.
char* ll_get_verifiable_notification(const uri_t u, const char* v) {
	return strdup("This work... verify at blah.");
}

// returns whether or not the given uri is recognized by the system.
int ll_verify_uri(const uri_t u) {
	return false;
}

char** ll_get_attribute(uri_t u,attribute_t a) {
	char** result = (char**) malloc(2*sizeof(char*));
	result[0] = strdup("ping");
	result[1] = NULL;
	return result;
}

// returns a null-terminated list of all general licenses available.
uri_t* ll_get_all_licenses() {
	uri_t* result = (uri_t*) malloc(4*sizeof(uri_t));
	result[0] = strdup("http://creativecommons.org/licenses/by/2.5/ar/");
	result[1] = strdup("http://creativecommons.org/licenses/by-sa/2.5/ar/");
	result[2] = strdup("http://creativecommons.org/licenses/by-nd/2.0/");
	result[3] = NULL;
	return result;
}

// returns a null-terminated list of all general licenses in a family.
uri_t* ll_get_licenses(const juris_t j) {
	uri_t* result = (uri_t*) malloc(3*sizeof(uri_t));
	result[0] = strdup("http://creativecommons.org/licenses/by/2.5/ar/");
	result[1] = strdup("http://creativecommons.org/licenses/by-sa/2.5/ar/");
	result[2] = NULL;
	return result;
}

