#include "system_default.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include "list.h"
#include <stdio.h>

int ll_set_default(const uri_t u) {
	module_t* modules= ll_get_config_modules();
	int i = 0;
	int result = true;
	while(modules[i]!=NULL) {
		ll_module_init(LIBLICENSE_CONFIG_MODULE_DIR,modules[i]);
		int (*set_default) (uri_t);
		*(void**) (&set_default) = ll_get_module_symbol(LIBLICENSE_CONFIG_MODULE_DIR,modules[i],"set_default");
		result = (result && set_default(u));
		ll_module_shutdown(LIBLICENSE_CONFIG_MODULE_DIR,modules[i]);
		i++;
	}
	ll_free_list(modules);
	return result;
}

uri_t ll_get_default() {
	module_t* modules= ll_get_config_modules();
	uri_t* responses = ll_new_list(ll_list_length(modules));
	int i = 0;
	while(modules[i]!=NULL) {
		ll_module_init(LIBLICENSE_CONFIG_MODULE_DIR,modules[i]);
		uri_t (*get_default) ();
		*(void**) (&get_default) = ll_get_module_symbol(LIBLICENSE_CONFIG_MODULE_DIR,modules[i],"get_default");
		responses[i] = get_default();
		ll_module_shutdown(LIBLICENSE_CONFIG_MODULE_DIR,modules[i]);
		i++;
	}
	uri_t final_answer = ll_list_mode(responses,"");
	ll_free_list(modules);
	if (final_answer==NULL) {
		fprintf(stderr,"No system default set.\n");
		ll_free_list(responses);
		return NULL;
	}
	final_answer = strdup(final_answer);
	ll_free_list(responses);
	return final_answer;
}
