#include "../system_licenses.h"
#include "../write_license.h"
#include "../read_license.h"
#include "../system_default.h"
#include <stdio.h>

int main(int argc, char** argv) {
	printf("Test system_licenses:\n");
	printf("\tget_jurisdiction: %s\n",ll_get_jurisdiction("creativecommons.org/licenses/by/2.5/au/"));
	printf("\tget_locales: skipped\n");
	printf("\tget_name: %s\n", ll_get_name("creativecommons.org/licenses/by/2.5/au/"));
	printf("\tget_version: ");
	version_t v = ll_get_version("creativecommons.org/licenses/by/2.5/au/");
	printf("%d.%d.%d\n",v[0],v[1],v[2]);
	printf("\tget_notification: %s\n", ll_get_notification("creativecommons.org/licenses/by/2.5/au/"));
	printf("\tget_verifiable_notification: %s\n",ll_get_verifiable_notification("creativecommons.org/licenses/by/2.5/au/", "http://www.mysite.com/bleh"));
	printf("\tverify_uri: %d\n", ll_verify_uri("creativecommons.org/licenses/by/2.5/au/"));
	printf("\tget_all_licenses: skipped\n");
	printf("\tget_licenses_in_juris: skipped\n");
	
	printf("Test write_license:\n");
	printf("\twrite: %d\n",ll_write("license_me.txt","creativecommons.org/licenses/by/2.5/au/"));
	printf("\tmodule_write: %d\n",ll_module_write("license_me.txt","creativecommons.org/licenses/by/2.5/au/","txt.h"));
	
	printf("Test read_license:\n");
	printf("\tread: %s\n",ll_read("license_me.txt"));
	printf("\tmodule_read: %s\n",ll_module_read("license_me.txt","txt.h"));
	
	printf("Test system_default:\n");
	printf("\tset_default: %d\n",ll_set_default("creativecommons.org/licenses/by/2.5/au/"));
	printf("\tget_default: %s\n",ll_get_default());
	printf("\tdefault_engines: skipped\n");

	printf("Test module_wrangler:");
	printf("\tget_config_modules: skipped\n");
	printf("\tget_io_modules: skipped\n");
	printf("\tget_module_symbol: skipped\n");
	printf("\tmodule_in_use: %d\n",ll_module_in_use("gedit.h"));
	printf("\tmodule_mime_types: skipped\n");
	return 0;
}
