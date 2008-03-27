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
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

static void set_and_check(char * license,
			  char * expected) {
  char * response;
  int status;

  status = ll_license_default_set (license);
  printf ("Setting default to '%s': %d\n", license, status);
  assert (status == 1);
  response = ll_license_default_get ();
  printf ("Getting the default: %s\n", response);
  assert ( (response == expected) || 
	   strcmp(response, expected) == 0);
  free (response);
}
  

int
main (int argc, char **argv)
{
  char *license = "http://creativecommons.org/licenses/by-nd/2.0/";
  char *response;
  int status;

  (void) argc;
  (void) argv;
  printf
    ("The test harness ensures that this test operates in a special, temporary"
     "home directory, so it will not will overwrite system-wide defaults."
     "\n\n"
     "Note that if you run it outside the test harness, that will NOT be true!"
     "\n\n");
  fflush (stdout);
  /* BREAKS_WINDOWS */

  set_and_check(license, license);
  set_and_check("", NULL);

  return 0;
}
