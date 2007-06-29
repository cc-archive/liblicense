#include <stdio.h>
#include <unistd.h>
#include "system_default.h"
#include <stdlib.h>

int main(int argc,char** argv) {
	printf("Warning: this test will overwrite system-wide defaults if run.  To stop hit control-c.  You have 3 seconds.\n\n");
	fflush(stdout);
	// BREAKS_WINDOWS
	sleep(3);
	char* license = "http://creativecommons.org/licenses/by-nd/2.0/";
	printf("Setting default to '%s': %d\n",license,ll_set_default(license));
	char* response = ll_get_default();
	printf("Getting the default: %s\n",response);
	free(response);
	return 0;
}
