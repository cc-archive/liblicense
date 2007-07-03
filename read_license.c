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

#include "read_license.h"
#include "module_wrangler.h"
#include <string.h>
#include <stdio.h>
#include "list.h"
#include "xdgmime/xdgmime.h"
uri_t ll_read(filename_t f) {
	module_t* modules = ll_get_io_modules();
	uri_t* results = ll_new_list(ll_list_length(modules));
	
	mime_type_t mt = xdg_mime_get_mime_type_from_file_name(f);
	printf("File mime-type: %s\n",mt);
	// Get uris from all applicable modules.
	int i = 0;
	while (modules[i]!=NULL) {
		mime_type_t* supported = ll_module_mime_types(modules[i]);
		if(ll_list_contains(supported,mt) || ll_list_length(supported)==0)
			results[i] = ll_module_read(f,modules[i]);
		else
			results[i] = strdup("");
		ll_free_list(supported);
		i++;
	}
	free(mt);
	uri_t license = strdup(ll_list_mode(results,""));
	ll_free_list(results);
	ll_free_list(modules);
	return license;
}

uri_t ll_module_read(filename_t f,module_t m) {
	ll_module_init(LIBLICENSE_IO_MODULE_DIR, m);
	char* (*function_read)(char*);
	*(void**) (&function_read) = ll_get_module_symbol(LIBLICENSE_IO_MODULE_DIR, m, "read");
	if(!function_read)
		printf("bad news bears, symbol not found.  HYCSBWK\n");
	uri_t result = (*function_read)(f);
	ll_module_shutdown(LIBLICENSE_IO_MODULE_DIR, m);
	return result;
}
