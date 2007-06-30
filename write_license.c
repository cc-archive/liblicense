#include "write_license.h"
#include <stdbool.h>
#include "list.h"
#include "xdgmime/xdgmime.h"
#include <stdio.h>
int ll_write(filename_t f,uri_t u) {
	module_t* modules = ll_get_io_modules();
	int result = 0;
	
	mime_type_t mt = xdg_mime_get_mime_type_from_file_name(f);
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
