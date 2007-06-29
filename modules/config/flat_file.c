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
		return "";
	char tmp[MAX_URI_LENGTH] = "";
	fgets(tmp,MAX_URI_LENGTH,file);
	fclose(file);
	return strdup(tmp);
}
