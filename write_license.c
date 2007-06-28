#include "write_license.h"
#include <stdbool.h>
int ll_write(filename_t f,uri_t u) {
	return true;
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
