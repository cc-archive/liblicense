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
#define MAX_URI_LENGTH 50
void init() {
}

void shutdown() {
}

int set_default(char* u) {
	FILE* file = fopen("default-license.txt","w");
	int result = fprintf(file,"%s",u);
	fclose(file);
	return result;
}

char* get_default() {
	FILE* file = fopen("default-license.txt","r");
	if (file==NULL)
		return strdup("");
	char tmp[MAX_URI_LENGTH] = "";
	fgets(tmp,MAX_URI_LENGTH,file);
	fclose(file);
	return strdup(tmp);
}
