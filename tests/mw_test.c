#include "module_wrangler.h"
#include "list.h"
#include <stdio.h>
#include <string.h>
int main(int argc, char** argv) {
	//leak free 6/29/07
	printf("Getting the config modules: ");
	char** list = ll_get_config_modules();
	ll_list_print(list);
	ll_free_list(list);
	
	printf("Getting the io modules: ");
	list = ll_get_io_modules();
	ll_list_print(list);
	char* m = strdup(list[0]);
	ll_free_list(list);
	
	printf("Getting mime-types for '%s':",m);
	list = ll_module_mime_types(m);
	ll_list_print(list);
	ll_free_list(list);
	free(m);
	
	return 0;
}
