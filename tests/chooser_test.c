#include <stdio.h>

#include <liblicense.h>

void print_licenses( const ll_license_list_t *list )
{
	printf("Matching licenses:\n");
	ll_license_list_t *curr = list;
	while (curr) {
		printf("\t%s\n",curr->license);
		curr = curr->next;
	}
	printf("\n");
}

void print_attributes( char **attributes )
{
	printf("Selected attributes: \n");
	while (*attributes) {
		printf("\t%s\n",*attributes);
		++attributes;
	}
	printf("\n");
}

void print_flags( char **attributes, int p, int r, int pr )
{
	printf("Selected flags:\n");
	int i = 0;
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
	ll_init();

	char *attributes[] = {
			"http://creativecommons.org/ns#Distribution",			//0
			"http://creativecommons.org/ns#CommercialUse",		//1
			"http://creativecommons.org/ns#DerivativeWorks",	//2
			"http://creativecommons.org/ns#ShareAlike",				//3
			"http://creativecommons.org/ns#Attribution",			//4
			NULL
	};
	print_attributes(attributes);

	ll_license_chooser_t *license_chooser = ll_new_license_chooser(NULL,attributes);
	
	int permits_flags, requires_flags, prohibits_flags;
	
	//permits Distribution(0) and DerivativeWorks(2)
	permits_flags = (1 << 0) | (1 << 2);
	
	//requires ShareAlike(3)
	requires_flags = (1 << 3);
	
	//doesn't prohibit any particular attribute, but could still prohibit an attribute that isn't in the 'attributes' list
	prohibits_flags = 0;
	
	//Attribution(4) and CommercialUse(1) must be unspecified in the license RDF.
	//If you don't want this to be the case, leave these two attributes out of the list that is
	//passed to ll_new_license_chooser()
	
	//returns GPL and LGPL
	print_flags(attributes,permits_flags,requires_flags,prohibits_flags);
	print_licenses(ll_get_licenses_from_flags(license_chooser,permits_flags,requires_flags,prohibits_flags));

	//permits Distribution(0)
	permits_flags = (1 << 0);
	
	//requires ShareAlike(3)
	requires_flags = (1 << 4);
	
	//prohibits CommercialUse(1)
	prohibits_flags = (1 << 1);

	//returns all by-nc-nd licenses
	print_flags(attributes,permits_flags,requires_flags,prohibits_flags);
	print_licenses(ll_get_licenses_from_flags(license_chooser,permits_flags,requires_flags,prohibits_flags));

	//permits Distribution(0)
	permits_flags = (1 << 0);
	
	//requires ShareAlike(3)
	requires_flags = (1 << 4);
	
	//prohibits CommercialUse(1)
	prohibits_flags = (1 << 1);

	ll_free_license_chooser(license_chooser);


	char *attributes2[] = {
			"http://creativecommons.org/ns#DerivativeWorks",	//0
			NULL
	};
	print_attributes(attributes2);
	license_chooser = ll_new_license_chooser("us",attributes2);

	//returns all the licenses that don't specify DerivativeWorks
	permits_flags = 0;
	requires_flags = 0;
	prohibits_flags = 0;

	print_flags(attributes2,permits_flags,requires_flags,prohibits_flags);
	print_licenses(ll_get_licenses_from_flags(license_chooser,permits_flags,requires_flags,prohibits_flags));

	ll_free_license_chooser(license_chooser);

	ll_stop();

	return 0;
}
