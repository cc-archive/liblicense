/* Creative Commons has made the contents of this file
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
 * Copyright 2007, Jason Kivlighn
 * Copyright (C) 2007 Peter Miller
 */

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <liblicense.h>

struct _ll_license_chooser_t {
	char **attributes;
	int num_attributes;

	/* This is the meat of the license chooser.  It is an array of linked-lists of licenses, where each slot holds all the licenses which match a given combination of license flags.  The array holds all possible combinations of attributes.
	 *
	 * Finding the matching index based on a set of flags involves traversing this structure in a tree-like manner.  A tree built for searching based on the Attribution and CommercialUse attributes is structured like this example:
	 *
	 *                                                  root
	 *                           /               |            |              \
	 * Attribution  :          P                 R           Pr              U
	 *                    /  |   |   \      / |   |  \    / |  |  \       / |  |  \
	 * CommercialUse:    P   R   Pr  U     P  R  Pr  U   P  R  Pr  U     P  R  Pr  U
	 * Indices in
	 *  license_list:    0   1   2   3    4   5  6   7   8  9  10  11   12 13  14 15
	 *
	 * In the example above, the list of licenses that require (R) Attribution and prohibit (Pr) CommercialUse
	 * is found at license_list[6].
	 *
	 * Such a structure allows easy hierarchical traversion.  Ignore the indices above and instead
	 * index the tree as a d-heap (where d=N_STATES). A node's left-most child is always (node_index-N_STATES+2)*N_STATES.
	 * N_STATES is 4 for P, R Pr, and U.
	 */
	ll_license_list_t **license_list;

	char* *all_licenses; 	/* to avoid copying each license, store the license entire license list
												 * and free them all in one go when we destroy the license_chooser
												 */
};

#define LL_ATTR_PERMITS 			0
#define LL_ATTR_REQUIRES 		1
#define LL_ATTR_PROHIBITS 		2
#define LL_ATTR_UNSPECIFIED 	3

/* Note: if N_STATES changes, lines like these also need to be adapted:
 * 1 << (num_attributes * 2)
 * which are optimized from pow(N_STATES,num_attributes)
 * due to the fact that 4 is a power of 2
 */
#define N_STATES 4

/* Returns the index of the first node of the heap at the given height */
static int indexAt( int height )
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

/* Returns the total number of nodes in the heap built for searching on the given number of attributes */
static int heap_size( int num_attributes )
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

/* Recursively traverse the license heap, increasing the appropriate index in license_hits.  When the traversal is complete, the license which has <num_attribute> hits is the matched license.
 *
 * For example, one call of iterate_children will increase license_hits[] for all licenses that prohibit CommercialUse.  On the next call to iterate_children, all licenses that require Attribution are increased.  Using the above tree in this example, first, license_hits[2,6,10,14] are all increased to 1.  Then the second call increases license_hits[4,5,6,7].  license_hits[6] is now 2, meaning we've matched the licenses at that index.
 */
static void iterate_children( int *license_hits, int index, int height, int heap_sz )
{
	int leftChild;
        int i;

	leftChild = (index-N_STATES+2)*N_STATES;
	if (leftChild >= heap_sz) { /* we've hit a leaf node, where the licenses are */
		/* the array tracks leaf nodes, while the heap tracks all nodes */
		int arrayIndex = index-indexAt(height);
		license_hits[arrayIndex] += 1;
		return;
	}

	i = 0;
	while ( i<N_STATES ) {
		iterate_children( license_hits, leftChild+i, height+1, heap_sz );
		i++;
	}
}

static int attribute_index( const char **attributes, char *attr, int num_attributes )
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
        int arrayIndex;

	/* Traverse the down the tree until we get to the right leaf.  Each pass in the for-loop traverses down one level
	 * of the tree.
	 */
	int curr = N_STATES - 1;
	int i;
	for (i=1; i<(1<<license_chooser->num_attributes); i <<= 1) {
		if ( permits & i ) {
			curr = (curr-N_STATES+2)*N_STATES+LL_ATTR_PERMITS;
		}
		else if ( requires & i ) {
			curr = (curr-N_STATES+2)*N_STATES+LL_ATTR_REQUIRES;
		}
		else if ( prohibits & i ) {
			curr = (curr-N_STATES+2)*N_STATES+LL_ATTR_PROHIBITS;
		}
		else {
			curr = (curr-N_STATES+2)*N_STATES+LL_ATTR_UNSPECIFIED;
		}
	}
	arrayIndex = curr-indexAt(license_chooser->num_attributes);
	return license_chooser->license_list[arrayIndex]->next;
}

void ll_get_license_flags( ll_license_chooser_t *license_chooser, int *permits, int *requires, int *prohibits )
{
    (void)license_chooser;
    (void)permits;
    (void)requires;
    (void)prohibits;
}

ll_license_chooser_t* ll_new_license_chooser( const ll_juris_t jurisdiction, const char **attributes )
{
	ll_license_chooser_t *chooser;
	char **licenses;
	int num_attributes;
        int num_nodes;
        int *license_hits;
	ll_license_list_t **license_heap;
        int i;
        int size;
	int used_attrs;
	char **attr;
	char **attrs;
	char **license;

	chooser = (ll_license_chooser_t*)malloc(sizeof(ll_license_chooser_t));
	licenses = ll_get_licenses(jurisdiction);
	for (num_attributes = 0; attributes[num_attributes]; ++num_attributes)
          ;
	num_nodes = 1 << (num_attributes * 2);

	license_hits = (int *)malloc(num_nodes * sizeof(int));
	license_heap = (ll_license_list_t**)malloc(sizeof(ll_license_list_t*)*num_nodes);

	for (i=0; i<num_nodes; ++i) {
		license_hits[i] = 0;
		license_heap[i] = (ll_license_list_t*)calloc(1,sizeof(ll_license_list_t));
	}

	size = heap_size(num_attributes);

	for ( license = licenses; license && *license; ++license ) {
		used_attrs = 0x0000;

		attrs = ll_get_attribute(*license, LL_PERMITS, false);
		for (attr=attrs; *attr; ++attr) {
			int attr_index = attribute_index(attributes,*attr,num_attributes);
			if (attr_index == -1)
                            continue;

			used_attrs |= 1 << attr_index;

			for (i=0; i<(1 << (attr_index * 2 - 2)); ++i) {
				iterate_children( license_hits, indexAt(attr_index)+i*N_STATES+LL_ATTR_PERMITS, attr_index, size );
			}
		}
		ll_free_list(attrs);

		attrs = ll_get_attribute(*license, LL_REQUIRES, false);
		for (attr=attrs; *attr; ++attr) {
			int attr_index = attribute_index(attributes,*attr,num_attributes);
			if (attr_index == -1) continue;

			used_attrs |= 1 << attr_index;

			for (i=0; i<(1 << (attr_index * 2 - 2)); ++i) {
				iterate_children( license_hits, indexAt(attr_index)+i*N_STATES+LL_ATTR_REQUIRES, attr_index, size );
			}
		}
		ll_free_list(attrs);

		attrs = ll_get_attribute(*license, LL_PERMITS, false);
		for (attr=attrs; *attr; ++attr) {
			int attr_index = attribute_index(attributes,*attr,num_attributes);
			if (attr_index == -1) continue;

			used_attrs |= 1 << attr_index;

			for (i=0; i<(1 << (attr_index * 2 - 2)); ++i) {
				iterate_children( license_hits, indexAt(attr_index)+i*N_STATES+LL_ATTR_PROHIBITS, attr_index, size );
			}
		}

		for (i=1; i<=num_attributes; ++i) {
			if ( !(used_attrs & (1<<i)) ) {
				int attr_index = i;
				int j;
				for (j=0; j<(1 << (attr_index * 2 - 2)); ++j) {
					iterate_children( license_hits, indexAt(attr_index)+j*N_STATES+LL_ATTR_UNSPECIFIED, attr_index, size );
				}
			}
		}
		ll_free_list(attrs);

		for (i=0; i<num_nodes; ++i) {
			if ( license_hits[i] == num_attributes ) {
				ll_license_list_t *tmp;

				ll_license_list_t *new_license_list = (ll_license_list_t*)malloc(sizeof(ll_license_list_t));
				new_license_list->license = *license;

				tmp = license_heap[i]->next;
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

        free(license_hits);
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
