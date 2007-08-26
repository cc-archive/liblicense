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

ll_juris_t ll_get_jurisdiction(const ll_uri_t);
char* ll_get_name(const ll_uri_t);

/** Returns an array of integers representing each division
  * The first element of the array specifies the number of version
  * divisions that follow, and the rest are the actual version numbers
  */
ll_version_t ll_get_version(const ll_uri_t);

char** ll_get_prohibits(const ll_uri_t);
char** ll_get_permits(const ll_uri_t);
char** ll_get_requires(const ll_uri_t);
char** ll_get_attribute(ll_uri_t, ll_attribute_t, int);
ll_uri_t ll_filename_to_uri(const ll_filename_t);
ll_filename_t ll_uri_to_filename(const ll_uri_t);

char* ll_jurisdiction_name(const ll_juris_t);
/*******************************************************/

/************** licenses_{cached,uncached} *************/
int ll_init(void);
int ll_stop(void);
int ll_verify_uri(const ll_uri_t);
ll_uri_t* ll_get_all_licenses(void);
ll_uri_t* ll_get_licenses(const ll_juris_t);
ll_juris_t* ll_get_jurisdictions(void);
/*******************************************************/

/******************* module_wrangler *******************/

typedef char* ll_module_t;
typedef char* ll_symbol_t;
typedef char* ll_mime_type_t;

ll_module_t* ll_get_config_modules(void);
ll_module_t* ll_get_io_modules(void);
int ll_module_init(char*, ll_module_t);
void* ll_get_module_symbol(char*, ll_module_t, ll_symbol_t);
int ll_module_shutdown(char*, ll_module_t);

void ll_print_module_info(void);

void ll_init_modules(void);
void ll_stop_modules(void);

int _ll_contains_token(char *string, const char *token);

typedef struct _LLModuleDesc LLModuleDesc;

extern LLModuleDesc **_module_list;

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

ll_uri_t ll_read(ll_filename_t);
ll_uri_t ll_module_read(ll_filename_t, ll_module_t);

/*******************************************************/

/******************** write_license ********************/

int ll_write(ll_filename_t, ll_uri_t);
int ll_module_write(ll_filename_t, ll_uri_t, ll_module_t);

/*******************************************************/

/******************* license_chooser *******************/

typedef struct _ll_license_chooser_t ll_license_chooser_t;
typedef struct ll_license_list_t {
	struct ll_license_list_t *next;
	const char *license;
} ll_license_list_t;

#define LL_UNSPECIFIED 0

ll_license_chooser_t* ll_new_license_chooser(const ll_juris_t, const char **attributes );
void ll_free_license_chooser(ll_license_chooser_t *);
const ll_license_list_t* ll_get_licenses_from_flags( ll_license_chooser_t *, int permits, int requires, int prohibits );
int ll_attribute_flag( ll_license_chooser_t *, const char *attr );

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
