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
#include <assert.h>
#include "liblicense.h"
static int verbose_flag = 0;
static int set_license_flag = 0;
static int remove_license_flag = 0;
static int set_webstatement_flag = 0;
static int remove_webstatement_flag = 0;

static const int E_NO_DEFAULT_LICENSE=1;
static const int E_LICENSE_DOES_NOT_VERIFY=2;
static const int E_COULD_NOT_WRITE=3;
static const int E_NO_WEB_STATEMENT_SPECIFIED=4;

static int die(const char * error_message_fmt_string,
	       const char * fmt_string_arg,
	       int exit_code) {
  ll_stop();
  fprintf (stderr, error_message_fmt_string,
	   fmt_string_arg);
  return exit_code;
}

void
help ()
{
  printf ("Usage: license [OPTION] [FILE]\n");
  printf
    ("Reads and writes license information (and other related metadata) for a given file or the system default.\n");
  printf ("If options are omitted, uses the default license.\n");
  printf ("If file is omitted, reads and writes the system default license.\n");
  printf ("NOTE that this tool will only read and write one piece of metadata each time it is run; call it twice if you want to set the license and the web statement URI.\n");
  printf ("\n");
  printf
    ("   -v, --verbose               Outputs more license information.\n");
  printf ("   -q, --quiet                 Output less.\n");
  printf
    ("   -a, --list=JURISDICTION     Lists all available licenses in JURISDICTION\n");
  printf
    ("                                 or unported licenses by default.\n");
  printf
    ("       --set-license                   Sets the license instead of reading it.\n");
  printf
    ("       --remove-license                Removes any existing licenses from the file.\n");
  printf
    ("   -l, --license=URI           Uses the license with URI instead of default.\n");
  printf
    ("       --set-web-statement     Sets the web statement URI.\n");
  printf
    ("       --get-web-statement     Gets (and prints) the web statement URI.\n");
  printf
    ("       --remove-web-statement  Removes any web statement URI from the file.\n");
  printf
    ("   -w, --web-statement=URI     Use this web statement URI when setting (NOTE: You must also pass --set-web-statement the license with URI instead of default.\n");
  printf
    ("   -m                          Lists all available modules and their capabilities\n");
  printf
    ("                                 for reading and writing licenses in files\n");
  printf
    ("   -u, --use=MODULE            Specify which module to use to read/write the license\n");
  printf ("   -h, --help                  Output this help text and quit.\n");
  printf ("\n");
  printf
    ("Exit status is 0 if OK, 1 if no default license is set but the default was");
  printf ("necessary to proceed, 2 if the given license does not exist, and 3 if there was an error writing the metadata to the file, and 4 if you neglected to provide the web statement but asked to set it.\n");
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
  char * printable_thing_we_are_setting = NULL;
  char * uri_of_thing_we_are_setting = NULL;
  char * new_value = NULL;
  
  static struct option long_options[] =
  {
    {"verbose", no_argument, &verbose_flag, 1},
    {"quiet", no_argument, &verbose_flag, 0},
    {"set-license", no_argument, &set_license_flag, 1},
    {"remove-license", no_argument, &remove_license_flag, 1},
    {"set-web-statement", no_argument, &set_webstatement_flag, 1},
    {"get-web-statement", no_argument, &remove_webstatement_flag, 1},
    {"license", required_argument, 0, 'l'},
    {"web-statement", required_argument, 0, 'w'},
    {"use", required_argument, 0, 'u'},
    {"help", no_argument, 0, 'h'},
    {"list-all", optional_argument, 0, 'a'},
    {0, 0, 0, 0}
  };
  int write_status = 0;
  int c = 0;
  int option_index = 0;
  ll_uri_t license = NULL;
  ll_uri_t value_read_back = NULL;
  ll_module_t module = NULL;
  char * webstatement = NULL;
  char * filename = NULL;

  ll_init ();
  while (
         (c =
          getopt_long (argc, argv, "vqhmu:l:w:a::", long_options,
                       &option_index)) != -1) {
    switch (c) {
    case 0:
      continue;
    case 'h':
      help ();
      return 0;
    case 'm':
      ll_print_module_info ();
      return 0;
    case 'u':
      if (optarg != NULL) {
	module = optarg;
      }
      break;
    case 'w':
      if (optarg != NULL) {
	webstatement = optarg;
      }
      continue;
    case 'l':
      if (optarg != NULL) {
	if (ll_verify_uri (optarg)) {
	  license = optarg;
	} else {
	  return die("Error: License '%s' does not exist.\n",
		     optarg, E_LICENSE_DOES_NOT_VERIFY);
	}
      }
      break;
    case 'a':
      if (optarg != NULL) {
	list_juris (optarg);
      } else {
	list_juris (NULL);
      }
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

  /* Before doing any real processing, do some sanity checks. */

  /* If we were asked to set a license,
     ensure one was specified, or there is a default. */
  if (set_license_flag && 
      license == NULL) {
    license = ll_license_default_get(); /* We're going to need this later. */
    if (license /* still */ == NULL) {
      return die("Error: No default license set.\n", NULL, E_NO_DEFAULT_LICENSE);
    }
  }

  /* Can't set a web statement if you don't tell me which one. */
  if (set_webstatement_flag &&
      webstatement == NULL) {
    return die("Error: You asked to set the web statement but did not specify the URI to the web statement value you wanted to set.", NULL, E_NO_WEB_STATEMENT_SPECIFIED);
  }
  
  if (argc != optind) {
    /* There is an extra argument beyond the options,
       which means a filename was specified. */
    filename = argv[optind];
  }

  if (filename == NULL) {
    /* If you set the web statement operations,
       you are clearly clueless. */
    if (set_webstatement_flag || remove_webstatement_flag) {
      return die("Error: You asked to set or remove the web statement, but you did not specify the filename to operate on.", NULL, E_COULD_NOT_WRITE);
    }
    
    /* If our job was to print information about a license,
       get that out of the way. */
    if (!set_license_flag &&
	license) {
      ll_license_print_info (license);
      return 0;
    }

    /* Otherwise we are operating on the system default license,
       rather than manipulating a file. */
    if (set_license_flag) { /* If we were to set it, set it. */
      ll_license_default_set (license);
    }
   
    /* Always read it and print it. */
    license = ll_license_default_get ();
    printf ("The system default license is %s.\n", license);

    if (verbose_flag) { /* Sometimes verbosely. */
      ll_license_print_info (license);
    }
  } else {
    /* We are supposed to operate on a file. */

    /* Turn removes into sets */
    if (remove_webstatement_flag) {
      webstatement = NULL;
      set_webstatement_flag = 1;
    }
    if (remove_license_flag) {
      license = NULL;
      set_license_flag = 1; /* A removal is a set to NULL */
    }

    /* Figure out which one we're going to set. */
    if (set_webstatement_flag && set_license_flag) {
      return die("Error: You asked to modify both the embedded license and the embedded web statement, but this program is unable to do that.  Run it twice to do each task separately.\n", NULL, E_COULD_NOT_WRITE);
    }

    if (set_webstatement_flag) {
      uri_of_thing_we_are_setting = LL_WEBSTATEMENT;
      printable_thing_we_are_setting = "web statement";
      new_value = webstatement;
    } else if (set_license_flag) {
      uri_of_thing_we_are_setting = LL_LICENSE;
      printable_thing_we_are_setting = "license";
      new_value = license;
    } else {
      assert (0);
    }
      
    /* First, handle license metadata. */
    printf ("Setting %s to uri: %s\n", printable_thing_we_are_setting, license);
      if (module) {
	write_status = ll_module_write (filename, uri_of_thing_we_are_setting,
					new_value, module);
      } else {
	write_status = ll_write (filename, uri_of_thing_we_are_setting,
				 new_value);
      }
      if (write_status < 0) {
	return die ("Error: Unable to write %s to file\n", 
		    printable_thing_we_are_setting, E_COULD_NOT_WRITE);
      }

      /* Even if we wrote a license, read it to make sure it worked */
      if (module) {
	value_read_back = ll_module_read (argv[optind], 
					  uri_of_thing_we_are_setting, module);
      }
      else {
	value_read_back = ll_read (argv[optind], uri_of_thing_we_are_setting);
      }
      
      /* Print the license.  Note that printf will print NULL as
	 (null), which is great if the user asked to remove a
	 license. */
      if (value_read_back == NULL) {
	printf ("No %s found for %s\n", printable_thing_we_are_setting, 
		filename);

	/* However, if the license is NULL but we tried
	   to write a non-NULL license, then we're screwed. */
	if ( (new_value != NULL) &&
	     (value_read_back == NULL) ) {
	  return die("Unable to write %s to file\n", 
		     printable_thing_we_are_setting, E_COULD_NOT_WRITE);
	}
	
      } else { 
	printf ("%s has %s of %s\n", filename, printable_thing_we_are_setting,
		value_read_back);
	if (verbose_flag) {
	  ll_license_print_info (license);
	}
      }
    }

  ll_stop ();
  return 0;
}
