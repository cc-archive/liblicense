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

#include "system_licenses.h"
#include "module_wrangler.h"
#ifndef LIBLICENSE_WRITE_LICENSE_H
#define LIBLICENSE_WRITE_LICENSE_H
	int ll_write(filename_t,uri_t);
	int ll_module_write(filename_t,uri_t,module_t);
#endif
