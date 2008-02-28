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

#include <errno.h>
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

#include "modules.h"

const char *liblicense_io_module_dir;
const char *liblicense_config_module_dir;

LLModuleDesc **_ll_module_list = NULL;

int _ll_so_filter(const struct dirent * d) {
	return strstr(d->d_name,".so")!=NULL;
}

void ll_init_modules(void) {
	struct dirent **namelist;
        int n;
	LLModuleDesc **curr_module;
	int i;

	liblicense_io_module_dir = getenv("LL_IO_MODULE_DIR");
	if (liblicense_io_module_dir == NULL)
		liblicense_io_module_dir = LIBLICENSE_IO_MODULE_DIR;
	if (_ll_module_list) return;
	n = scandir(liblicense_io_module_dir , &namelist, _ll_so_filter, alphasort);
	if (n==-1) {
		fprintf(stderr, "scandir(\"%s\"): %s\n",
                        liblicense_io_module_dir, strerror(errno));
		return;
	}

	_ll_module_list = (LLModuleDesc**)calloc(n+1,sizeof(LLModuleDesc*));
	curr_module = _ll_module_list;

	for (i=0;i<n;++i) {
		char reg_file[strlen(liblicense_io_module_dir)+strlen(namelist[i]->d_name)+1];
		void *handle;

		reg_file[0]='\0';
		strcat(reg_file,liblicense_io_module_dir);
		strcat(reg_file,namelist[i]->d_name);

		handle = dlopen(reg_file,RTLD_LAZY);
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
			fprintf(stderr, "dlopen(\"%s\"): %s\n", reg_file,
                            dlerror());
		}
		free(namelist[i]);
	}
	free(namelist);
}

void ll_stop_modules(void) {
	LLModuleDesc **curr_module = _ll_module_list;
	if( curr_module ) {
		while (*curr_module) {
			if((*curr_module)->module_shutdown != NULL)
				(*curr_module)->module_shutdown();
			dlclose((*curr_module)->handle);
			++curr_module;
		}
		free(_ll_module_list);
		_ll_module_list = NULL;
	}
}

ll_module_t* ll_get_config_modules(void) {
  struct dirent **namelist;
  int n;
  ll_module_t* result;
  int i;

  liblicense_config_module_dir = getenv("LL_CONFIG_MODULE_DIR");
  if (liblicense_config_module_dir == NULL)
	  liblicense_config_module_dir = LIBLICENSE_CONFIG_MODULE_DIR;

  n = scandir(liblicense_config_module_dir, &namelist, _ll_so_filter, alphasort);
  if (n==-1) {
	  fprintf(stderr, "scandir(\"%s\"): %s", liblicense_config_module_dir,
            strerror(errno));
  	return ll_new_list(0);
  }
  result = ll_new_list(n);
  for (i=0;i<n;++i) {
    result[i] = strdup(namelist[i]->d_name);
    free(namelist[i]);
  }
  free(namelist);
  return result;
}

ll_module_t* ll_get_io_modules(void) {
	int length;
	ll_module_t* result;
	int i;

	assert(_ll_module_list);
	length = 0;
	while (_ll_module_list[length]) {length++;}

	result = ll_new_list(length);

	for (i = 0; i < length; i++) {
		result[i] = strdup(_ll_module_list[i]->name);
	}

	return result;
}

int ll_module_init(const char* directory, ll_module_t m) {
	char reg_file[strlen(directory)+strlen(m)+1];
	void *handle;

        /*
         * create file to open
         */
	reg_file[0]='\0';
	strcat(reg_file,directory);
	strcat(reg_file,m);
	handle = dlopen(reg_file,RTLD_LAZY);

	if (handle) {
		void (*function_init)(void);

		function_init = dlsym(handle, "init");
                if (function_init)
                    (*function_init)();
                /*
                 * Note that we did not call dlclose, this will be done
                 * later in the ll_module_shutdown function.
                 */
		return 1;
	}
	return 0;
}

int ll_module_shutdown(const char *directory, ll_module_t m) {
	char lib_file[strlen(directory)+strlen(m)+1];
	void* handle;
	void (*function_shutdown)(void);

        /*
         * create file to open
         */
	lib_file[0]='\0';
	strcat(lib_file,directory);
	strcat(lib_file,m);

	/*
         * Get the handle without skewing the number open.
         */
	handle = dlopen(lib_file,RTLD_LAZY);
        if (!handle)
            return -1;

	function_shutdown = dlsym(handle, "shutdown");
        if (function_shutdown)
            (*function_shutdown)();

        /*
         * We deliberately call dlclose twice.  The reference count
         * must reach zero, once for this function, and once for the
         * ll_module_init function.
         */
	dlclose(handle);

	/*
         * close
         */
	dlclose(handle);
	return 0;
}

void* ll_get_module_symbol(const char* directory, ll_module_t m, ll_symbol_t s) {
	void* handle;
	void* symbol;

	/*
         * create file to open
         */
	char reg_file[strlen(directory)+strlen(m)+1];
	reg_file[0]='\0';
	strcat(reg_file,directory);
	strcat(reg_file,m);
	handle = dlopen(reg_file,RTLD_LAZY);
	if (!handle) {
	  fprintf(stderr, "dlopen(\"%s\"): %s\n", reg_file, dlerror());
	  exit(EXIT_FAILURE);
        }
	symbol = dlsym(handle,s);
	dlclose(handle);
	return symbol;
}

/*
 * IO module functions.
 */
void ll_print_module_info(void) {
	LLModuleDesc **curr_module;

	assert(_ll_module_list);
	curr_module = _ll_module_list;
	while (*curr_module) {
		printf("%s - %s\n",(*curr_module)->name,(*curr_module)->description);
		printf("\tSupported formats: %s\n",(*curr_module)->mime_types ? (*curr_module)->mime_types : "any");
		printf("\tRead support: %s\n",(*curr_module)->read ? "yes" : "no");
		printf("\tWrite support: %s\n",(*curr_module)->write ? "yes" : "no");
		//printf("\tSupported prdicates: %s\n", (*curr_module)->supported_predicates
		if ((*curr_module)->features) {
			printf("\tOther features:");
			if ((*curr_module)->features & LL_FEATURES_EMBED) {
				printf(" embeds natively in file\n");
			}
		}
		++curr_module;
	}
}

int _ll_contains_token(const char **haystack, const char *needle)
{
  char * looking_at_this_one = haystack;
  while(looking_at_this_one != NULL) {
    // Then check
    if (strcmp(looking_at_this_one, needle) == 0)
      {
	return 1;
      }
    // Otherwise bump up to the next one
    looking_at_this_one++;
  }
  return 0; // If we get here, we've failed.
}
