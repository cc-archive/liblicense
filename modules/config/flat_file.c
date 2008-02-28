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

/* FIXME: Non-namespaced names! */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_URI_LENGTH 100
void init(void) {
}

void shutdown(void) {
}

char* get_filename(void) {
    char* home;
    char* path;
    struct stat sb;

    home = getenv("HOME");
    path = (char*) calloc((strlen(home)+strlen("/.license/default")+1),sizeof(char));
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
    strcat(path,"default");
    return path;
}
int set_default(char* u) {
    char* path;
    FILE* file;
    int result;

    path = get_filename();
    if (path == NULL) {
        free(path);
        return false;
    }
    file = fopen(path,"w");
    free(path);
    result = true;
    if (u!=NULL && u[0]!='\0')
        result = fprintf(file,"%s",u);
    fclose(file);
    return result;
}

char* get_default(void) {
    char* path;
    FILE* file;
    char tmp[MAX_URI_LENGTH];

    path = get_filename();
    if (path == NULL) {
        free(path);
        return NULL;
    }
    file = fopen(path,"r");
    free(path);
    if (file==NULL)
        return strdup("");
    tmp[0] = '\0';
    fgets(tmp,MAX_URI_LENGTH,file);
    fclose(file);
    return strdup(tmp);
}
