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

#include "liblicense.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <dirent.h>

#define MAX_MODULES 10
#define MAX_MIME_TYPES 20
#define MAX_LINE_LENGTH 150

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

int _ll_so_filter(const struct dirent * d) {
	return strstr(d->d_name,".so")!=NULL;
}

module_t* ll_get_config_modules() {
  struct dirent **namelist;
  int n = scandir(LIBLICENSE_CONFIG_MODULE_DIR , &namelist, _ll_so_filter, alphasort);
  if (n==-1) {
  	fprintf(stderr,"No config modules found.");
  	return ll_new_list(0);
  }
	module_t* result = ll_new_list(n);
  int i;
  for (i=0;i<n;++i) {
    result[i] = strdup(namelist[i]->d_name);
    free(namelist[i]);
  }
  free(namelist);
	return result;
}
module_t* ll_get_io_modules() {
	// temp data structure
	char** lines = ll_new_list(MAX_MODULES);
	
	// create file to open
	char reg_file[strlen(LIBLICENSE_IO_MODULE_DIR)+strlen("registry")+1];
	reg_file[0]='\0';
	strcat(reg_file,LIBLICENSE_IO_MODULE_DIR);
	strcat(reg_file,"registry");
	printf("regsitry file: %s\n",reg_file);
	// open file
	FILE* registry = fopen(reg_file,"r");
	
	// process lines
	char* line = (char*) malloc(MAX_LINE_LENGTH*sizeof(char));
	lines[0] = fgets(line, MAX_LINE_LENGTH, registry);
	int i = 0;
	while(lines[i]!=NULL) {
		// reduce down to first token on line
		lines[i]=strtok(lines[i]," \n");
		i++;
	  line = (char*) malloc(MAX_LINE_LENGTH*sizeof(char));
		lines[i] = fgets(line, MAX_LINE_LENGTH, registry);
	}
	free(line);
	fclose(registry);
	// shrink string lengths and result array
	module_t* result = ll_new_list(i+1);
	i=0;
	while(lines[i]!=NULL) {
		result[i]=strdup(lines[i]);
		i++;
	}
	ll_free_list(lines);
	return result;
}

int ll_module_init(char* directory,module_t m) {// create file to open
	char reg_file[strlen(directory)+strlen(m)+1];
	reg_file[0]='\0';
	strcat(reg_file,directory);
	strcat(reg_file,m);
	void *handle = dlopen(reg_file,RTLD_LAZY);

	if (handle) {
		void (*function_init)();
		*(void**) (&function_init) = dlsym(handle,"init");
		(*function_init)();
		return 1;
	}
	return 0;
}

int ll_module_shutdown(char* directory,module_t m) {// create file to open
	char lib_file[strlen(directory)+strlen(m)+1];
	lib_file[0]='\0';
	strcat(lib_file,directory);
	strcat(lib_file,m);
	
	//Get the handle without skewing the number open.
	void* handle = dlopen(lib_file,RTLD_LAZY);

	void (*function_shutdown)();
	*(void**) (&function_shutdown) = dlsym(handle,"shutdown");
	(*function_shutdown)();

	dlclose(handle);
	
	//close
	dlclose(handle);
	return 0;
}

void* ll_get_module_symbol(char* directory, module_t m, symbol_t s) {
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
mime_type_t* ll_module_mime_types(module_t m) {
		// temp data structure
	char** types = ll_new_list(MAX_MIME_TYPES);

	// create file to open
	char reg_file[strlen(LIBLICENSE_IO_MODULE_DIR)+strlen("registry")+1];
	reg_file[0]='\0';
	strcat(reg_file,LIBLICENSE_IO_MODULE_DIR);
	strcat(reg_file,"registry");
	
	// open file
	FILE* registry = fopen(reg_file,"r");
	
	// process lines
	int stop = false;
	char line[MAX_LINE_LENGTH] = "";
	while(line!=NULL && !stop) {
		if (fgets(line, MAX_LINE_LENGTH, registry)==NULL) {
			printf("We're in trouble.\n");
			stop=true;
		}
		// reduce down to first token on line
		char* start=strtok(line," \n");
		if (strcmp(start,m)==0) {
			stop=true;
			char* last_type="";
			int i = 0;
			while(last_type!=NULL) {
				last_type = strtok(NULL," \n");
				types[i] = last_type;
				i++;
			}
		}
	}
	fclose(registry);
	// shrink result array
	module_t* result = ll_new_list(ll_list_length(types));
	int i=0;
	while(types[i]!=NULL) {
		result[i]=strdup(types[i]);
		i++;
	}
	free(types);
	return result;
}
