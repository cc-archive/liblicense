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

#include "liblicense.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "xdgmime/xdgmime.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

ll_uri_t ll_read(ll_filename_t f) {
	int i, length = 0;
	ll_uri_t* results;
        const char *mt;
	LLModuleDesc **curr_module;
        char *license;

	assert(_ll_module_list);
	while (_ll_module_list[length]) {length++;}

	results = ll_new_list(length);

	mt = xdg_mime_get_mime_type_for_file(f,NULL);

	i = 0;
	curr_module = _ll_module_list;
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

	license = ll_list_mode(results,"");
	if (license) {
		license = strdup(license);
	}

	ll_free_list(results);

	return license;
}

ll_uri_t ll_module_read(ll_filename_t f, ll_module_t m) {
        char *license;
	LLModuleDesc **curr_module;

	assert(_ll_module_list);
	license = NULL;
	curr_module = _ll_module_list;
	while (*curr_module) {
		if ( strcmp((*curr_module)->name,m) == 0 ) {
			if ((*curr_module)->write) {
				license = ((*curr_module)->read)(f);
				break;
			}
		}
		++curr_module;
	}

	return license;
}
