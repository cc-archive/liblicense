#include "../liblicense.h"
#include <stdio.h>

int main(int argc, char** argv) {
	printf("Test liblicense:\n");
	printf("\tget_jurisdiction: %s\n",get_jurisdiction("creativecommons.org/licenses/by/2.5/au/"));
	printf("\tget_short_jurisdictions: skipped\n");
	printf("\tget_bitcode_jurisdictions: skipped\n");
	printf("\tget_locale: %s\n",get_locale("creativecommons.org/licenses/by/2.5/au/"));
	printf("\tget_short_locales: skipped\n");
	printf("\tget_bitcode_locales: skipped\n");
	printf("\tget_name: %s\n", get_name("creativecommons.org/licenses/by/2.5/au/"));
	printf("\tget_version: ");
	version_t v = get_version("creativecommons.org/licenses/by/2.5/au/");
	printf("%d.%d.%d\n",v[0],v[1],v[2]);
	printf("\tget_short_versions: skipped\n");
	printf("\tget_bitcode_versions: skipped\n");
	printf("\tget_short: %s\n", get_short("creativecommons.org/licenses/by/2.5/au/"));
	printf("\thas_flag: %d\n", has_flag(1,"creativecommons.org/licenses/by/2.5/au/"));
	printf("\tfamily_flags: skipped\n");
	printf("\tfamily: %s\n", family("creativecommons.org/licenses/by/2.5/au/"));
	printf("\tget_notification: %s\n", get_notification("creativecommons.org/licenses/by/2.5/au/"));
	printf("\tget_verifiable_notification: %s\n",get_verifiable_notification("creativecommons.org/licenses/by/2.5/au/", "http://www.mysite.com/bleh"));
	printf("\tverify_uri: %d\n", verify_uri("creativecommons.org/licenses/by/2.5/au/"));
	printf("\tget_bitcode_license: %s\n", get_bitcode_license("Creative Commons",1, "au","au"));
	printf("\tget_short_license: %s\n", get_short_license("Creative Commons","attribution", "au","au"));
	printf("\tget_all_licenses: skipped\n");
	printf("\tget_general_licenses: skipped\n");
	printf("\tget_families: skipped\n");
	return 0;
}
