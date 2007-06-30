#include "read_license.h"
int main(int argc, char** argv) {
	char* file="license_me.txt";
	uri_t license = ll_read(file);
	printf("File license: %s\n",license);
	free(license);
	return 0;
}
