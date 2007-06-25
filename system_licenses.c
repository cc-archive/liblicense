#include "liblicense.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//returns the jurisdiction for the given license.
juris_t get_jurisdiction(const uri_t uri) {
	return strdup("au");
}

// returns a null-terminated list of the available jurisdiction for the given short name
juris_t* get_short_jurisdictions(const short_t s) {
	juris_t* result = (juris_t*) malloc(2*sizeof(juris_t));
	result[0] = strdup("au");
	result[1] = NULL;
	return result;
}

// returns a null-terminated list of the available jurisdiction for the given bitcode.
juris_t* get_bitcode_jurisdictions(const bitcode_t b) {
	juris_t* result = (juris_t*) malloc(2*sizeof(juris_t));
	result[0] = strdup("au");
	result[1] = NULL;
	return result;
}

// returns the locale for the given license.
locale_t get_locale(const uri_t u) {
	return strdup("en_US");
}

// returns a null-terminated list of the available locales for the given jurisdiction and short name.
locale_t* get_short_locales(const short_t s, const juris_t j) {
	locale_t* result = (locale_t*) malloc(2*sizeof(locale_t));
	result[0] = strdup("en");
	result[1] = NULL;
	return result;
}

// returns a null-terminated list of the available locales for the given jurisdiction and bitcode.
locale_t* get_bitcode_locales(const bitcode_t b, const juris_t j) {
	locale_t* result = (locale_t*) malloc(2*sizeof(locale_t));
	result[0] = strdup("en");
	result[1] = NULL;
	return result;
}

// returns the name of the license.
char* get_name(const uri_t u) {
	return strdup("Attribution");
}

// returns the version of the license.
version_t get_version(const uri_t u) {
	version_t result = (version_t) malloc(3*sizeof(int));
	result[0] = 3;
	result[1] = 0;
	result[2] = 0;
	return result;
}

// returns a null-terminated list of the available versions for the given short name and jurisdiction.
version_t* get_short_versions(const short_t s, const juris_t j) {
	version_t* result = (version_t*) malloc(3*sizeof(version_t));
	result[0] = (version_t) malloc(3*sizeof(int));
	result[0][0] = 2;
	result[0][1] = 5;
	result[0][2] = 0;
	result[1] = (version_t) malloc(3*sizeof(int));
	result[1][0] = 3;
	result[1][1] = 0;
	result[1][2] = 0;
	result[2] = NULL;
	return result;
}

// returns a null-terminated list of the available versions for the given bitcode and jurisdiction.
version_t* get_bitcode_versions(const bitcode_t b, const juris_t j) {
	version_t* result = (version_t*) malloc(3*sizeof(version_t));
	result[0] = (version_t) malloc(3*sizeof(int));
	result[0][0] = 2;
	result[0][1] = 5;
	result[0][2] = 0;
	result[1] = (version_t) malloc(3*sizeof(int));
	result[1][0] = 3;
	result[1][1] = 0;
	result[1][2] = 0;
	result[2] = NULL;
	return result;
}

// returns the short name for the given uri.
short_t get_short(const uri_t u) {
	return strdup("attribution");
}

// returns if the flag is set for the given uri.
int has_flag(const flag_t f, const uri_t u) {
	return true;
}

// returns a null-terminated list of the flags available for a given family.
FlagInfo* family_flags(const family_t f) {
	return NULL;
}

// returns the family the given uri belongs to.
family_t family(const uri_t u) {
	return strdup("Creative Commons");
}

// returns the notification string for the given url.
char* get_notification(const uri_t u) {
	return strdup("This work is licensed under the...");
}

// returns the notification string for the given uri and verification url.
char* get_verifiable_notification(const uri_t u, const char* v) {
	return strdup("This work... verify at blah.");
}

// returns whether or not the given uri is recognized by the system.
int verify_uri(const uri_t u) {
	return false;
}

// returns the uri which satisfies the given attributes.
uri_t get_short_license(const family_t f, const short_t s, const juris_t j, const locale_t l) {
	return strdup("creativecommons.org/licenses/by/2.5/au/");
}

// returns the uri which satisfies the given attributes.
uri_t get_bitcode_license(const family_t f, const bitcode_t b, const juris_t j, const locale_t l) {
	return strdup("creativecommons.org/licenses/by/2.5/au/");
}

// returns a null-terminated list of all general licenses available.
LicenseInfo* get_all_licenses() {
	return NULL;
}

// returns a null-terminated list of all general licenses in a family.
LicenseInfo* get_general_licenses(const family_t f) {
	return NULL;
}

// returns a null-terminated list of available families.
family_t* get_families() {
	family_t* result = (family_t*) malloc(3*sizeof(family_t));
	result[0] = strdup("Creative Commons");
	result[1] = strdup("Creative Commons Sampling");
	result[2] = NULL;
	return result;
}
