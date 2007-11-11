/*
 * liblicense - a library for managing license metadata
 *
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
 * Copyright (C) 2007 Peter Miller
 */

#include "liblicense.h"


const char LL_ATTRIBUTE_CREATOR[] =
  "http://purl.org/dc/elements/1.1/creator";

char **
ll_license_get_creator (ll_uri_t uri)
{
    return ll_get_attribute (uri, LL_ATTRIBUTE_CREATOR, 0);
}
