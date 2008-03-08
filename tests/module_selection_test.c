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
#include <string.h>

int main() {
	 /* Assert that an MP3 file uses the MP3 module */
	 
	 char* file;
	 LLModuleDesc * module;
	 LLModuleSearchState state;
	 
	 file = "data/empty.flac";
	 memset(&state, 0, sizeof(LLModuleSearchState));

	 ll_init();
	 
	 module = ll_module_search(file, &state);
	 while(module) {
		 printf("%s\n", module->name);
		 module = ll_module_search(file, &state);
	 }
	 
	 ll_stop();
	 
	 return 0;
}
