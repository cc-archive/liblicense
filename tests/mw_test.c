// Creative Commons has made the contents of this file
// available under a CC-GNU-GPL license:
//
// http://creativecommons.org/licenses/GPL/2.0/
//
// A copy of the full license can be found as part of this
// distribution in the file COPYING.
// 
// You may use the liblicense software in accordance with the
// terms of that license. You agree that you are solely 
// responsible for your use of the liblicense software and you
// represent and warrant to Creative Commons that your use
// of the liblicense software will comply with the CC-GNU-GPL.
//
// Copyright 2007, Creative Commons, www.creativecommons.org.
// Copyright 2007, Scott Shawcroft.

#include "module_wrangler.h"
#include "list.h"
#include <stdio.h>
#include <string.h>
int main(int argc, char** argv) {
	//leak free 6/29/07
	printf("Getting the config modules: ");
	char** list = ll_get_config_modules();
	ll_list_print(list);
	ll_free_list(list);
	
	printf("Getting the io modules: ");
	list = ll_get_io_modules();
	ll_list_print(list);
	char* m = strdup(list[0]);
	ll_free_list(list);
	
	printf("Getting mime-types for '%s':",m);
	list = ll_module_mime_types(m);
	ll_list_print(list);
	ll_free_list(list);
	free(m);
	
	return 0;
}
