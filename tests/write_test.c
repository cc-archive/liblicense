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

#include <liblicense/liblicense.h>

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
        char* file;
        ll_uri_t license;
        int result;

        (void)argc;
        (void)argv;
	ll_init();

	file="license_me.txt";
	license = "creativecommons.org/licenses/by/2.5/au/";
	result = ll_write(file,license);
	printf("Write result: %d\n",result);

	ll_stop();

	return 0;
}
