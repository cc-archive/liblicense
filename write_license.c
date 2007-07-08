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

#include "write_license.h"
#include <stdbool.h>
#include "list.h"
#include "xdgmime/xdgmime.h"
#include <stdio.h>
int ll_write(filename_t f,uri_t u) {
	module_t* modules = ll_get_io_modules();
	int result = 0;
	
	const mime_type_t mt = xdg_mime_get_mime_type_for_file(f,NULL);
	printf("File mime-type: %s\n",mt);
	// Get uris from all applicable modules.
	int i = 0;
	while (modules[i]!=NULL) {
		mime_type_t* supported = ll_module_mime_types(modules[i]);
		if(ll_list_contains(supported,mt) || ll_list_length(supported)==0)
			result += ll_module_write(f,u,modules[i]);
		ll_free_list(supported);
		i++;
	}
	ll_free_list(modules);
	return result;
}
int ll_module_write(filename_t f,uri_t u,module_t m) {
	ll_module_init(LIBLICENSE_IO_MODULE_DIR, m);
	int (*function_write)(char*,char*);
	*(void**) (&function_write) = ll_get_module_symbol(LIBLICENSE_IO_MODULE_DIR, m, "write");
	if(!function_write)
		printf("bad news bears, symbol not found.  HYCSBWK\n");
	int result = (*function_write)(f,u);
	ll_module_shutdown(LIBLICENSE_IO_MODULE_DIR, m);
	return result;
}
