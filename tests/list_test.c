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

#include <string.h>
#include <stdio.h>

int main(int argc, char** argv) {
	char** list;

        (void)argc;
        (void)argv;
	list = ll_new_list(5);
	list[0] = strdup("one");
	list[1] = strdup("two");
	printf("Current list: ");
	ll_list_print(list);

	printf("Contains 'one': %d\n", ll_list_contains(list,"one"));
	printf("Contains 'three': %d\n", ll_list_contains(list,"three"));
	list[2] = strdup("three");
	list[3] = strdup("four");
	list[4] = strdup("four");
	printf("Current list: ");
	ll_list_print(list);
	printf("Index of 'three': %d\n",ll_list_index(list,"three"));
	printf("List length: %d\n", ll_list_length(list));
	printf("List mode: '%s'\n", ll_list_mode(list,"four"));

	printf("List free: \n");
	ll_free_list(list);
	return 0;
}
