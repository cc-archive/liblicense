#include "read_license.h"
#include <string.h>
uri_t ll_read(filename_t f) {
	return strdup("http://creativecommons.org/licenses/by/2.5/ar/");
}

uri_t ll_module_read(filename_t f,module_t m) {
	return strdup("http://creativecommons.org/licenses/by-sa/2.5/ar/");
}
