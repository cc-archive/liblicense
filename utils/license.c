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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "liblicense.h"
static int verbose_flag;
static int set_flag;
static int remove_flag;

void
help ()
{
  printf ("Usage: license [OPTION] [FILE]\n");
  printf
    ("Reads and writes license information for a given file or the system default.\n");
  printf ("If options are omitted, assume default license.\n");
  printf ("If file is omitted, assumes system default.\n");
  printf ("\n");
  printf
    ("   -v, --verbose               Outputs more license information.\n");
  printf ("   -q, --quiet                 Output less.\n");
  printf
    ("   -a, --list=JURISDICTION     Lists all available licenses in JURISDICTION\n");
  printf
    ("                                 or unported licenses by default.\n");
  printf
    ("       --set                   Sets the license instead of reading it.\n");
  printf
    ("       --remove                Removes any existing licenses from the file.\n");
  printf
    ("   -l, --license=URI           Uses the license with URI instead of default.\n");
  printf
    ("   -m                          Lists all available modules and their capabilities\n");
  printf
    ("                                 for reading and writing licenses in files\n");
  printf
    ("   -u, --use=MODULE            Specify which module to use to read/write the license\n");
  printf ("   -h, --help                  Output this help text and quit.\n");
  printf ("\n");
  printf
    ("Exit status is 0 if OK, 1 if no default license is set and 2 if the given\n");
  printf ("license does not exist.\n");
}

int
list_juris (ll_juris_t j)
{
  ll_uri_t *licenses;
  int i;

  if (j != NULL && strcmp (j, "") == 0)
    j = NULL;
  licenses = ll_get_licenses (j);
  i = 0;
  while (licenses[i] != NULL)
    {
      printf ("%s - %s\n", ll_get_first(ll_get_attribute(licenses[i], LL_NAME, false)), licenses[i]);
      i++;
    }
  return 0;
}

int
main (int argc, char **argv)
{
  static struct option long_options[] =
  {
    {"verbose", no_argument, &verbose_flag, 1},
    {"quiet", no_argument, &verbose_flag, 0},
    {"set", no_argument, &set_flag, 1},
    {"remove", no_argument, &remove_flag, 1},
    {"license", required_argument, 0, 'l'},
    {"use", required_argument, 0, 'u'},
    {"help", no_argument, 0, 'h'},
    {"list-all", optional_argument, 0, 'a'},
    {0, 0, 0, 0}
  };
  int c;
  int option_index;
  ll_uri_t license = NULL;
  ll_module_t module = NULL;
  ll_init ();
  while (
         (c =
          getopt_long (argc, argv, "vqhmu:l:a::", long_options,
                       &option_index)) != -1)
    {
      switch (c)
        {
        case 0:
          continue;
        case 'h':
          help ();
          return 0;
        case 'm':
          ll_print_module_info ();
          return 0;
        case 'u':
          if (optarg != NULL)
            {
              module = optarg;
            }
          break;
        case 'l':
          if (optarg != NULL)
            {
              if (ll_verify_uri (optarg))
                license = optarg;
              else
                {
                  fprintf (stderr, "Error: License '%s' does not exist.\n",
                           optarg);
                  return 2;
                }
            }
          break;
        case 'a':
          if (optarg != NULL)
            list_juris (optarg);
          else
            list_juris (NULL);
          ll_stop ();
          return 0;
        case '?':
        case 'v':
          verbose_flag = 1;
          break;
        case 'q':
          verbose_flag = 0;
          break;
        default:
          abort ();
        }
    }
  if (set_flag)
    {
      if (license == NULL)
        {
          license = ll_license_default_get ();
          if (license == NULL)
            {
              fprintf (stderr, "Error: No default license set.\n");
              ll_stop ();
              return 1;
            }
        }
    }
  if (argc == optind)
    {
      /* No more arguments, assume default. */
      if (set_flag)
        {
          ll_license_default_set (license);
        }
      else if (license)
        {
          ll_license_print_info (license);
          return 0;
        }
      else
        {
          license = ll_license_default_get ();
        }
      printf ("The system default license is %s.\n", license);
      if (verbose_flag)
        ll_license_print_info (license);
    }
  else
    {
      /* Next argument is file, use it. */
      if (remove_flag)
        {
          license = NULL;
        }
      if (set_flag || remove_flag)
        {
          int ret;
          printf ("uri: %s\n", license);
          if (module)
            {
              ret = ll_module_write (argv[optind], LL_LICENSE, license, module);
            }
          else
            {
              ret = ll_write (argv[optind], LL_LICENSE, license);
            }
          if (!ret)
            {
              printf ("Unable to write license to file\n");
              return 2;
            }
        }
      /* Even if we wrote a license, read it to make sure it worked */
      if (module)
        {
          license = ll_module_read (argv[optind], LL_LICENSE, module);
        }
      else
        {
          license = ll_read (argv[optind], LL_LICENSE);
        }

      if (license)
        {
          printf ("%s is licensed under %s\n", argv[optind], license);
          if (verbose_flag)
            {
              ll_license_print_info (license);
            }
        }
      else
        {
          if (set_flag)
            {
              printf ("Unable to write license to file\n");
              return 2;
            }
          else
            {
              printf ("No license found for %s\n", argv[optind]);
            }
        }
    }
  ll_stop ();
  return 0;
}
