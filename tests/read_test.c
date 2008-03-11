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

void do_test() {
	char *file="data/licensed.mp3";
	ll_uri_t license;
	license = ll_read(file, LL_LICENSE);
	printf("File license: %s\n",license);
	assert (license != NULL);
	free(license);
}

int main(int argc, char** argv) {

        (void)argc;
        (void)argv;
	ll_init();

	do_test(); /* Separated out into a function
		      so one can verify that calling
		      it twice or thrice, it still works. */
	do_test();
	ll_stop();
	return 0;
}
