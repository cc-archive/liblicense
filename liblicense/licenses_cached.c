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
 * Copyright 2007, Scott Shawcroft.
 * Copyright (C) 2007 Peter Miller
 */

#include "liblicense.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>
#include <raptor.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

const char * liblicense_license_dir = NULL;

static sqlite3 *db;

static int _ll_sql_callback(void* list,int argc,char**argv,char**colNames) {
        ll_uri_t* results;

	(void)argc;
	(void)colNames;
	results = (ll_uri_t*) list;
	if (results!=NULL) {
		if (argv[0]) {
			results[ll_list_length(results)]=strdup(argv[0]);
		} else {
			results[ll_list_length(results)]=strdup("");
		}
	}
	return 0;
}

static ll_uri_t* _ll_query(char* query, int max) {
	char *zErrMsg = NULL;
	ll_uri_t* values = ll_new_list(max);
	int rc = sqlite3_exec(db, query, _ll_sql_callback, values, &zErrMsg);
	if (rc!=SQLITE_OK) {
		printf("Query \"%s\" failed: %s\n",query,zErrMsg);
		ll_free_list(values);
		return NULL;
	}
	return values;
}
// helper which returns whether a file ands in .rdf
static int _ll_rdf_filter(const struct dirent * d) {
	return strstr(d->d_name,".rdf")!=NULL;
}
static char* _ll_cache_time_filename(void) {
    struct stat sb;
	char* home = getenv("HOME");
    char* path = (char*) calloc((strlen(home)+strlen("/.license/last_cache.time")+1),sizeof(char));
    path[0] = '\0';
    strcat(path,home);
    strcat(path,"/.license/");
    /* make directory as needed*/
    if (stat(path, &sb) == -1) {
        if (mkdir(path,(S_IRWXU | S_IRGRP | S_IROTH))==-1) {
            fprintf(stderr,"Failed to make directory.\n");
            return NULL;
        }
    } else if ((sb.st_mode & S_IFMT) != S_IFDIR) {
        fprintf(stderr,"File exists and is not a directory: %s\n",path);
        return NULL;
    }
    strcat(path,"last_cache.time");
    return path;
}
static int _ll_last_cache_time(void) {
	FILE* file;
    char tmp[15] = "";
	time_t t;
	char* path = _ll_cache_time_filename();
	if (path == NULL) {
        free(path);
        return 0;
    }
    file = fopen(path,"r");
    free(path);
    if (file==NULL)
        return 0;
    fgets(tmp,15,file);
    fclose(file);
    t = (time_t) atoi(tmp);
    return t;
}

int _ll_update_cache_time(void) {
	FILE* file;
	char* path = _ll_cache_time_filename();
	if (path == NULL) {
        free(path);
        return false;
    }
    file = fopen(path,"w");
    free(path);
    fprintf(file,"%d",(int) time(NULL));
    fclose(file);
    return true;
}

int ll_update_cache(void) {
	int i;
	int n;
	time_t last_cache;
	struct dirent **namelist = NULL;
	/* check last update time versus license file update */

	/* create table if it doesn't exist. */
	char *zErrMsg = NULL;
	int rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS licenses( uri TEXT UNIQUE ON CONFLICT IGNORE, jurisdiction TEXT, obsolete INTEGER);" , _ll_sql_callback, 0, &zErrMsg);
	if (rc!=SQLITE_OK) {
		printf("Table creation failed: %s\n",zErrMsg);
		return false;
	}
	/* insert newly modified files */
	n = scandir(liblicense_license_dir, &namelist, _ll_rdf_filter, alphasort);
	last_cache = _ll_last_cache_time();
	for (i=0;i<n;++i) {
		ll_uri_t u = ll_filename_to_uri(namelist[i]->d_name);
		ll_filename_t fn = ll_uri_to_filename(u);
		struct stat fileinfo;
		if (stat(fn,&fileinfo)==-1) {
			printf("error failed to stat %s, skipping\n",fn);
			free(u);
			free(fn);
			free(namelist[i]);
			continue;
		}
		if (fileinfo.st_mtime>last_cache) {
			char* query;
			ll_uri_t* list;
			/* Get data */
			ll_juris_t j = ll_get_first(ll_get_attribute(u, LL_JURISDICTION, false));
			ll_uri_t *successor = ll_get_attribute(u, LL_REPLACED_BY,0);
			int obsolete = ll_list_length(successor);
			ll_free_list(successor);
			/* build insert query */
			if (j!=NULL){
				size_t buf_size = sizeof(char)*(strlen("INSERT INTO licenses VALUES ('','',)")+strlen(u)+strlen(j)+2);
				query = (char*) malloc(buf_size);
				snprintf(query,buf_size,"INSERT INTO licenses VALUES ('%s','%s',%d)",u,j,obsolete);
			} else {
				size_t buf_size = sizeof(char)*(strlen("INSERT INTO licenses VALUES ('',NULL,)")+strlen(u)+2);
				query = (char*) malloc(buf_size);
				snprintf(query,buf_size,"INSERT INTO licenses VALUES ('%s',NULL,%d)",u,obsolete);
			}
			list = _ll_query(query,0);
			ll_free_list(list);
			if(j!=NULL)
				free(j);
			free(query);
		}
		free(u);
		free(fn);
		free(namelist[i]);
	}
	free(namelist);
	_ll_update_cache_time();
	return true;
}

// initializes the library and its dependencies.
int ll_init(void) {
	char* home;
	char* path;
    struct stat sb;

	raptor_init();
	setlocale(LC_ALL,"");

	liblicense_license_dir = getenv("LL_LICENSE_DIR");
	if(liblicense_license_dir == NULL) {
		liblicense_license_dir = LICENSE_DIR;
	}
	/* get the filename for the cache */
	home = getenv("HOME");
    path = (char*) calloc((strlen(home)+strlen("/.license/license_cache.db")+1),sizeof(char));
    path[0] = '\0';
    strcat(path,home);
    strcat(path,"/.license/");
    /* make directory as needed*/
    if (stat(path, &sb) == -1) {
        if (mkdir(path,(S_IRWXU | S_IRGRP | S_IROTH))==-1) {
            fprintf(stderr,"Failed to make directory.\n");
            return 1;
        }
    } else if ((sb.st_mode & S_IFMT) != S_IFDIR) {
        fprintf(stderr,"File exists and is not a directory: %s\n",path);
        return 1;
    }
    strcat(path,"license_cache.db");

    sqlite3_open(path, &db);
	if (!ll_update_cache())
	    printf("Failed to update cache.");
	ll_init_modules();
	free(path);
	return 0;
}

// stops the library and its dependencies.
int ll_stop(void) {
	raptor_finish();
	ll_stop_modules();
	sqlite3_close(db);
	return 0;
}

// returns a null-terminated list of all general licenses available.
ll_uri_t* ll_get_all_licenses(void) {
  ll_uri_t* result = _ll_query("SELECT uri FROM licenses LIMIT 500;",500);
  return result;
}

// returns a null-terminated list of all general licenses in a family.
ll_uri_t* ll_get_licenses(const ll_juris_t _j) {
	ll_juris_t j = _j;
	ll_uri_t* result;
	char* query;
	if (j && strcmp(j,"unported") == 0) 
		j = NULL;

	if (j!=NULL){
		size_t buf_size = sizeof(char)*(strlen("SELECT uri FROM licenses WHERE jurisdiction='' AND obsolete=0 LIMIT 15")+strlen(j)+1);
		query = (char*) malloc(buf_size);
		snprintf(query,buf_size,"SELECT uri FROM licenses WHERE jurisdiction='%s' AND obsolete=0 LIMIT 15",j);
	} else {
		query = strdup("SELECT uri FROM licenses WHERE jurisdiction ISNULL AND obsolete=0 LIMIT 15");
	}
	result = _ll_query(query,15);
	free(query);
  	return result;
}

// returns a null-terminated list of all jurisdictions in use
ll_juris_t* ll_get_jurisdictions(void) {
	ll_juris_t* result = _ll_query("SELECT DISTINCT(jurisdiction) FROM licenses ORDER BY jurisdiction LIMIT 50",50);

	int i;
	int len = ll_list_length(result);
	for (i=0; i<len; ++i) {
		if (strcmp(result[i],"") == 0) {
			free(result[i]);
			result[i] = strdup("unported");
		}
	}

	return result;
}

// returns whether or not the given uri is recognized by the system.
int ll_verify_uri(const ll_uri_t u) {
	ll_uri_t* licenses = ll_get_all_licenses();
	int result = ll_list_contains(licenses,u);
	ll_free_list(licenses);
	return result;
}
