/*
 * Creative Commons has made the contents of this file
 * available under a CC-GNU-LGPL license:
 *
 * http://creativecommons.org/licenses/LGPL/2.1/
 *
 * A copy of the full license can be found as part of this
 * distribution in the file COPYING.
 *
 * You may use the liblicense software in accordance with the
 * terms of that license. You agree that you are solely
 * responsible for your use of the liblicense software and you
 * represent and warrant to Creative Commons that your use
 * of the liblicense software will comply with the CC-GNU-LGPL.
 *
 * Copyright 2007, Creative Commons, www.creativecommons.org.
 * Copyright 2007, Scott Shawcroft.
 * Copyright (C) 2007 Peter Miller
 */

#include <liblicense.h>

#include <stdio.h>

const char *stub_supported_predicates[] = {NULL};
const char *stub_mime_types[] = {NULL};

LL_MODULE_DEFINE("stub.so",
		 "A stupid module that can't read or write a license",
		 "0.2",
		 LL_FEATURES_NONE,
		 stub_supported_predicates,
		 stub_mime_types,
		 NULL /* no init */,
         NULL /* no read */,
         NULL /* no write */,
         NULL /* no shutdown */);
