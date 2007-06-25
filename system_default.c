#include "system_default.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
int ll_set_default(const uri_t u) {
	return true;
}

const char* ll_get_default() {
	return strdup("http://creativecommons.org/licenses/by-nd/2.0/");
}

char** ll_default_engines() {
	return NULL;
}
