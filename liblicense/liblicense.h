/* Creative Commons has made the contents of this file
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

#ifndef LIBLICENSE_H
#define LIBLICENSE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WIN32
#define LL_DIR_SEPARATOR '/'
#define LL_DIR_SEPARATOR_S "/"
#else
#define LL_DIR_SEPARATOR '\\'
#define LL_DIR_SEPARATOR_S "\\"
#endif

/******************* license_info *******************/
typedef char* ll_juris_t;
typedef char* ll_uri_t;
typedef char* ll_locale_t;
typedef int* ll_version_t;
typedef char* ll_attribute_t;
typedef char* ll_filename_t;

struct ll_attribute_search_t {
	ll_uri_t uri;
	char* subject;
	char* predicate;
	char* locale;
	int type;
	char** values;
	char** locales;
	int num_values;
};
typedef struct ll_attribute_search_t ll_attribute_search_t;

/**
 * The ll_get_jurisdiction function is used to obtain the jurisdiction
 * code for the given license (the "coverage" attribute).
 *
 * @param license_uri
 *     The URI of the license of interest
 * @returns
 *     a string containing the code of the jusrisdiction,
 *     or NULL on failure
 */
ll_juris_t ll_get_jurisdiction(const ll_uri_t license_uri);

/**
 * The ll_get_name function is used to obtain the name of a license.
 *
 * @param license_uri
 *     The URI of the license of interest
 * @returns
 *     the name of the license (use free() when done with),
 *     or NULL on failure
 */
char *ll_get_name(const ll_uri_t license_uri);

/**
 * The ll_get_version functions is used to obtain the version of the
 * license of interest.
 *
 * @param license_uri
 *     The URI of the license of interest
 * @returns
 *     an array of integers representing each division.  The first
 *     element of the array specifies the number of version divisions
 *     that follow, and the rest are the actual version numbers.
 *     Use free() when you are done with it.
 */
ll_version_t ll_get_version(const ll_uri_t license_uri);

char** ll_get_prohibits(const ll_uri_t);
char** ll_get_permits(const ll_uri_t);
char** ll_get_requires(const ll_uri_t);
char** ll_get_attribute(ll_uri_t, ll_attribute_t, int);
ll_uri_t ll_filename_to_uri(const ll_filename_t);
ll_filename_t ll_uri_to_filename(const ll_uri_t);

/**
 * The ll_jurisdiction_name functions is used to map a jusrisdiction
 * code (two letter country code) to a jusrisdiction name (country
 * name).
 *
 * @param jurisdiction
 *     The justisdiction code of interest
 * @returns
 *     a string containing the jurisdiction name (countery name),
 *     or NULL on error.
 */
char *ll_jurisdiction_name(const ll_juris_t jurisdiction);

/*******************************************************/

/************** licenses_{cached,uncached} *************/
int ll_init(void);
int ll_stop(void);

/**
 * The ll_verify_uri function is used to determine whether or not the
 * given uri is recognized by the system.
 *
 * @param license_uri
 *     The URI of the license of interest
 * @returns
 *     1 if recognized,
 *     0 if not
 */
int ll_verify_uri(const ll_uri_t license_uri);

/**
 * The ll_get_all_licenses function is used to obtain a list of all
 * general license URIs available for new works.
 *
 * @returns
 *     a list of strings.
 *     A list is a null terminated array.
 *     Use ll_free_list to free it.
 */
ll_uri_t* ll_get_all_licenses(void);

/**
 * The ll_get_licenses function is used to obtain a list of all general
 * licenses in a family.
 *
 * @param justrisdiction
 *     A string naming the jurisdiction code of interest.
 * @returns
 *     a list of strings containing URIs of licenses
 *     A list is a null terminated array.
 *     Use #ll_free_list to free it.
 */
ll_uri_t* ll_get_licenses(const ll_juris_t justrisdiction);

/**
 * The ll_get_jurisdictions function is used to obtain a list of all
 * jurisdiction codes.
 *
 * @returns
 *     a list of strings containing available juristriction codes (as
 *     opposed to all possible jurisdiction codes)
 *     A list is a null terminated array.
 *     Use #ll_free_list to free it.
 */
ll_juris_t* ll_get_jurisdictions(void);

/*******************************************************/

/******************* module_wrangler *******************/

typedef char* ll_module_t;
typedef char* ll_symbol_t;
typedef char* ll_mime_type_t;

/**
 * The ll_get_config_modules function may be used to obtain a list of
 * available configuration modules.
 *
 * @returns
 *     a list of config module filenames (strings), terminated with a NULL pointer.
 *     Use #ll_free_list when you are done with it.
 */
ll_module_t* ll_get_config_modules(void);

/**
 * The ll_get_io_modules function may be used to obtain a list of
 * available I/O modules.
 *
 * @returns
 *     a list of I/O module filenames (strings), terminated with a NULL
 *     pointer.  Use #ll_free_list when you are done with it.
 * @note
 *     The #ll_init_modules function shall be called before this function.
 */
ll_module_t* ll_get_io_modules(void);

/**
 * The ll_module_init function may be used to load a module and call
 * its "init" function with no arguments.  If the module does not have an
 * "init" symbol, nothing is called, but the module is still dlopen()ed.
 * No checks are made that the "init" symbol is a functions (and not,
 * say, a variable) and no checks are made that the function takes no
 * arguments.
 *
 * Each successfull call <i>must</i> be balanced by a #ll_module_shutdown
 * call when you are done with the module.
 *
 * @param directory
 *     The directory containing the modules.
 * @param module
 *     The name of the module of interest
 * @returns
 *     0 if the module can't be opened with dlopen,
 *     1 if it can.
 */
int ll_module_init(const char *directory, ll_module_t module);

/**
 * The ll_get_module_symbol function may be used to obtain a pointer to
 * a symbol in a module.  Usually this will be a function, but it could
 * be data.
 *
 * @param directory
 *     The directory containing the modules.
 * @param module
 *     The name of the module of interest
 * @param symbol
 *     The name of the symbol of interest
 * @returns
 *     the NULL pointer if the symbol does not exist, or a pointer to
 *     the symbol in dynamic memory.
 * @note
 *     This function will exit with a fatal error if it is not possble
 *     to dlopen the module.
 */
void* ll_get_module_symbol(const char *directory, ll_module_t module,
    ll_symbol_t symbol);

/**
 * The ll_module_shutdown function may be used to call the shutdown
 * symbol (if it exists) of the module and then unloads the module.  No
 * checks are made that the "shutdown" symbol is a function (and not,
 * say, a variable) and no checks are made that the function takes no
 * arguments.
 *
 * @param directory
 *     The directory containing the modules.
 * @param module
 *     The name of the module of interest
 * @returns
 *     -1 on error (the module cannot be dlopen()ed),
 *     0 on success
 */
int ll_module_shutdown(const char *directory, ll_module_t module);

/**
 * The ll_print_module_info function is used to print a human readable
 * summary of all available I/O modules on the standard output.
 *
 * @note
 *     The #ll_init_modules function shall be called before this function.
 */
void ll_print_module_info(void);

/**
 * The ll_init_modules function is used to initialize the I/O module
 * system before any of the module handling functions are called.
 *
 * It is safe to call this function more than once, but this does <b>not</b>
 * increase a reference count, so the first call to #ll_stop_modules will
 * stop all modules.
 *
 * @note
 *     If an error occurs reading the modules directory, an error will
 *     be reported on stderr, and this function will return without
 *     doing anything.
 */
void ll_init_modules(void);

/**
 * The ll_stop_modules function is used to finalize the I/O module
 * system after the last use of the module system has completed.
 *
 * It is mostly safe to call this function more than once.  See the note
 * on #ll_init_modules for more information.
 */
void ll_stop_modules(void);

/**
 * The _ll_contains_token function is used to look for a given token in
 * a string.
 *
 * @param haystack
 *     The string to be searched
 * @param needle
 *     The string to be searched for
 * @returns
 *     1 if the needle is found in the haystack,
 *     0 if not found.
 */
int _ll_contains_token(const char *haystack, const char *needle);

typedef struct _LLModuleDesc LLModuleDesc;

extern LLModuleDesc **_ll_module_list;

typedef void (*LLModuleInitFunc) (void);
typedef char* (*LLModuleReadFunc) (const char*);
typedef int (*LLModuleWriteFunc) (const char*,const char*);
typedef void (*LLModuleShutdownFunc) (void);

enum ll_features {
	LL_FEATURES_NONE = 		0x000000,
	LL_FEATURES_EMBED = 	0x000001
};
typedef enum ll_features ll_features;

struct _LLModuleDesc {
	char *name;
	char *description;
	char *version;
	int features;
	char *mime_types;
	LLModuleInitFunc module_init;
	LLModuleReadFunc read;
	LLModuleWriteFunc write;
	void *handle;
};

#ifdef _MSC_VER
#define LL_MODULE_EXPORT __declspec(dllexport) extern
#else
#define LL_MODULE_EXPORT
#endif

#define LL_MODULE_DEFINE(name,description,version,features,mime_types,init,read,write)	\
LL_MODULE_EXPORT LLModuleDesc ll_module_desc = {	\
  name,							\
  description,						\
  version,						\
  features,							\
  mime_types,							\
  init,							\
  read,							\
  write,							\
  0											\
};

/*******************************************************/

/******************** system_default *******************/

int ll_set_default(const ll_uri_t);
ll_uri_t ll_get_default(void);

/*******************************************************/

/********************* read_license ********************/

/**
 * The ll_read function may be used to obtain the URI of the license
 * of the given file.  All available modules will be tried against the
 * file.
 *
 * @param filename
 *     The name of the file for which the license is desired.
 * @returns
 *     a string containing the URI of the license corresponding to the file.
 *     (sort of: it actually returns the license with the highest count)
 * @note
 *     The #ll_init_modules function shall be called before this function.
 */
ll_uri_t ll_read(ll_filename_t filename);

/**
 * The ll_module_read function may be used to obtain the URI of the
 * license of the given file, as determined by the named module.
 *
 * @param filename
 *     The name of the file for which the license is desired.
 * @param module
 *     The name of the module to scan the file with
 * @returns
 *     a string containing the URI of the license corresponding to the file.
 * @note
 *     The #ll_init_modules function shall be called before this function.
 */
ll_uri_t ll_module_read(ll_filename_t filename, ll_module_t module);

/*******************************************************/

/******************** write_license ********************/

/**
 * The ll_write function is used to write the given license to the given
 * file using all applicable modules.
 *
 * It first attempts to embed the license into the file, it only writes
 * to external locations if it cannot embed the license using any
 * module.
 *
 * @param filename
 *     The name of the file the license is about
 * @param license_uri
 *     The URI of the license of the file
 * @returns
 *     -1 if no embedders are available for this file type;
 *     0 if all available embedders failed;
 *     1 on success
 * @note
 *     The #ll_init_modules function shall be called before this function.
 */
int ll_write(ll_filename_t filename, ll_uri_t license_uri);

/**
 * Writes the given license to the given file using the given module.
 *
 * @param filename
 *     The name of the file the license is about
 * @param license_uri
 *     The URI of the license of the file
 * @param module
 *     The name of the module to use to write the license to the file.
 * @returns
 *     -1 if no embedders are available for this file type;
 *     0 if all available embedders failed;
 *     1 on success
 * @note
 *     The #ll_init_modules function shall be called before this function.
 * @note
 *     The #ll_init_modules function shall be called before this function.
 */
int ll_module_write(ll_filename_t filename, ll_uri_t license_uri,
    ll_module_t module);

/*******************************************************/

/******************* license_chooser *******************/

typedef struct _ll_license_chooser_t ll_license_chooser_t;
typedef struct ll_license_list_t {
	struct ll_license_list_t *next;
	const char *license;
} ll_license_list_t;

#define LL_UNSPECIFIED 0

/**
 * The ll_new_license_chooser function is used to find a suitable
 * license given a list of license attributes.
 *
 * @param jurisdiction
 *     The two letter country code of the jurisdiction of interest
 * @param attributes
 *     a (null terminated?) list of attribute names
 * @returns
 *     an opaque pointer to a result.
 *     Free with #ll_free_license_chooser when you are done with it.
 */
ll_license_chooser_t* ll_new_license_chooser(const ll_juris_t jurisdiction,
    const char **attributes);

void ll_free_license_chooser(ll_license_chooser_t *);

/**
 * The ll_get_licenses_from_flags function is used to extract a
 * linked-list of matching licenses from the result returned by a call
 * to #ll_new_license_chooser. The flags given should be OR'ed together
 * and come from the result of ll_attribute_flag()
 *
 * @param choo
 *     The results from an earlier #ll_new_license_chooser call.
 * @param permits
 *     bit mask
 * @param requires
 *     bit mask
 * @param prohibits
 *     bit mask
 * @returns
 *     list of URIs of compatible licenses
 */
const ll_license_list_t* ll_get_licenses_from_flags(ll_license_chooser_t *choo,
    int permits, int requires, int prohibits);

/**
 * The ll_attribute_flag function is used to obtain the bit map bit
 * corresponding to the named attribute in the ll_free_license_chooser
 * result.  This is used to query the result for licenses.
 *
 * @param choo
 *     The results from an earlier #ll_new_license_chooser call.
 * @param attr
 *     The name of the attribute
 * @returns
 *     integer with single bit set
 */
int ll_attribute_flag(ll_license_chooser_t *choo, const char *attr);

/*******************************************************/

/************************* list ************************/

char** ll_new_list(int);
void ll_free_list(char**);
int ll_list_contains(char**, char*);
int ll_list_index(char**, char*);
int ll_list_length(char**);
char* ll_list_mode(char**, char*);
void ll_list_print(char**);

/*******************************************************/

#ifdef __cplusplus
}
#endif

#endif /* LIBLICENSE_H */
