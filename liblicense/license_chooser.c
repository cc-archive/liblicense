
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <liblicense.h>

struct _ll_license_chooser_t {
	char **attributes;
	int num_attributes;
	ll_license_list_t **license_list;
	uri_t *all_licenses; 	//to avoid copying each license, store the license entire license list
												//and free them all in one go when we destroy the license_chooser
};

#define LL_PERMITS 			0
#define LL_REQUIRES 		1
#define LL_PROHIBITS 		2
#define LL_UNSPECIFIED 	3

/* Note: if N_STATES changes, lines like these also need to be adapted:
 * 1 << (num_attributes * 2)
 * which are optimized from pow(N_STATES,num_attributes)
 * due to the fact that 4 is a power of 2
 */
#define N_STATES 4

int indexAt( int height )
{
	int h = 0;
	int i = N_STATES;
	int counter = 1;

	while (h < height) {
		h++;
		counter *= N_STATES;
		i += counter;
	}
	return i - counter;
}

int size( int num_attributes )
{
	int h = 0;
	int i = N_STATES;
	int counter = 1;

	while (h < num_attributes) {
		h++;
		counter *= N_STATES;
		i += counter;
	}
	return i;
}

void iterate_children( int *license_hits, uri_t license, int index, int height, int num_attributes )
{
	int leftChild = (index-N_STATES+2)*N_STATES;

	if (leftChild >= size(num_attributes)) {
		/* the array tracks leaf nodes, while the heap tracks all nodes */
		int arrayIndex = index-indexAt(height);
		license_hits[arrayIndex] += 1;
		return;
	}

	int i = 0;
	while ( i<N_STATES ) {
		iterate_children( license_hits, license, leftChild+i, height+1, num_attributes );
		i++;
	}
}

int attribute_index( char **attributes, char *attr, int num_attributes )
{
	int i;
	for (i=0; i<num_attributes; ++i) {
		if ( strcmp(attr,attributes[i]) == 0 )
			return i+1;
	}
	return -1;
}

int ll_attribute_flag( ll_license_chooser_t *license_chooser, const char *attr )
{
	int i;
	for (i=0; i<license_chooser->num_attributes; ++i) {
		if (strcmp(attr,license_chooser->attributes[i])==0) {
			return (1<<i);
		}
	}
	return -1;
}

const ll_license_list_t* ll_get_licenses_from_flags( ll_license_chooser_t *license_chooser, int permits, int requires, int prohibits )
{
	/* traverse the down the tree until we get to the right leaf */
	int curr = N_STATES - 1;
	int i;
	for (i=1; i<(1<<license_chooser->num_attributes); i <<= 1) {
		if ( permits & i ) {
			curr = (curr-N_STATES+2)*N_STATES+LL_PERMITS;
		}
		else if ( requires & i ) {
			curr = (curr-N_STATES+2)*N_STATES+LL_REQUIRES;
		}
		else if ( prohibits & i ) {
			curr = (curr-N_STATES+2)*N_STATES+LL_PROHIBITS;
		}
		else {
			curr = (curr-N_STATES+2)*N_STATES+LL_UNSPECIFIED;
		}
	}
	int arrayIndex = curr-indexAt(license_chooser->num_attributes);
	return license_chooser->license_list[arrayIndex]->next;
}

void ll_get_license_flags( ll_license_chooser_t *license_chooser, int *permits, int *requires, int *prohibits )
{
}

ll_license_chooser_t* ll_new_license_chooser( const juris_t jurisdiction, char **attributes )
{
	ll_license_chooser_t *chooser = (ll_license_chooser_t*)malloc(sizeof(ll_license_chooser_t));

	uri_t *licenses = ll_get_licenses(jurisdiction);
	
	int num_attributes;
	for (num_attributes = 0; attributes[num_attributes]; ++num_attributes);
	int num_nodes = 1 << (num_attributes * 2);

	int license_hits[num_nodes];
	ll_license_list_t **license_heap = (ll_license_list_t**)malloc(sizeof(ll_license_list_t*)*num_nodes);

	int i;
	for (i=0; i<num_nodes; ++i) {
		license_hits[i] = 0;
		license_heap[i] = (ll_license_list_t*)calloc(1,sizeof(ll_license_list_t));
	}

	int used_attrs;
	char **attr;
	char **attrs;
	uri_t *license;
	for ( license = licenses; *license; ++license ) {
		used_attrs = 0x0000;

		attrs = ll_get_permits(*license);
		for (attr=attrs; *attr; ++attr) {
			int attr_index = attribute_index(attributes,*attr,num_attributes);
			if (attr_index == -1) continue;

			used_attrs |= 1 << attr_index;

			for (i=0; i<(1 << (attr_index * 2 - 2)); ++i) {
				iterate_children( license_hits, *license, indexAt(attr_index)+i*N_STATES+LL_PERMITS, attr_index, num_attributes );
			}
		}
		ll_free_list(attrs);

		attrs = ll_get_requires(*license);
		for (attr=attrs; *attr; ++attr) {
			int attr_index = attribute_index(attributes,*attr,num_attributes);
			if (attr_index == -1) continue;

			used_attrs |= 1 << attr_index;

			for (i=0; i<(1 << (attr_index * 2 - 2)); ++i) {
				iterate_children( license_hits, *license, indexAt(attr_index)+i*N_STATES+LL_REQUIRES, attr_index, num_attributes );
			}
		}
		ll_free_list(attrs);

		attrs = ll_get_prohibits(*license);
		for (attr=attrs; *attr; ++attr) {
			int attr_index = attribute_index(attributes,*attr,num_attributes);
			if (attr_index == -1) continue;

			used_attrs |= 1 << attr_index;

			for (i=0; i<(1 << (attr_index * 2 - 2)); ++i) {
				iterate_children( license_hits, *license, indexAt(attr_index)+i*N_STATES+LL_PROHIBITS, attr_index, num_attributes );
			}
		}

		for (i=1; i<=num_attributes; ++i) {
			if ( !(used_attrs & (1<<i)) ) {
				int attr_index = i;
				int j;
				for (j=0; j<(1 << (attr_index * 2 - 2)); ++j) {
					iterate_children( license_hits, *license, indexAt(attr_index)+j*N_STATES+LL_UNSPECIFIED, attr_index, num_attributes );
				}
			}
		}
		ll_free_list(attrs);

		for (i=0; i<num_nodes; ++i) {
			if ( license_hits[i] == num_attributes ) {
				ll_license_list_t *new_license_list = (ll_license_list_t*)malloc(sizeof(ll_license_list_t));
				new_license_list->license = *license;

				ll_license_list_t *tmp = license_heap[i]->next;
				license_heap[i]->next = new_license_list;
				new_license_list->next = tmp;
			}
			license_hits[i] = 0;
		}
	}

	chooser->all_licenses = licenses;
	chooser->num_attributes = num_attributes;
	chooser->attributes = (char**)malloc(sizeof(char*)*num_attributes);
	for (i=0; i<num_attributes; ++i) {
		chooser->attributes[i] = strdup(attributes[i]);
	}
	chooser->license_list = license_heap;

	return chooser;
}

void ll_free_license_chooser(ll_license_chooser_t *chooser)
{
	int num_nodes = 1 << (chooser->num_attributes * 2);

	int i;
	ll_license_list_t *curr, *tmp;
	for (i=0; i<num_nodes; ++i) {
		curr = chooser->license_list[i];
		while (curr) {
			tmp = curr->next;
			free(curr);
			curr = tmp;
		}
	}

	for (i=0; i<chooser->num_attributes; ++i) {
		free(chooser->attributes[i]);
	}
	free(chooser->attributes);

	free(chooser->license_list);
	ll_free_list(chooser->all_licenses);
	free(chooser);
}
