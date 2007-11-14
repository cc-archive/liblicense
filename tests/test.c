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

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int
main (int argc, char **argv)
{
  char **result1;
  int i;
  ll_version_t version;
  char *result;

  (void) argc;
  (void) argv;
  printf ("Starting ll:\n");
  ll_init ();
  printf ("Test system_licenses:\n");
  result =
    ll_get_jurisdiction ("http://creativecommons.org/licenses/by-nd/2.0/");
  printf ("\tget_jurisdiction: %s\n", result);
  free (result);
  result = ll_get_name ("http://creativecommons.org/licenses/by-nd/2.0/");
  printf ("\tget_name: %s\n", result);
  free (result);

  printf ("\tget_version: ");
  version = ll_get_version ("http://creativecommons.org/licenses/by-nd/2.0/");
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

  printf ("\tverify_uri: %d\n",
          ll_verify_uri ("creativecommons.org/licenses/by/2.5/au/"));
  printf ("\tget_attribute:\n");
  result1 =
    ll_get_attribute ("http://creativecommons.org/licenses/by-nd/2.0/za/",
                      "http://purl.org/dc/elements/1.1/title", 1);
  i = 0;
  while (result1[i] != NULL)
    printf ("\t\t%s\n", result1[i++]);
  printf ("\tget_all_licenses:\n");
  ll_free_list (result1);
  result1 = ll_get_all_licenses ();
  if(result1 == NULL) {
	  printf("ERROR: ll_get_all_licenses() failed");
	  ll_stop();
	  return 1;
  }
  i = 0;
  while (result1[i] != NULL)
    printf ("\t\t%s\n", result1[i++]);
  printf ("\tget_licenses: \n");
  ll_free_list (result1);
  result1 = ll_get_licenses (NULL);
  i = 0;
  while (result1[i] != NULL)
    printf ("\t\t%s\n", result1[i++]);
  ll_free_list (result1);

  printf ("Test write_license:\n");
  printf ("\twrite: %d\n",
          ll_write ("license_me.txt",
                    "creativecommons.org/licenses/by/2.5/au/"));
#if 0
  printf ("\tmodule_write: %d\n",
          ll_module_write ("license_me.txt",
                           "creativecommons.org/licenses/by/2.5/au/",
                           "sidecar_xmp.so"));
#endif

  printf ("Test read_license:\n");
  {
	  ll_uri_t uri = ll_read ("license_me.txt");
	  printf ("\tread: %s\n", uri);
	  free( uri );
  }
#if 0
  printf ("\tmodule_read: %s\n",
          ll_module_read ("license_me.txt", ".libs/sidecar_xmp.so"));
#endif

  printf ("Test system_default:\n");
  printf ("\tlicense_default_set: %d\n",
          ll_license_default_set ("creativecommons.org/licenses/by/2.5/au/"));
  {
	  ll_uri_t license_default = ll_license_default_get ();
	  printf ("\tget_default: %s\n", license_default);
	  free(license_default);
  }

  printf ("Test module_wrangler:");
  printf ("\tget_config_modules: skipped\n");
  printf ("\tget_io_modules: skipped\n");
  printf ("\tget_module_symbol: skipped\n");

  printf ("Stopping ll:\n");
  ll_stop ();
  return 0;
}
