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
 * Copyright (C) 2007 Creative Commons, www.creativecommons.org.
 * Copyright (C) 2007 Scott Shawcroft.
 * Copyright (C) 2007 Peter Miller
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "liblicense.h"


void
ll_license_print_info (ll_uri_t uri)
{
  char **attrs;
  char **attr;
  char *string;
  ll_version_t version;
  ll_juris_t juris;
  int i;

  printf ("License URI: %s\n", uri);

  string = ll_get_first(ll_get_attribute(uri, LL_NAME, false));
  if (string)
    {
      printf ("Name: %s\n", string);
      free (string);
    }

  version = ll_parse_version(ll_get_first (ll_get_attribute (uri, LL_VERSION, false)));
  printf ("Version: ");
  if (version)
    {
      for (i = 1; i <= version[0]; ++i)
        {
          if (i != 1)
            printf (".");
          printf ("%d", version[i]);
        }
      printf ("\n");
      free (version);
    }
  else
    {
      printf ("(unversioned)\n");
    }

  juris = ll_get_first(ll_get_attribute(uri, LL_JURISDICTION, false));
  if (juris)
    {
      string = ll_jurisdiction_name (juris);
    }
  else
    {
      string = strdup ("Unported");
      juris = strdup ("unported");
    }
  printf ("Jurisdiction: %s (%s)\n", string, juris);
  free (string);
  free (juris);

  attrs = ll_get_first(ll_get_attribute(uri, LL_CREATOR, false));
  if (*attrs)
    {
      printf ("Creator: %s\n", *attrs);
    }
  ll_free_list (attrs);

  printf ("Rights:\n");

  attrs = ll_get_attribute(uri, LL_PERMITS, false);
  if (*attrs)
    {
      printf ("   Permits\n");
      for (attr = attrs; *attr; ++attr)
        {
          printf ("      %s\n", *attr);
        }
    }
  ll_free_list (attrs);

  attrs = ll_get_attribute(uri, LL_REQUIRES, false);

  if (*attrs)
    {
      printf ("   Requires\n");
      for (attr = attrs; *attr; ++attr)
        {
          printf ("      %s\n", *attr);
        }
    }
  ll_free_list (attrs);

  attrs = ll_get_attribute(uri, LL_PROHIBITS, false);
  if (*attrs)
    {
      printf ("   Prohibits\n");
      for (attr = attrs; *attr; ++attr)
        {
          printf ("      %s\n", *attr);
        }
    }
  ll_free_list (attrs);
}
