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

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#define BASENAME "write_test_temporary_file"
#define FILENAME BASENAME ".txt"
#define XMPNAME BASENAME ".xmp"

#define MP3FILE "write_test.mp3"

void make_text_file() {
	FILE* f = fopen(FILENAME, "w");
	fclose(f);
}

void clean_up() {
	unlink(FILENAME);
	unlink(XMPNAME);
}

void test_license_readwrite(ll_uri_t license, const char * filename) {
	ll_uri_t license_read_back;
	int result;

	result = ll_write(filename,LL_LICENSE, license);
	assert(result);
	
	license_read_back = ll_read(filename, LL_LICENSE);
	assert(result);
	printf("License read bacK: %s\n", license_read_back);
	
	assert (result > -1);
	assert (strcmp(license, license_read_back) == 0);

}

int main(int argc, char** argv) {
	ll_uri_t license = "http://creativecommons.org/licenses/by/2.5/au/";

        (void)argc;
        (void)argv;
	ll_init();

	/* Test MP3 */
	test_license_readwrite(license, MP3FILE);
	printf("One ok\n");

	/* Test MP3 again to make sure writing 
	 * a license isn't destructive to ll's internal structures */
	test_license_readwrite(license, MP3FILE);
	printf("Two ok\n");

	/* Create a "temporary" text file */
	clean_up();
	make_text_file();

	/* test_license_readwrite(license, FILENAME); */
	ll_stop();

	return 0;
}
