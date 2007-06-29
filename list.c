#include "list.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

char** ll_new_list(int length) {
	return (char**) calloc(length+1,sizeof(char*));
}
void ll_free_list(char** list) {
	int i=0;
	while(list[i]!=NULL) {
		free(list[i]);
		i++;
	}
	free(list);
}

int ll_list_contains(char** list, char* needle) {
	int i=0;
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
	char** values = ll_new_list(ll_list_length(list));
	int* counts = (int*) calloc(ll_list_length(list),sizeof(int));
	int i = 0;
	int x = 0;
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
	int max = 0;
	char* max_value = NULL;
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
	printf("[ ");
	if (list[0]!=NULL)
		printf("'%s'",list[0]);
	int i = 1;
	while (list[i]!=NULL)
		printf(", '%s'",list[i++]);
	printf(" ]\n");
}
