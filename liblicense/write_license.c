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

#include "xdgmime.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

int ll_write(ll_filename_t outfile, ll_uri_t value) {
	return ll_module_write(outfile, value, NULL);
}

static int _ll_module_write(ll_filename_t outfile, 
			    ll_uri_t value,
			    ll_module_t requested_module_name,
			    int embed_or_not) {
	LLModuleSearchState state;
	LLModuleDesc * module;
	int last_result;
	int all_results_fused = -1; /* -1 means failure */
	int this_one_supports_embed;

	memset(&state, 0, sizeof(LLModuleSearchState));

	module = ll_module_search(outfile, &state);

	while (module && module->write) {
		/* Either if no module is specified,
		   or if this module is the one specified: */

		if ( (requested_module_name == NULL) ||
		     (strcmp(module->name, requested_module_name) == 0)) {
			/* If the module matches our preferences about
			   embedding */
			this_one_supports_embed = module->features & 
				LL_FEATURES_EMBED;
			if (this_one_supports_embed == embed_or_not) {
				/* Do the write! */
				last_result = (module->write)(outfile, value);
				if ((last_result > -1) &&
				    (all_results_fused < 0) ) {
					all_results_fused = last_result;
				}
			}
		}
		/* Whether or not the module matched, repeat the search */
		module = ll_module_search(outfile, &state);
	}
	
	return all_results_fused;
}

/**
 * This function goes through the module list and writes the license
 * with every module that works, returning the FIXME
 */
int ll_module_write(ll_filename_t outfile, 
			 ll_uri_t value,
			 ll_module_t requested_module_name) {
	int result = 0;
	int managed_to_embed = 0;
	result = managed_to_embed = _ll_module_write(outfile, value, requested_module_name, 1);
	if (managed_to_embed < 1) {
		result = _ll_module_write(outfile, value, requested_module_name, 0);
	}
	return result;
}
