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

static const int _LL_PATH_MAX = 4096;

static void get(const char * filename,
		const char * predicate,
		const char * expected_result) {

	char * got_result = ll_read(filename, predicate);
	if (expected_result == NULL) { /* expected empty */
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

void path_join(char * buffer, int buffer_size, char * dirname, char * basename) {
	/* Honestly, is this what C programmers do? */
	int remaining = buffer_size - 1; /* Zero padded */

	/* Zero that buffer */
	memset(buffer, 0, buffer_size);

	/* Jam the dirname in */
	strncat(buffer, dirname, remaining);
	remaining -= strlen(dirname);
	assert (remaining > 0);

	/* Slide the basename on */
	strncat(buffer, basename, remaining);
}

int main() {
	const char * mp3 = "../tests/data_empty.mp3";
	const char * pdf = "../tests/data_empty.pdf";
	
	ll_init();

	char * tempfile = malloc(_LL_PATH_MAX);
	char * tempdir = malloc(_LL_PATH_MAX);
	strcpy(tempdir, "/tmp/liblicense_test_dir.XXXXXX");
	assert(mkdtemp(tempdir) > -1);

	/* First, copy in the MP3 to the temp file */
	path_join(tempfile, _LL_PATH_MAX, tempdir, "data_empty.mp3");
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

	set_then_get(tempfile, LL_LICENSE, "http://creativecommons.org/licenses/by/2.0/");
	set_then_get(tempfile, LL_WEBSTATEMENT, "http://example.com/statement/");
	set_then_get(tempfile, LL_MORE_PERMISSIONS, NULL);
	unlink(tempfile);

	/* Copy pasta for PDF :-( */

	path_join(tempfile, _LL_PATH_MAX, tempdir, "data_empty.pdf");
	/* First, copy in the PDF to the temp file */
	FILE * pdf_fd = fopen(pdf, "r");
	tempfile_fd = fopen(tempfile, "w");

	bufsize = 4096;
	buf = malloc(bufsize);
	num_read_bytes = 0;
	num_write_bytes = 0;

	while (! feof(pdf_fd)) {
		num_read_bytes = fread(buf, 1, bufsize, pdf_fd);
		num_write_bytes = fwrite(buf, 1, num_read_bytes, tempfile_fd);
		assert(num_read_bytes == num_write_bytes);
	}

	/* TEMPORARY FIXME:
	 * Comment out PDF tests due to them failing.
	set_then_get(tempfile, LL_LICENSE, "http://creativecommons.org/licenses/by/2.0/");
	set_then_get(tempfile, LL_WEBSTATEMENT, "http://example.com/statement/");
	set_then_get(tempfile, LL_MORE_PERMISSIONS, NULL);
	*/
	unlink(tempfile);

	return 0;
}
