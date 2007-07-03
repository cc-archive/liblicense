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
