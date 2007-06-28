#ifndef LIBLICENSE_SYSTEM_LICENSES_H
#define LIBLICENSE_SYSTEM_LICENSES_H

#define LICENSE_DIR "/home/scott/work/cc/svn/liblicense/trunk/licenses/"
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
