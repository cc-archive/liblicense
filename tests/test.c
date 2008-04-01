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

#include "liblicense.h"

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <assert.h>

int
main (int argc, char **argv)
{
  int verified;
  int known_good_version[] = {2,2,0};
  char *known_good_title[] = {"Attribution-NoDerivs", NULL};

  char **result1;
  int i;
  ll_version_t version;
  char *result;
  char *u = "http://creativecommons.org/licenses/by-nd/2.0/";

  (void) argc;
  (void) argv;
  printf("Printing LL_LICENSE as dug out of liblicense: ");
  printf("%s\n", LL_LICENSE);

  printf ("Starting ll:\n");
  ll_init ();
  printf ("Test system_licenses:\n");
  result =
    ll_get_first(ll_get_attribute(u, LL_JURISDICTION, false));
  printf ("\tget_jurisdiction: %s\n", result);
  assert (result == NULL);
  free (result);
  result = ll_get_first(ll_get_attribute(u, LL_NAME, false));
  printf ("\tget_name: %s\n", result);
  assert(strcmp(result, "Nevezd meg! - Ne változtasd!") == 0);
  /* FIXME
   * get_name returns Hungarian.
   * why? 
   */
  free (result);

  printf ("\tget_version: ");
  version = ll_parse_version(ll_get_first (ll_get_attribute (u, LL_VERSION, false)));
  assert (ll_int_arrays_equal(version, known_good_version));
  
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
      printf ("unversioned\n");
    }

  
  verified = ll_verify_uri ("creativecommons.org/licenses/by/2.5/au/");
  printf ("\tverify_uri without http prefix: %d\n", verified);
  assert (! verified);

  verified = ll_verify_uri ("http://creativecommons.org/licenses/by/2.5/au/");
  printf ("\tverify_uri with http prefix: %d\n", verified);
  assert (verified);

  printf ("\tget_attribute:\n");
  result1 =
    ll_get_attribute ("http://creativecommons.org/licenses/by-nd/2.0/za/",
                      "http://purl.org/dc/elements/1.1/title", 1);
  i = 0;
  assert(ll_lists_equal(result1, known_good_title));

  while (result1[i] != NULL)
    printf ("\t\t%s\n", result1[i++]);
  printf ("\tget_all_licenses:\n");
  ll_free_list (result1);
  result1 = ll_get_all_licenses ();
  assert (ll_list_contains(result1, "http://creativecommons.org/licenses/by/1.0/"));

  if(result1 == NULL) {
	  printf("ERROR: ll_get_all_licenses() failed");
	  ll_stop();
	  return 1;
  }
  i = 0;
  while (result1[i] != NULL)
    printf ("\t\t%s\n", result1[i++]);
  printf ("\tget_licenses: \n"); /* Only ones that are current */
  ll_free_list (result1);
  result1 = ll_get_licenses (NULL);
  assert (! ll_list_contains(result1, "http://creativecommons.org/licenses/by/1.0/"));
  assert (ll_list_contains(result1, "http://creativecommons.org/licenses/by/3.0/"));
  i = 0;
  while (result1[i] != NULL)
    printf ("\t\t%s\n", result1[i++]);
  ll_free_list (result1);


  printf ("Stopping ll:\n");
  ll_stop ();
  return 0;
}
