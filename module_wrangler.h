#ifndef LIBLICENSE_MODULE_WRANGLER_H
#define LIBLICENSE_MODULE_WRANGLER_H

	#define LIBLICENSE_CONFIG_MODULE_DIR "../modules/config/.libs/"
	#define LIBLICENSE_IO_MODULE_DIR "../modules/io/"
	typedef char* module_t;
	typedef char* symbol_t;
	typedef char* mime_type_t;

	module_t* ll_get_config_modules();
	module_t* ll_get_io_modules();
	int ll_module_init(char*,module_t);
	void* ll_get_module_symbol(char*,module_t, symbol_t);
	int ll_module_shutdown(char*,module_t);
	
	// IO module functions.
	mime_type_t* ll_module_mime_types(module_t);
	

#endif
