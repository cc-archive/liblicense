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

#include <stdio.h>
#include <string.h>
int main(int argc, char** argv) {
	char** list;

        (void)argc;
        (void)argv;
	ll_init();

	/* leak free 6/29/07 */
	printf("Getting the config modules: ");
	list = ll_get_config_modules();
	ll_list_print(list);
	ll_free_list(list);

	printf("Getting the io modules: ");
	list = ll_get_io_modules();
	ll_list_print(list);
	ll_free_list(list);

	printf("Getting module info:\n");
	ll_print_module_info();

	ll_stop();

	return 0;
}
