#ifndef LIBLICENSE_LIST_H
#define LIBLICENSE_LIST_H
	char** ll_new_list(int);
	void ll_free_list(char**);
	int ll_list_contains(char**,char*);
	int ll_list_index(char**,char*);
	int ll_list_length(char**);
	char* ll_list_mode(char**,char*);
	void ll_list_print(char**);
#endif
