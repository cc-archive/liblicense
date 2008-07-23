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
#include <unistd.h>

static void get(const char * filename,
		const char * predicate,
		const char * expected_result) {

	char * got_result = ll_read(filename, predicate);
	if (expected_result == NULL) { // expected empty
		assert(got_result == NULL);
		/* success */
	}
	else {
		assert (got_result != NULL);
		/* otherwise, check the strings */
		assert (strcmp(got_result, expected_result) == 0);
		/* success */
	}
}
	
static void set_then_get(const char * filename,
			 const char * predicate,
			 const char * expected_result) {
	int success = ll_write(filename, predicate, expected_result);
	assert(success);

	get(filename, predicate, expected_result);
}

int main() {
	const char * mp3 = "../tests/data_empty.mp3";
	const char * pdf = "../tests/data_empty.pdf";
	
	ll_init();

	char * tempfile = malloc(4096);
	strcpy(tempfile, "/tmp/liblicense_test.XXXXXX");
	assert(mkstemp(tempfile) > -1);

	/* First, copy in the MP3 to the temp file */
	FILE * mp3_fd = fopen(mp3, "r");
	FILE * tempfile_fd = fopen(tempfile, "w");

	int bufsize = 4096;
	char * buf = malloc(bufsize);
	int num_read_bytes = 0;
	int num_write_bytes = 0;

	while (! feof(mp3_fd)) {
		num_read_bytes = fread(buf, 1, bufsize, mp3_fd);
		num_write_bytes = fwrite(buf, 1, num_read_bytes, tempfile_fd);
		assert(num_read_bytes == num_write_bytes);
	}

	/* Then rename it so that it ends in .mp3 */
	fclose(tempfile_fd);
	rename(tempfile, "/tmp/wtf.mp3"); /* FIXME this sucks */
	strcpy(tempfile, "/tmp/wtf.mp3");


	set_then_get(tempfile, LL_LICENSE, "http://creativecommons.org/licenses/by/2.0/");
	set_then_get(tempfile, LL_WEBSTATEMENT, "http://example.com/statement/");
	unlink(tempfile);

	return 0;
}
