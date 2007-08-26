// Creative Commons has made the contents of this file
// available under a CC-GNU-LGPL license:
//
// http://creativecommons.org/licenses/LGPL/2.1/
//
// A copy of the full license can be found as part of this
// distribution in the file COPYING.
// 
// You may use the liblicense software in accordance with the
// terms of that license. You agree that you are solely 
// responsible for your use of the liblicense software and you
// represent and warrant to Creative Commons that your use
// of the liblicense software will comply with the CC-GNU-LGPL.
//
// Copyright 2007, Creative Commons, www.creativecommons.org.
// Copyright 2007, Scott Shawcroft.

#include "liblicense.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <dirent.h>
#include <assert.h>

#define MAX_MODULES 10
#define MAX_MIME_TYPES 20
#define MAX_LINE_LENGTH 150

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

LLModuleDesc **_module_list = NULL;

int _ll_so_filter(const struct dirent * d) {
	return strstr(d->d_name,".so")!=NULL;
}

void ll_init_modules() {
	if (_module_list) return;

	struct dirent **namelist;
	int n = scandir(LIBLICENSE_IO_MODULE_DIR , &namelist, _ll_so_filter, alphasort);
	if (n==-1) {
		fprintf(stderr,"No io modules found.");
		return;
	}

	_module_list = (LLModuleDesc**)calloc(n+1,sizeof(LLModuleDesc*));
	LLModuleDesc **curr_module = _module_list;

	int i;
	for (i=0;i<n;++i) {
		char reg_file[strlen(LIBLICENSE_IO_MODULE_DIR)+strlen(namelist[i]->d_name)+1];
		reg_file[0]='\0';
		strcat(reg_file,LIBLICENSE_IO_MODULE_DIR);
		strcat(reg_file,namelist[i]->d_name);

		void *handle = dlopen(reg_file,RTLD_LAZY);
		if (handle) {
			LLModuleDesc *module_desc = dlsym(handle,"ll_module_desc");
			if (!module_desc) {
				fprintf(stderr,"Error: module '%s' doesn't call the required LL_MODULE_INIT\n",reg_file);
				dlclose(handle);
				continue;
			}
			module_desc->handle = handle;
			module_desc->module_init();
			*curr_module = module_desc;
			curr_module++;
		} else {
			fprintf(stderr,"Unable to dlopen() '%s'\n",reg_file);
		}
		free(namelist[i]);
	}
	free(namelist);
}

void ll_stop_modules() {
	LLModuleDesc **curr_module = _module_list;
	while (*curr_module) {
		dlclose((*curr_module)->handle);
		++curr_module;
	}
	free(_module_list);
	_module_list = NULL;
}

ll_module_t* ll_get_config_modules() {
  struct dirent **namelist;
  int n = scandir(LIBLICENSE_CONFIG_MODULE_DIR , &namelist, _ll_so_filter, alphasort);
  if (n==-1) {
  	fprintf(stderr,"No config modules found.");
  	return ll_new_list(0);
  }
	ll_module_t* result = ll_new_list(n);
  int i;
  for (i=0;i<n;++i) {
    result[i] = strdup(namelist[i]->d_name);
    free(namelist[i]);
  }
  free(namelist);
	return result;
}
ll_module_t* ll_get_io_modules() {
	assert(_module_list);

	int length = 0;
	while (_module_list[length]) {length++;}

	ll_module_t* result = ll_new_list(length);

	int i;
	for (i = 0; i < length; i++) {
		result[i] = strdup(_module_list[i]->name);
	}

	return result;
}

int ll_module_init(char* directory,ll_module_t m) {// create file to open
	char reg_file[strlen(directory)+strlen(m)+1];
	reg_file[0]='\0';
	strcat(reg_file,directory);
	strcat(reg_file,m);
	void *handle = dlopen(reg_file,RTLD_LAZY);

	if (handle) {
		void (*function_init)() = dlsym(handle,"init");
		(*function_init)();
		return 1;
	}
	return 0;
}

int ll_module_shutdown(char* directory,ll_module_t m) {// create file to open
	char lib_file[strlen(directory)+strlen(m)+1];
	lib_file[0]='\0';
	strcat(lib_file,directory);
	strcat(lib_file,m);
	
	//Get the handle without skewing the number open.
	void* handle = dlopen(lib_file,RTLD_LAZY);

	void (*function_shutdown)() = dlsym(handle,"shutdown");
	(*function_shutdown)();

	dlclose(handle);
	
	//close
	dlclose(handle);
	return 0;
}

void* ll_get_module_symbol(char* directory, ll_module_t m, ll_symbol_t s) {
	// create file to open
	char reg_file[strlen(directory)+strlen(m)+1];
	reg_file[0]='\0';
	strcat(reg_file,directory);
	strcat(reg_file,m);
	void* handle = dlopen(reg_file,RTLD_LAZY);
	if (!handle) {
	  fprintf(stderr, "%s\n", dlerror());
	  exit(EXIT_FAILURE);
  }
	void* symbol = dlsym(handle,s);
	dlclose(handle);
	return symbol;
}

// IO module functions.
void ll_print_module_info() {
	assert(_module_list);

	LLModuleDesc **curr_module = _module_list;
	while (*curr_module) {
		printf("%s - %s\n",(*curr_module)->name,(*curr_module)->description);
		printf("\tSupported formats: %s\n",(*curr_module)->mime_types ? (*curr_module)->mime_types : "any");
		printf("\tRead support: %s\n",(*curr_module)->read ? "yes" : "no");
		printf("\tWrite support: %s\n",(*curr_module)->write ? "yes" : "no");
		if ((*curr_module)->features) {
			printf("\tOther features:");
			if ((*curr_module)->features & LL_FEATURES_EMBED) {
				printf(" embeds natively in file\n");
			}
		}
		++curr_module;
	}
}

int _ll_contains_token(char *string, const char *token)
{
	char *copy = strdup(string);

	char * pch = strtok (copy," ");
	while (pch != NULL)
	{
		if ( strcmp(pch,token) == 0 ) {
			free(copy);
			return 1;
		}
		pch = strtok (NULL, " ");
	}

	free(copy);
	return 0;
}

