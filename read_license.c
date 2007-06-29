#include "read_license.h"
#include "module_wrangler.h"
#include <string.h>
#include <stdio.h>
#include "list.h"
uri_t ll_read(filename_t f) {
	module_t* modules = ll_get_io_modules();
	uri_t* results = ll_new_list(ll_list_length(modules));
	
	// Get uris from all applicable modules.
	int i = 0;
	while (modules[i]!=NULL) {
		mime_type_t* supported = ll_module_mime_types(modules[i]);
		if(ll_list_contains(supported,"future/mime") || ll_list_length(supported)==0)
			results[i] = ll_module_read(f,modules[i]);
		else
			results[i] = strdup("");
		ll_free_list(supported);
		i++;
	}
	
	// Decide on an answer based on all of the results.

	// Write the decision into the conflicting modules.
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
