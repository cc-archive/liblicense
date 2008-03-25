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
		"http://creativecommons.org/licenses/by-nc-nd/3.0/us/",
		NULL};
	char * empty[] = {NULL};

	char * nc_and_ncnd[] = {
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
	static const char *attributes2[] = {
                        LL_DERIVATIVE_WORKS,
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

	ll_license_chooser_t *license_chooser;
	int permits_flags;
        int requires_flags;
        int prohibits_flags;


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
	const char * ncnd_permits[] = {LL_DISTRIBUTION, NULL};
	const char * ncnd_requires[] = {LL_ATTRIBUTION, NULL};
	const char * ncnd_prohibits[] = {LL_COMMERCIAL_USE, NULL};
	const char ** ncnd_results;

	ncnd_results = check(NULL,
			     gpl_attributes,
			     ncnd_permits,
			     ncnd_requires,
			     ncnd_prohibits);
	//print_licenses(ncnd_results, ncnd);

	print_attributes(attributes2);
	license_chooser = ll_new_license_chooser("http://creativecommons.org/international/us",attributes2);

	/* returns all the licenses that don't specify DerivativeWorks */
	permits_flags = LL_UNSPECIFIED;
	requires_flags = LL_UNSPECIFIED;
	prohibits_flags = LL_UNSPECIFIED;

	print_flags(attributes2,permits_flags,requires_flags,prohibits_flags);
	results = ll_get_licenses_from_flags(license_chooser,permits_flags,requires_flags,prohibits_flags);
	print_licenses(results, nc_and_ncnd);
	
	ll_free_license_chooser(license_chooser);

	ll_stop();

	return 0;
}
