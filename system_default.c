#include "system_default.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
int ll_set_default(const uri_t u) {
	return true;
}

uri_t ll_get_default() {
	return strdup("http://creativecommons.org/licenses/by-nd/2.0/");
}

module_t* ll_default_engines() {
	module_t* result = (module_t*) malloc(2*sizeof(module_t));
	result[0] = strdup("gconf.o");
	result[1] = NULL;
	return result;
}
