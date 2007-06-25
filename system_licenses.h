#ifndef STRING_SET_H
#define STRING_SET_H
typedef char* juris_t;
typedef char* uri_t;
typedef char* locale_t;
typedef char* short_t;
typedef int bitcode_t;
typedef char* family_t;
typedef int* version_t;
typedef int flag_t;
typedef struct {
	flag_t flag;
	char* name;
	char* desc;
} FlagInfo ;

typedef struct {
	char* name;
	char* desc;
	short_t short_name;
} LicenseInfo;

juris_t get_jurisdiction(const uri_t);
juris_t* get_short_jurisdictions(const short_t);
juris_t* get_bitcode_jurisdictions(const bitcode_t);
locale_t get_locale(const uri_t);
locale_t* get_short_locales(const short_t, const juris_t);
locale_t* get_bitcode_locales(const bitcode_t, const juris_t);
char* get_name(const uri_t);
version_t get_version(const uri_t);
version_t* get_short_versions(const short_t, const juris_t);
version_t* get_bitcode_versions(const bitcode_t, const juris_t);
short_t get_short(const uri_t);
int has_flag(const flag_t, const uri_t);
FlagInfo* family_flags(const family_t);
family_t family(const uri_t);
char* get_notification(const uri_t);
char* get_verifiable_notification(const uri_t, const char*);
int verify_uri(const uri_t);
uri_t get_bitcode_license(const family_t,const bitcode_t, const juris_t,const locale_t);
uri_t get_short_license(const family_t,const short_t, const juris_t, const locale_t);
LicenseInfo* get_all_licenses();
LicenseInfo* get_general_licenses(const family_t);
family_t* get_families();
#endif
