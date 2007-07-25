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

/******************* system_licenses *******************/
typedef char* juris_t;
typedef char* uri_t;
typedef char* ll_locale_t;
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

/*******************************************************/

/******************* module_wrangler *******************/

typedef char* module_t;
typedef char* symbol_t;
typedef char* mime_type_t;

module_t* ll_get_config_modules();
module_t* ll_get_io_modules();
int ll_module_init(char*,module_t);
void* ll_get_module_symbol(char*,module_t, symbol_t);
int ll_module_shutdown(char*,module_t);

void ll_print_module_info();

void ll_init_modules();
void ll_stop_modules();

int _ll_contains_token(char *string, const char *token);

typedef struct _LLModuleDesc LLModuleDesc;

extern LLModuleDesc **_module_list;

typedef void (*LLModuleInitFunc) (void);
typedef char* (*LLModuleReadFunc) (const char*);
typedef int (*LLModuleWriteFunc) (const char*,const char*);
typedef void (*LLModuleShutdownFunc) (void);

typedef enum {
	LL_FEATURES_NONE = 		0x000000,
	LL_FEATURES_EMBED = 	0x000001
} features;

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

/* FIXME: Windows */
#define LL_MODULE_EXPORT

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

int ll_set_default(const uri_t);
uri_t ll_get_default();

/*******************************************************/

/********************* read_license ********************/

uri_t ll_read(filename_t);
uri_t ll_module_read(filename_t,module_t);

/*******************************************************/

/******************** write_license ********************/

int ll_write(filename_t,uri_t);
int ll_module_write(filename_t,uri_t,module_t);

/*******************************************************/

/************************* list ************************/

char** ll_new_list(int);
void ll_free_list(char**);
int ll_list_contains(char**,char*);
int ll_list_index(char**,char*);
int ll_list_length(char**);
char* ll_list_mode(char**,char*);
void ll_list_print(char**);

/*******************************************************/

#ifdef __cplusplus
}
#endif

#endif /* LIBLICENSE_H */