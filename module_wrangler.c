#include "module_wrangler.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

#define MAX_MODULES 5
#define MAX_LINE_LENGTH 100
module_t* ll_get_config_modules() {
	module_t* result = (module_t*) malloc(3*sizeof(module_t));
	result[0] = strdup("gconf.o");
	result[1] = strdup("xdg.o");
	result[2] = NULL;
	return result;
}
module_t* ll_get_io_modules() {
	// temp data structure
	char** lines = (char**) malloc(MAX_MODULES*sizeof(char*));
	
	// create file to open
	char reg_file[strlen(LIBLICENSE_IO_MODULE_DIR)+strlen("registry")+1];
	reg_file[0]='\0';
	strcat(reg_file,LIBLICENSE_IO_MODULE_DIR);
	strcat(reg_file,"registry");
	
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
	  char* line = (char*) malloc(MAX_LINE_LENGTH*sizeof(char));
		lines[i] = fgets(line, MAX_LINE_LENGTH, registry);
	}
	// shrink string lengths and result array
	module_t* result = (module_t*) malloc((i+1)*sizeof(module_t));
	result[i]=NULL;
	i=0;
	while(lines[i]!=NULL) {
		result[i]=strdup(lines[i]);
		free(lines[i]);
		i++;
	}
	free(lines);
	return result;
}

int ll_module_init(char* directory,module_t m) {// create file to open
	char reg_file[strlen(directory)+strlen(m)+1];
	reg_file[0]='\0';
	strcat(reg_file,directory);
	strcat(reg_file,m);
	void *handle = dlopen(reg_file,RTLD_LAZY);

	void (*function_init)();
	*(void**) (&function_init) = dlsym(handle,"init");
	(*function_init)();

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

// Config Module functions.
int ll_module_in_use(module_t m) {
	return true;
}

// IO module functions.
mime_type_t* ll_module_mime_types(module_t m) {
		// temp data structure
	char** types = (char**) malloc(MAX_MODULES*sizeof(char*));
	types[0]=NULL;
	
	// create file to open
	char reg_file[strlen(LIBLICENSE_IO_MODULE_DIR)+strlen("registry")+1];
	reg_file[0]='\0';
	strcat(reg_file,LIBLICENSE_IO_MODULE_DIR);
	strcat(reg_file,"registry");
	
	// open file
	FILE* registry = fopen(reg_file,"r");
	
	// process lines
	int stop = false;
	int i = 0;
	char* line = (char*) malloc(MAX_LINE_LENGTH*sizeof(char));
	while(line!=NULL && !stop) {
		line = fgets(line, MAX_LINE_LENGTH, registry);
		// reduce down to first token on line
		char* start=strtok(line," \n");
		if (strcmp(start,m)==0) {
			stop=true;
			char* last_type="bleh";
			while(last_type!=NULL) {
				last_type = strtok(NULL," \n");
				fflush(stdout);
				types[i] = last_type;
				i++;
			}
		}
	}
	
	// shrink string lengths and result array
	module_t* result = (module_t*) malloc((i)*sizeof(module_t));
	result[i-1]=NULL;
	i=0;
	while(types[i]!=NULL) {
		result[i]=strdup(types[i]);
		i++;
	}
	return result;
}
