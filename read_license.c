#include "read_license.h"
#include "module_wrangler.h"
#include <string.h>
#include <stdio.h>
uri_t ll_read(filename_t f) {
	return strdup("http://creativecommons.org/licenses/by/2.5/ar/");
}

uri_t ll_module_read(filename_t f,module_t m) {
	ll_module_init(LIBLICENSE_IO_MODULE_DIR, m);
	printf("Module initialized.\n");
	fflush(stdout);
	function_read read = (function_read) ll_get_module_symbol(LIBLICENSE_IO_MODULE_DIR, m, "read");
	printf("Function loaded.\n");
	fflush(stdout);
	uri_t result = read(f);
	printf("Loaded function called.\n");
	fflush(stdout);
	ll_module_shutdown(LIBLICENSE_IO_MODULE_DIR, m);
	printf("Module shutdown.\n");
	fflush(stdout);
	return result;
}
