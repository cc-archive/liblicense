// Creative Commons has made the contents of this file
// available under a CC-GNU-GPL license:
//
// http://creativecommons.org/licenses/GPL/2.0/
//
// A copy of the full license can be found as part of this
// distribution in the file COPYING.
// 
// You may use the liblicense software in accordance with the
// terms of that license. You agree that you are solely 
// responsible for your use of the liblicense software and you
// represent and warrant to Creative Commons that your use
// of the liblicense software will comply with the CC-GNU-GPL.
//
// Copyright 2007, Creative Commons, www.creativecommons.org.
// Copyright 2007, Scott Shawcroft.

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
