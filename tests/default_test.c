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

#include <liblicense/liblicense.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int
main (int argc, char **argv)
{
  char *license;
  char *response;

  (void) argc;
  (void) argv;
  printf
    ("Warning: this test will overwrite system-wide defaults if run.  "
    "To stop hit control-C.  You have 3 seconds.\n\n");
  fflush (stdout);
  /* BREAKS_WINDOWS */
  sleep (3);
  license = "http://creativecommons.org/licenses/by-nd/2.0/";
  printf ("Setting default to '%s': %d\n", license,
          ll_license_default_set (license));
  response = ll_license_default_get ();
  printf ("Getting the default: %s\n", response);
  free (response);
  printf ("Setting default to '%s': %d\n", "", ll_license_default_set (""));
  response = ll_license_default_get ();
  printf ("Getting the default: %s\n", response);
  free (response);
  return 0;
}
