#include "module_wrangler.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
module_t* ll_get_config_modules() {
	module_t* result = (module_t*) malloc(3*sizeof(module_t));
	result[0] = strdup("gconf.o");
	result[1] = strdup("xdg.o");
	result[2] = NULL;
	return result;
}
module_t* ll_get_io_modules() {
	module_t* result = (module_t*) malloc(3*sizeof(module_t));
	result[0] = strdup("nautilus.o");
	result[1] = strdup("txt.o");
	result[2] = NULL;
	return result;
}
void* ll_get_module_symbol(module_t m, symbol_t s) {
	return NULL;
}

// Config Module functions.
int ll_module_in_use(module_t m) {
	return true;
}

// IO module functions.
mime_type_t* ll_module_mime_types(module_t m) {
	mime_type_t* result = (mime_type_t*) malloc(3*sizeof(mime_type_t));
	result[0] = strdup("text/plain");
	result[1] = strdup("text/css");
	result[2] = NULL;
	return result;
}
