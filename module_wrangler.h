#include "system_licenses.h"
#ifndef LIBLICENSE_MODULE_WRANGLER_H
#define LIBLICENSE_MODULE_WRANGLER_H

	#define LIBLICENSE_CONFIG_MODULE_DIR
	#define LIBLICENSE_IO_MODULE_DIR
	typedef char* module_t;
	typedef char* symbol_t;
	typedef char* filename_t;
	typedef char* mime_type_t;

	module_t* ll_get_config_modules();
	module_t* ll_get_io_modules();
	void* ll_get_module_symbol(module_t, symbol_t);
	
	// Config Module functions.
	int ll_module_in_use(module_t);
	
	// IO module functions.
	mime_type_t* ll_io_module_mime_types(module_t);
	

#endif
