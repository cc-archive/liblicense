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

#include <string.h>
#include <stdio.h>
#include <assert.h>

int main(int argc, char** argv) {
	char * known_good_final_list[] = {"one", "two", "three", "four",
					  "four", "four", "one", NULL};
	char** list;

        (void)argc;
        (void)argv;
	list = ll_new_list(8);
	list[0] = strdup("one");
	list[1] = strdup("two");
	printf("Current list: ");
	ll_list_print(list);

	printf("Contains 'one': %d\n", ll_list_contains(list,"one"));
	assert (ll_list_contains(list,"one"));
	printf("Contains 'three': %d\n", ll_list_contains(list,"three"));
	assert (! ll_list_contains(list,"three"));

	list[2] = strdup("three");
	list[3] = strdup("four");
	list[4] = strdup("four");
	list[5] = strdup("four");
	list[6] = strdup("one");
	printf("Current list: ");
	ll_list_print(list);
	assert (ll_lists_equal(list, known_good_final_list));
	printf("Index of 'three': %d\n",ll_list_index(list,"three"));
	assert(ll_list_index(list,"three") == 2);
	printf("List length: %d\n", ll_list_length(list));
	assert(ll_list_length(list) == 7);
	
	printf("List mode: '%s'\n", ll_list_mode(list,"four"));
	assert(strcmp(ll_list_mode(list,"four"), "one") == 0);

	printf("List free: \n");
	ll_free_list(list);
	return 0;
}
