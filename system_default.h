#include "system_licenses.h"
#ifndef LIBLICENSE_SYSTEM_DEFAULT_H
#define LIBLICENSE_SYSTEM_DEFAULT_H
	int ll_set_default(const uri_t);
	const char* ll_get_default();
	char** ll_default_engines();
#endif
