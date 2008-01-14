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

#include <stdio.h>

#include <liblicense.h>

void print_licenses( const ll_license_list_t *list )
{
	const ll_license_list_t *curr;

	printf("Matching licenses:\n");
	curr = list;
	while (curr) {
		printf("\t%s\n",curr->license);
		curr = curr->next;
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

int main(int argc, char *argv[])
{
	static const char *attributes[] = {
                        LL_DISTRIBUTION,
                        LL_COMMERCIAL_USE,
                        LL_DERIVATIVE_WORKS,
                        LL_SHARE_ALIKE,
                        LL_ATTRIBUTION,
			NULL
	};
	static const char *attributes2[] = {
                        LL_DERIVATIVE_WORKS,
			NULL
	};
	ll_license_chooser_t *license_chooser;
	int permits_flags;
        int requires_flags;
        int prohibits_flags;

        (void)argc;
        (void)argv;
	ll_init();

	print_attributes(attributes);

	license_chooser = ll_new_license_chooser(NULL,attributes);

	permits_flags = ll_attribute_flag(license_chooser,
                       LL_DISTRIBUTION) |
	 ll_attribute_flag(license_chooser, LL_DERIVATIVE_WORKS);
	requires_flags = ll_attribute_flag(license_chooser,
                                           LL_SHARE_ALIKE);
	prohibits_flags = LL_UNSPECIFIED;

	/*
         * Attribution and CommercialUse must be unspecified in the license RDF.
	 * If you don't want this to be the case, leave these two attributes out of the list that is
	 * passed to ll_new_license_chooser()
         */

	/* returns GPL and LGPL */
	print_flags(attributes,permits_flags,requires_flags,prohibits_flags);
	print_licenses(ll_get_licenses_from_flags(license_chooser,permits_flags,requires_flags,prohibits_flags));

	permits_flags = ll_attribute_flag(license_chooser, LL_DISTRIBUTION);
	requires_flags = ll_attribute_flag(license_chooser, LL_ATTRIBUTION);
	prohibits_flags = ll_attribute_flag(license_chooser, LL_COMMERCIAL_USE);

	/* returns by-nc-nd */
	print_flags(attributes,permits_flags,requires_flags,prohibits_flags);
	print_licenses(ll_get_licenses_from_flags(license_chooser,permits_flags,requires_flags,prohibits_flags));

	ll_free_license_chooser(license_chooser);

	print_attributes(attributes2);
	license_chooser = ll_new_license_chooser("us",attributes2);

	/* returns all the licenses that don't specify DerivativeWorks */
	permits_flags = LL_UNSPECIFIED;
	requires_flags = LL_UNSPECIFIED;
	prohibits_flags = LL_UNSPECIFIED;

	print_flags(attributes2,permits_flags,requires_flags,prohibits_flags);
	print_licenses(ll_get_licenses_from_flags(license_chooser,permits_flags,requires_flags,prohibits_flags));

	ll_free_license_chooser(license_chooser);

	ll_stop();

	return 0;
}
