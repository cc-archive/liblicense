// Creative Commons has made the contents of this file
// available under a CC-GNU-GPL license:
//
// http://creativecommons.org/licenses/GPL/2.0/
//
// A copy of the full license can be found as part of this
// distribution in the file COPYING.
// 
// You may use the liblicense software in accordance with the
// terms of that license. You agree that you are solely 
// responsible for your use of the liblicense software and you
// represent and warrant to Creative Commons that your use
// of the liblicense software will comply with the CC-GNU-GPL.
//
// Copyright 2007, Creative Commons, www.creativecommons.org.
// Copyright 2007, Scott Shawcroft.

#include "liblicense.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
static int verbose_flag;
static int set_flag;

void help() {
	printf("	Usage: license [OPTION] [FILE]\n");
	printf("Reads and writes license information for a given file or the system default.\n");
	printf("If options are omitted, assume default license.\n");
	printf("If file is omitted, assumes system default.\n");
	printf("\n");
	printf("       --verbose               Outputs more license information.\n");
	printf("       --quiet                 Output less.\n");
	printf("   -a, --list=JURISDICTION     Lists all available licenses in JURISDICTION\n");
	printf("                                 or unported licenses by default.\n");
	printf("       --set                   Sets the license instead of reading it.\n");
	printf("   -l, --license=URI           Uses the license with URI instead of default.\n");
	printf("       --help                  Output this help text and quit.\n");
	printf("\n");
	printf("Exit status is 0 if OK, 1 if no default license is set and 2 if the given\n");
	printf("license does not exist.\n");
}

void print_license_info(uri_t license) {
	printf("More.\n");
}

int list_juris(juris_t j) {
	uri_t* licenses = ll_get_licenses(j);
	int i = 0;
	while (licenses[i]!=NULL) {
		printf("%s - %s\n",ll_get_name(licenses[i]), licenses[i]);
		i++;
	}
	return 0;
}

int main(int argc, char** argv) {
	static struct option long_options[] =
	{
		{"verbose", no_argument, &verbose_flag, 1},
		{"quiet", no_argument, &verbose_flag, 0},
		{"set", no_argument, &set_flag, 1},
		{"license", required_argument, 0, 'l'},
		{"help", no_argument, 0, 'h'},
		{"list-all", optional_argument, 0, 'a'},
		{0,0,0,0}
	};
	int c;
	int option_index;
	uri_t license = NULL;
	ll_init();
	while((c = getopt_long(argc,argv,"l:a::",long_options,&option_index))!=-1) {
		switch(c) {
			case 0:
				continue;
			case 'h':
				help();
				return 0;
			case 'l':
				if (optarg!=NULL) {
					if (ll_verify_uri(optarg))
						license=optarg;
					else {
						fprintf(stderr,"Error: License '%s' does not exist.\n",optarg);
						return 2;
					}
				}
				break;
			case 'a':
				if (optarg!=NULL)
					list_juris(optarg);
				else
					list_juris(NULL);
				ll_stop();
				return 0;
			case '?':
				break;
			default:
				abort();
		}
	}
	if (set_flag) {
		if (license==NULL) {
			license = ll_get_default();
			if (license==NULL) {
				fprintf(stderr,"Error: No default license set.\n");
				ll_stop();
				return 1;
			}			
		}
	}
	if (argc==optind) { /* No more arguments, assume default. */
		if (set_flag) {
			ll_set_default(license);
		} else {
			license = ll_get_default();
		}
		printf("The system default license is %s.\n",license);
		if (verbose_flag)
			print_license_info(license);
	} else { /* Next argument is file, use it. */
		if (set_flag) {
			ll_write(argv[optind],license);
		} else {
			license = ll_read(argv[optind]);
		}
		if (license)
			printf("%s is licensed under %s\n",argv[optind],license);
		else
			printf("No license found for %s\n",argv[optind]);
	}
	ll_stop();
	return 0;
}
