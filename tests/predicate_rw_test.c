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

static void set_then_get(const char * filename,
			 const char * predicate,
			 const char * expected_result) {
	int success = ll_write(filename, predicate, expected_result);
	assert(success);

	char * got_result = ll_read(filename, predicate);
	if (got_result == expected_result == NULL) {
		return; /* success */
	}
	/* otherwise, check the strings */
	assert (strcmp(got_result, expected_result) == 0);
}
	
int main() {
	const char * mp3 = "../tests/data_empty.mp3";
	const char * pdf = "../tests/data_empty.pdf";
	
	ll_init();

	char * tempfile = malloc(4096);
	strcpy(tempfile, "/tmp/liblicense_test.XXXXXX");
	assert(mkstemp(tempfile) > -1);

	return 0;
}
