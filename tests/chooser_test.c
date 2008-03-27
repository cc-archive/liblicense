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

void print_licenses( const ll_license_list_t *list, const char ** assert_equiv )
{
	int i = 0;
	const ll_license_list_t *curr;

	printf("Matching licenses:\n");
	curr = list;
	if (curr == NULL) {
		printf("\tNone.");
		if (ll_list_length(assert_equiv) != 0) {
			assert("The lists are not of" == "length.");
		}
	}
	while (curr) {
		printf("bbq %d = %s\n", i, assert_equiv[i]);
		assert (strcmp(assert_equiv[i], curr->license) == 0);
		printf("\t%s\n",curr->license);
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

	ll_stop();

	return 0;
}
