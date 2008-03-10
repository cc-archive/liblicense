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

#include "xdgmime.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

ll_uri_t ll_read(ll_filename_t infile) {
	return ll_module_read(infile, NULL);
}

ll_uri_t ll_module_read(ll_filename_t infile, ll_module_t requested_module_name) {
	char * license = NULL;
	int result_index = 0;
	LLModuleSearchState state;
	LLModuleDesc * module;
	char * one_result;
	char ** all_results;

	all_results = malloc(sizeof(char) * _ll_modules_count_available());
	memset(all_results, 0, sizeof(char) * _ll_modules_count_available());
	memset(&state, 0, sizeof(LLModuleSearchState));

	module = ll_module_search(infile, &state);
			
	while (module && module->read) {
		/* Either if no module is specified,
		   or if this module is the one specified: */

		if ( (requested_module_name == NULL) ||
		     (strcmp(module->name, requested_module_name) == 0)) {

			/* Do the read! */
			one_result = (module->read)(infile);
			if (one_result) {
				all_results[result_index] = one_result;
				result_index++;
			}
		}
		/* Whether or not the module matched, keep searching */
		module = ll_module_search(infile, &state);

	}
		
	license = ll_list_mode(all_results,"");
	if (license) {
		license = strdup(license);
	}
	
	ll_free_list(all_results);
	
	return license;
}
