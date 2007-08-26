// Creative Commons has made the contents of this file
// available under a CC-GNU-LGPL license:
//
// http://creativecommons.org/licenses/LGPL/2.1/
//
// A copy of the full license can be found as part of this
// distribution in the file COPYING.
// 
// You may use the liblicense software in accordance with the
// terms of that license. You agree that you are solely 
// responsible for your use of the liblicense software and you
// represent and warrant to Creative Commons that your use
// of the liblicense software will comply with the CC-GNU-LGPL.
//
// Copyright 2007, Creative Commons, www.creativecommons.org.
// Copyright 2007, Scott Shawcroft.

#include "liblicense.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

int ll_set_default(const ll_uri_t u) {
	ll_module_t* modules= ll_get_config_modules();
	int i = 0;
	int result = true;
	while(modules[i]!=NULL) {
		ll_module_init(LIBLICENSE_CONFIG_MODULE_DIR,modules[i]);
		int (*set_default) (ll_uri_t) = ll_get_module_symbol(LIBLICENSE_CONFIG_MODULE_DIR,modules[i],"set_default");
		result = (result && set_default(u));
		ll_module_shutdown(LIBLICENSE_CONFIG_MODULE_DIR,modules[i]);
		i++;
	}
	ll_free_list(modules);
	return result;
}

ll_uri_t ll_get_default() {
	ll_module_t* modules= ll_get_config_modules();
	ll_uri_t* responses = ll_new_list(ll_list_length(modules));
	int i = 0;
	while(modules[i]!=NULL) {
		ll_module_init(LIBLICENSE_CONFIG_MODULE_DIR,modules[i]);
		ll_uri_t (*get_default) (void) = ll_get_module_symbol(LIBLICENSE_CONFIG_MODULE_DIR,modules[i],"get_default");
		responses[i] = get_default();
		ll_module_shutdown(LIBLICENSE_CONFIG_MODULE_DIR,modules[i]);
		i++;
	}
	ll_uri_t final_answer = ll_list_mode(responses,"");
	ll_free_list(modules);
	if (final_answer==NULL) {
		ll_free_list(responses);
		return NULL;
	}
	final_answer = strdup(final_answer);
	ll_free_list(responses);
	return final_answer;
}
