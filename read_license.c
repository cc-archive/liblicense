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

#include "liblicense.h"
#include <string.h>
#include <stdio.h>

#include "xdgmime/xdgmime.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

uri_t ll_read(filename_t f) {
	module_t* modules = ll_get_io_modules();
	uri_t* results = ll_new_list(ll_list_length(modules));
	
	const mime_type_t mt = xdg_mime_get_mime_type_for_file(f,NULL);
	printf("File mime-type: %s\n",mt);
	// Get uris from all applicable modules.
	int i = 0;
	while (modules[i]!=NULL) {
		mime_type_t* supported = ll_module_mime_types(modules[i]);
		if(ll_list_contains(supported,mt) || ll_list_length(supported)==0) {
			results[i] = ll_module_read(f,modules[i]);
			if (!results[i]) results[i] = strdup("");
		}
		else
			results[i] = strdup("");

		ll_free_list(supported);
		i++;
	}

	uri_t license = ll_list_mode(results,"");
	if (license)
		license = strdup(license);

	ll_free_list(results);
	ll_free_list(modules);
	return license;
}

uri_t ll_module_read(filename_t f,module_t m) {
	ll_module_init(LIBLICENSE_IO_MODULE_DIR, m);
	char* (*function_read)(char*);
	*(void**) (&function_read) = ll_get_module_symbol(LIBLICENSE_IO_MODULE_DIR, m, "read");
	if(!function_read)
		printf("Error: symbol 'read' not found in '%s'.\n",m);
	uri_t result = (*function_read)(f);
	ll_module_shutdown(LIBLICENSE_IO_MODULE_DIR, m);
	return result;
}
