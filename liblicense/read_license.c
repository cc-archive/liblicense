// Creative Commons has made the contents of this file
// available under a CC-GNU-LGPL license:
//
// http://creativecommons.org/licenses/LGPL/2.1/
//
// A copy of the full license can be found as part of this
// distribution in the file COPYING.
// 
// You may use the liblicense software in accordance with the
// terms of that license. You agree that you are solely 
// responsible for your use of the liblicense software and you
// represent and warrant to Creative Commons that your use
// of the liblicense software will comply with the CC-GNU-LGPL.
//
// Copyright 2007, Creative Commons, www.creativecommons.org.
// Copyright 2007, Scott Shawcroft.

#include "liblicense.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "xdgmime/xdgmime.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

uri_t ll_read(filename_t f) {
	assert(_module_list);

	int i, length = 0;
	while (_module_list[length]) {length++;}

	uri_t* results = ll_new_list(length);

	const mime_type_t mt = xdg_mime_get_mime_type_for_file(f,NULL);
	printf("File mime-type: %s\n",mt);

	i = 0;
	LLModuleDesc **curr_module = _module_list;
	while (*curr_module) {
		if ( !(*curr_module)->mime_types || _ll_contains_token((*curr_module)->mime_types,mt) ) {
			if ((*curr_module)->read) {
				char *result = ((*curr_module)->read)(f);
				if (result) {
					results[i++] = result;
				}
			}
		}
		++curr_module;
	}

	char *license = ll_list_mode(results,"");
	if (license) {
		license = strdup(license);
	}

	ll_free_list(results);

	return license;
}

uri_t ll_module_read(filename_t f,module_t m) {
	assert(_module_list);

	char *license = NULL;

	LLModuleDesc **curr_module = _module_list;
	while (*curr_module) {
		if ( strcmp((*curr_module)->name,m) == 0 ) {
			if ((*curr_module)->write) {
				license = ((*curr_module)->read)(f);
			}
		}
		++curr_module;
	}

	return license;
}
