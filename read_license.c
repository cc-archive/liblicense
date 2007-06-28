#include "read_license.h"
#include "module_wrangler.h"
#include <string.h>
#include <stdio.h>
uri_t ll_read(filename_t f) {
	return strdup("http://creativecommons.org/licenses/by/2.5/ar/");
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
