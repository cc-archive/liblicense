#include "../system_licenses.h"
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
	return 0;
}
