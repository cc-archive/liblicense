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
	attribute_t key;
	char** values;
	int num_values;
	char* last_value;
} attribute_search_t;

int ll_init();
int ll_stop();

juris_t ll_get_jurisdiction(const uri_t);
locale_t* ll_get_locales(const uri_t);
char* ll_get_name(const uri_t);
version_t ll_get_version(const uri_t);
char* ll_get_notification(const uri_t);
char* ll_get_verifiable_notification(const uri_t, const char*);
int ll_verify_uri(const uri_t);
char** ll_get_attribute(uri_t,attribute_t);

uri_t* ll_get_all_licenses();
uri_t* ll_get_licenses(const juris_t);

#endif
