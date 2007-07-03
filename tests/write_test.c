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

#include "write_license.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
	char* file="license_me.txt";
	uri_t license = "creativecommons.org/licenses/by/2.5/au/";
	int result = ll_write(file,license);
	printf("Write result: %d\n",result);
	return 0;
}
