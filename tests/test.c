#include "../system_licenses.h"
#include "../write_license.h"
#include "../read_license.h"
#include "../system_default.h"
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
int main(int argc, char** argv) {
  printf("Starting ll:\n");
  ll_init();
	printf("Test system_licenses:\n");
	char* result = ll_get_jurisdiction("http://creativecommons.org/licenses/by-nd/2.0/");
	printf("\tget_jurisdiction: %s\n",result);
	free(result);
	result = ll_get_name("http://creativecommons.org/licenses/by-nd/2.0/");
	printf("\tget_name: %s\n", result);
	free(result);
	printf("\tget_version: ");
	version_t v = ll_get_version("http://creativecommons.org/licenses/by-nd/2.0/");
	printf("%d.%d.%d\n",v[0],v[1],v[2]);
	free(v);
	printf("\tverify_uri: %d\n", ll_verify_uri("creativecommons.org/licenses/by/2.5/au/"));
	printf("\tget_attribute:\n");
	char** result1 = ll_get_attribute("http://creativecommons.org/licenses/by-nd/2.0/za/","http://purl.org/dc/elements/1.1/title",1);
	int i =0;
	while(result1[i]!=NULL)
		printf("\t\t%s\n",result1[i++]);
	printf("\tget_all_licenses:\n");
	result1 = ll_get_all_licenses();
	i =0;
	while(result1[i]!=NULL)
		printf("\t\t%s\n",result1[i++]);
	printf("\tget_licenses_in_juris: skipped\n");
	
	//printf("Test write_license:\n");
	//printf("\twrite: %d\n",ll_write("license_me.txt","creativecommons.org/licenses/by/2.5/au/"));
	printf("\tmodule_write: %d\n",ll_module_write("license_me.txt","creativecommons.org/licenses/by/2.5/au/",".libs/sidecar_xmp.so"));
	
	//printf("Test read_license:\n");
	//printf("\tread: %s\n",ll_read("license_me.txt"));
	printf("\tmodule_read: %s\n",ll_module_read("license_me.txt",".libs/sidecar_xmp.so"));
	
	//printf("Test system_default:\n");
	//printf("\tset_default: %d\n",ll_set_default("creativecommons.org/licenses/by/2.5/au/"));
	//printf("\tget_default: %s\n",ll_get_default());
	//printf("\tdefault_engines: skipped\n");

	printf("Test module_wrangler:");
	printf("\tget_config_modules: skipped\n");
	printf("\tget_io_modules: skipped\n");
	printf("\tget_module_symbol: skipped\n");
	//printf("\tmodule_mime_types:\n");
	ll_module_mime_types(".libs/stub.so");
	printf("Stopping ll:\n");
	ll_stop();
	return 0;
}
