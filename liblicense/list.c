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

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

char** ll_new_list(int length) {
	return (char**) calloc(length+1,sizeof(char*));
}
void ll_free_list(char** list) {
        int i;

	if (list==NULL)
		return;
	i = 0;
	while(list[i]!=NULL) {
		free(list[i]);
		i++;
	}
	free(list);
}

int ll_list_contains(char** list, char* needle) {
        int i;

	if (list==NULL || needle==NULL)
		return false;
	i = 0;
	while(list[i]!=NULL) {
		if (strcmp(list[i],needle)==0)
			return true;
		i++;
	}
	return false;
}

int ll_list_index(char** list, char* needle) {
	int i=0;
	while(list[i]!=NULL) {
		if (strcmp(list[i],needle)==0)
			return i;
		i++;
	}
	return -1;
}

int ll_list_length(char** list) {
	int i=0;
	while(list[i]!=NULL)
		i++;
	return i;
}

char* ll_list_mode(char** list, char* ignore) {
	char** values;
	int* counts;
        int i;
        int x;
        int max;
	char* max_value;

	values = ll_new_list(ll_list_length(list));
	counts = (int*) calloc(ll_list_length(list),sizeof(int));
	i = 0;
	x = 0;
	while (list[i]!=NULL) {
		if(strcmp(list[i],ignore)!=0) {
			if (!ll_list_contains(values,list[i])) {
				values[x]=list[i];
				x++;
			}
			counts[ll_list_index(values,list[i])]++;
		}
		i++;
	}
	max = 0;
	max_value = NULL;
	i = 0;
	while (values[i]!=NULL) {
		if(counts[i]>max) {
			max = counts[i];
			max_value = values[i];
		}
		i++;
	}
	free(values);
	free(counts);
	return max_value;
}

void ll_list_print(char** list) {
	if (list==NULL) {
		printf("(null)\n");
		return;
	}
	printf("[ ");
	if (list[0]!=NULL) {
                int i;

		printf("'%s'",list[0]);
		i = 1;
		while (list[i]!=NULL)
			printf(", '%s'",list[i++]);
	}
	printf(" ]\n");
}
