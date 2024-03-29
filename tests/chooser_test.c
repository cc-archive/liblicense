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
 * Copyright 2007, Creative Commons, www.creativecommons.org
 * Copyright (C) 2007 Peter Miller
 */

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <liblicense.h>
#include <stdbool.h>

static int destructively_find_and_remove(const char * needle,
					 char ** haystack) {
  int i = 0;

  int haystack_length = 0;

  /* Look through the haystack for this needle.
   * If we find it, great; swap this value and the bottom of the
   * the haystack for each other, and then NULL out the bottom.

   * The result is that we shrink the haystack by one and remove
   * needle if we find it.
   */

  /* First let's see how long haystack is. */
  while(haystack[haystack_length] != NULL) {
    haystack_length += 1;
  }

  for (i = 0; i < haystack_length; i++) {
    if (strcmp(haystack[i], needle) == 0) {
      /* Great, we found it.  Do the fiddly copying thing,
       * and at the end of that return true. */
      haystack[i] = haystack[haystack_length - 1]; /* Grab last element. */
      haystack[haystack_length - 1] = NULL; /* Clear last element now. */
      return true;
    }
    /* else, keep looping. */
  }

  return false;
}

void print_licenses( const ll_license_list_t *got, const char ** _hoped_for )
{
	int i = 0;
	bool found_it = false;
	const ll_license_list_t *curr;

	/* Now this makes no assumptions about the order in which the
	 * "got" licenses are received.  It wastes space and time by
	 * creating an internal copy of hoped_for and removing items
	 * from it in order, checking at the end that all have been
	 * removed. */

	char ** hoped_for; /* This will store a mutable copy of _hoped_for. */
	int hoped_for_len = 0;

	/* Calculate length of hoped_for */
	while(_hoped_for[hoped_for_len] != NULL) {
	  hoped_for_len += 1;
	}

	/* make hoped_for that big plus one. */
	hoped_for = calloc(sizeof(char *), hoped_for_len);

	/* Copy each item from _hoped_for into hoped_for. */
	for (i = 0; i < hoped_for_len ; i++) {
	  hoped_for[i] = strdup(_hoped_for[i]);
	}

	/* Be sure to free() this at the end.  Or not, it's just the setup
	 * for a silly unit test. */

	printf("Matching licenses:\n");
	curr = got;
	if (curr == NULL) {
		printf("\tNone.");
		if (ll_list_length(hoped_for) != 0) {
			assert("The lists are not of" == "length.");
		}
	}
	while (curr) {
		printf("got %d = %s\n", i, curr->license);
		found_it = destructively_find_and_remove(curr->license,
							 hoped_for);

		/* Assert that we found it. */
		assert(found_it);
		
		/* Also assert that liblicense believes this URI
		   refers to a real license. */
		ll_verify_uri(curr->license);
							 
		/* Since we found in the list of things we wanted, we
		 * move on to the next license returned by the
		 * search. */
		curr = curr->next;
		i++;
	}
	printf("\n");
}

void print_attributes(const char **attributes )
{
	printf("Selected attributes: \n");
	while (*attributes) {
		printf("\t%s\n",*attributes);
		++attributes;
	}
	printf("\n");
}

void print_flags(const char **attributes, int p, int r, int pr )
{
	int i;

	printf("Selected flags:\n");
	i = 0;
	while (attributes[i]) {
		if (p & (1<<i)) {
			printf("\tPERMITS   : %s\n",attributes[i]);
		} else if (r & (1<<i)) {
			printf("\tREQUIRES  : %s\n",attributes[i]);
		} else if (pr & (1<<i)) {
			printf("\tPROHIBITS : %s\n",attributes[i]);
		}
		++i;
	}
	printf("\n");
}

/**
 * Input: 
 * @param jurisdiction A jurisdiction; NULL is okay (means all)
 * @param attributes a NULL-terminated list of attributes to consider
 * @param permits_strings A list of attributes the license must permit
 * @param requires_strings A list of attributes the license must require (?)
 * @param prohibits_strings A list of attributes the license must prohibit
 * @return A list of strings which are license URIs that match
 */
const char ** check(const char * jurisdiction,
	      const char ** attributes,
	      const char ** permits_strings,
	      const char ** requires_strings,
	      const char ** prohibits_strings) {
  
  ll_license_chooser_t * license_chooser;
  int i = 0;
  char ** results;
  char * permits;
  char * requires;
  char * prohibits;

  int permits_flags = 0;
  int requires_flags = 0;
  int prohibits_flags = 0;
  
  print_attributes(attributes);
  license_chooser = ll_new_license_chooser(jurisdiction, attributes);
  
  /* For each of permits, requires, prohibits
   * generate the flag ints.
   */

  i = 0;
  while(permits_strings[i] != NULL) {
    permits = permits_strings[i];
    permits_flags = permits_flags |
      ll_attribute_flag(license_chooser, permits);
    i += 1;
  }

  i = 0;
  while(requires_strings[i] != NULL) {
    requires = requires_strings[i];
    requires_flags = requires_flags |
      ll_attribute_flag(license_chooser, requires);
    i += 1;
  }

  i = 0;
  while(prohibits_strings[i] != NULL) {
    prohibits = prohibits_strings[i];
    prohibits_flags = prohibits_flags |
      ll_attribute_flag(license_chooser, prohibits);
    i += 1;
  }


  /* Okay, the ints are prepared. */
  /* Print the flags just so the user sees what we're about to try. */
  print_flags(attributes,permits_flags,requires_flags,prohibits_flags);
  results = ll_get_licenses_from_flags(license_chooser,permits_flags,requires_flags,prohibits_flags);
  return results;
}

int main(int argc, char *argv[])
{
	char ** results;
	char * gpl_and_lgpl[] = {
		"http://creativecommons.org/licenses/LGPL/2.1/",
		"http://creativecommons.org/licenses/GPL/2.0/",
		NULL};
	char * ncnd[] = {
		"http://creativecommons.org/licenses/by-nc-nd/3.0/",
		NULL};
	char * nc_and_ncnd[] = {
	  "http://creativecommons.org/licenses/nd-nc/1.0/",
	  "http://creativecommons.org/licenses/nd/1.0/",
	  "http://creativecommons.org/licenses/by-nd/3.0/",
	  "http://creativecommons.org/licenses/by-nc-nd/3.0/",
	  NULL};
	char * nc_and_ncnd_us[] = {
	  "http://creativecommons.org/licenses/by-nd/3.0/us/",
	  "http://creativecommons.org/licenses/by-nc-nd/3.0/us/",
	  NULL};

	char * nc_and_ncnd_de[] = {
	  "http://creativecommons.org/licenses/by-nd/3.0/de/",
	  "http://creativecommons.org/licenses/by-nc-nd/3.0/de/",
	  "http://creativecommons.org/licenses/by-nd/2.0/de/",
	  "http://creativecommons.org/licenses/by-nc-nd/2.0/de/",
	  NULL};

	static const char *gpl_attributes[] = {
                        LL_DISTRIBUTION,
			LL_COMMERCIAL_USE,
			LL_SOURCE_CODE,
                        LL_DERIVATIVE_WORKS,
                        LL_SHARE_ALIKE,
                        LL_ATTRIBUTION,
			NULL
	};

        (void)argc;
        (void)argv;
	ll_init();

	const char * gpl_permits[] = {LL_DISTRIBUTION,
				      LL_DERIVATIVE_WORKS,
				      NULL};
	const char * gpl_requires[] = {LL_SHARE_ALIKE,
				       LL_SOURCE_CODE,
				       NULL};
	const char * gpl_prohibits[]  = {NULL};
	const char ** gpl_results;

	gpl_results = check(NULL,
			    gpl_attributes,
			    gpl_permits,
			    gpl_requires,
			    gpl_prohibits);
	
	print_licenses(gpl_results, gpl_and_lgpl);

	/* Do it again, just to ensure sanity
	   of check function. */

	gpl_results = check(NULL,
			    gpl_attributes,
			    gpl_permits,
			    gpl_requires,
			    gpl_prohibits);
	
	print_licenses(gpl_results, gpl_and_lgpl);


	/*
         * Attribution and CommercialUse must be unspecified in the license RDF.
	 * If you don't want this to be the case, leave these two attributes out of the list that is
	 * passed to ll_new_license_chooser()
         */
	
	static const char *ncnd_attributes[] = {
                        LL_DISTRIBUTION,
			LL_COMMERCIAL_USE,
                        LL_DERIVATIVE_WORKS,
                        LL_SHARE_ALIKE,
                        LL_ATTRIBUTION,
			NULL
	};

	const char * ncnd_permits[] = {LL_DISTRIBUTION, NULL};
	const char * ncnd_requires[] = {LL_ATTRIBUTION, NULL};
	const char * ncnd_prohibits[] = {LL_COMMERCIAL_USE, NULL};

	/* For one thing, assert that by-nc-nd 3.0 US has the fields
	 * we'd expect */
	int i_ncnd_permits, i_ncnd_requires, i_ncnd_prohibits = 0;
	ll_license_chooser_t * chooser = ll_new_license_chooser(NULL,
							ncnd_attributes);
	ll_get_license_flags(chooser,
			     ncnd[0],
			     &i_ncnd_permits, &i_ncnd_requires, &i_ncnd_prohibits);
	printf("%d, %d, %d for license\n", i_ncnd_permits, i_ncnd_requires, i_ncnd_prohibits);
	/* No matter what these values are set to,
	   they must be something.
	   Not -1 (which means no license found) 
	   and not 0 (which means LL_UNSPECIFIED) */
	assert (i_ncnd_permits > 0);
	assert (i_ncnd_prohibits > 0);
	assert (i_ncnd_requires > 0);

	const char ** ncnd_results;

	ncnd_results = check(NULL,
			     ncnd_attributes,
			     ncnd_permits,
			     ncnd_requires,
			     ncnd_prohibits);
	print_licenses(ncnd_results, ncnd);

	
	static const char *nc_and_ncnd_attributes[] = {
                        LL_DERIVATIVE_WORKS,
			NULL
	};
	const char * nc_and_ncnd_permits[] = {NULL};
	const char * nc_and_ncnd_prohibits[] = {NULL};
	const char * nc_and_ncnd_requires[] = {NULL};
	const char ** nc_and_ncnd_results = 
	  check(NULL,
		nc_and_ncnd_attributes,
		nc_and_ncnd_permits,
		nc_and_ncnd_requires,
		nc_and_ncnd_prohibits);

	print_licenses(nc_and_ncnd_results, nc_and_ncnd);

	const char ** nc_and_ncnd_us_results = 
	  check("http://creativecommons.org/international/us/",
		nc_and_ncnd_attributes,
		nc_and_ncnd_permits,
		nc_and_ncnd_requires,
		nc_and_ncnd_prohibits);

	print_licenses(nc_and_ncnd_us_results, nc_and_ncnd_us);


	const char ** nc_and_ncnd_de_results = 
	  check("http://creativecommons.org/international/de/",
		nc_and_ncnd_attributes,
		nc_and_ncnd_permits,
		nc_and_ncnd_requires,
		nc_and_ncnd_prohibits);

	print_licenses(nc_and_ncnd_de_results, nc_and_ncnd_de);

	ll_stop();

	return 0;
}
