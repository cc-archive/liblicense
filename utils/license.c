#include "liblicense.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int set(int argc, char** argv) {
	if (argc==2) {
		fprintf(stderr,"license: set requires at least one more argument.\n");
		return 1;
	} else if (argc>=3) {
		uri_t license;
		if (argc==3) {
			license = ll_get_default();
		} else {
			license=argv[3];
		}
		if(license!=NULL)
				printf("Setting license for %s to %s.\n",argv[2],ll_get_name(license));
		if (license!=NULL && strcmp(argv[2],"default")!=0)
			return !ll_write(argv[2],license);
		else if (license!=NULL)
			return ll_set_default(license);
		else
			return 1;
	}
	return 0;
}

int get(int argc, char** argv) {
	if (argc==2) {
		fprintf(stderr,"license: get requires one more argument.\n");
		return 1;
	} else if (argc==3) {
		if (strcmp(argv[2],"default")!=0)
			printf("'%s' is licensed under %s.\n",argv[2],ll_get_name(ll_read(argv[2])));
		else {
			uri_t license = ll_get_default();
			printf("The system default license is %s.\n",license);
		}
	}
	return 0;
}

int help(int argc, char** argv) {
	if (argc==2) {
		printf("license -- alpha version\n\
		list - lists all available licenses.\n\
		get - gets the license for the given file.\n\
		set - sets the license for the given file.\n\
		help - this.  For more give the command after help.\n");
	} else {
		if (strcmp(argv[2],"set")==0) {
			printf("set usage: license set <file> [<license uri>]\n");
			printf("note: <file> can be 'default' for system default.\n");
		} else if (strcmp(argv[2],"get")==0) {
			printf("get usage: license get <file>\n");
			printf("note: <file> can be 'default' for system default.\n");
		} else if (strcmp(argv[2],"list")==0) {
			printf("get usage: license list\n");
		} else {
			printf("unknown command: %s\n",argv[2]);
		}
	}	
	return 0;
}

int list(int argc, char** argv) {
	uri_t* licenses = ll_get_all_licenses();
	int i = 0;
	while (licenses[i]!=NULL) {
		printf("%s - %s\n",ll_get_name(licenses[i]), licenses[i]);
		i++;
	}
	return 0;
}

int main(int argc, char** argv) {
	int result = 0;
	ll_init();
	if (argc==1) {
		fprintf(stderr,"license: please enter a command and appropriate options.\n");
		result = 1;
	} else if (argc>=2) {
	
		// commands
		if (strcmp(argv[1],"list")==0) {
			result = list(argc,argv);
		} else if (strcmp(argv[1],"set")==0) {
			result = set(argc,argv);
		} else if (strcmp(argv[1],"get")==0) {
			result = get(argc,argv);
		} else if (strcmp(argv[1],"help")==0) {
			result = help(argc,argv);
		} else {
			fprintf(stderr,"license: unknown command: '%s'\n",argv[1]);
			result = 1;
		}
	}
	ll_stop();
	return result;
}
