/*
 * Creative Commons has made the contents of this file
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

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xdgmime/xdgmime.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

int ll_write(ll_filename_t f, ll_uri_t u) {
	int result = -1;
	const char *mt;
	int embedded = 0;
	LLModuleDesc **curr_module;

	assert(_ll_module_list);
	mt = xdg_mime_get_mime_type_for_file(f,NULL);
	curr_module = _ll_module_list;
	while (*curr_module) {
		if ( !(*curr_module)->mime_types || _ll_contains_token((*curr_module)->mime_types,mt) ) {
			if ((*curr_module)->features & LL_FEATURES_EMBED) {
				result = (*curr_module)->write(f,u);
				if (result) {
					embedded = 1;
				}
			}
		}
		++curr_module;
	}

	if (!embedded) {
		curr_module = _ll_module_list;
		while (*curr_module) {
			if ( !(*curr_module)->mime_types || _ll_contains_token((*curr_module)->mime_types,mt) ) {
				if ((*curr_module)->write) {
					result = (*curr_module)->write(f,u);
				}
			}
			++curr_module;
		}
	}

	return result;
}

int ll_module_write(ll_filename_t f, ll_uri_t u, ll_module_t m) {
	int result = -1;
	LLModuleDesc **curr_module;

	assert(_ll_module_list);
	curr_module = _ll_module_list;
	while (*curr_module) {
		if ( strcmp((*curr_module)->name,m) == 0 ) {
			if ((*curr_module)->write) {
				result = (*curr_module)->write(f,u);
			}
		}
		++curr_module;
	}

	return result;
}
