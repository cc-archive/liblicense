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

int main() {

	char * file;
	ll_uri_t value;
	int result;

	ll_init();

	/* First, try to store a license URI in
	   an MP3 file, which should work fine. */
	

	/* Then assert that a web statement can
	   also go in an MP3 file. */

	/* Then assert that a cc:morePermissions can't 
	   go in an MP3 file */

	/* Then assert that cc:morePermissions can fit in a PDF
	   with exempi. */

	assert (0); /* This test fails because it is not yet written */

	return 0;
}
