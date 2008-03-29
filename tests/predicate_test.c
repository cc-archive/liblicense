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
 * Copyright 2007-2008, Creative Commons, www.creativecommons.org.
 * Copyright 2007, Scott Shawcroft.
 * Copyright (C) 2007 Peter Miller
 */

#include "liblicense.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

static void search(const char * filename,
		   const char * predicate,
		   const char * expected_module_name) {
	LLModuleDesc * module = NULL;
	LLModuleSearchState state;
	memset(&state, 0, sizeof(LLModuleSearchState));
	/* First, try to store a license URI in
	   an MP3 file, which should work fine. */
	module = ll_module_search(filename, predicate, &state);
	if (expected_module_name != NULL) {
		assert(module != NULL);
		assert(strcmp(module->name, expected_module_name) == 0);
	} else {
		assert (module == NULL);
	}
}

int main() {
	const char * mp3 = "../tests/data_empty.mp3";
	const char * pdf = "../tests/data_empty.pdf";
	
	ll_init();

	/* First, try to store a license URI in
	   an MP3 file, which should work fine. */
	search(mp3, LL_LICENSE, "id3.so");
	
	/* Then assert that a web statement can
	   also go in an MP3 file. */
	search(mp3, LL_WEBSTATEMENT, "id3.so");
	
	/* Then assert that a cc:morePermissions can't 
	   go in an MP3 file */
	search(mp3, LL_MORE_PERMISSIONS, NULL);

	/* Then assert that cc:morePermissions can fit in a PDF
	   with exempi. */
	search(pdf, LL_MORE_PERMISSIONS, "exempi.so");
	
	ll_stop();
	
	return 0;
}
