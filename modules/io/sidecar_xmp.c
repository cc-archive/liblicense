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
// Copyright 2007, Jason Kivlighn.

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include <liblicense.h>

#include <exempi/xmp.h>
#include <exempi/xmpconsts.h>

#define MIN(a,b) (((a) < (b)) ? (a) : (b))

char *sidecar_filename( const char *filename )
{
	const char *base_sep = strrchr(filename,'/');
	if ( !base_sep )
		base_sep = filename;

	const char *ext_sep = strrchr(filename,'.');
	if ( !ext_sep ) {
		ext_sep = &filename[strlen(filename)];
	}
	size_t len = ext_sep - filename;

	char *sidecar = (char*)malloc(sizeof(char)*(len+4+1));
	strncpy(sidecar,filename,len);
	strcpy(&sidecar[len],".xmp");

	return sidecar;
}

/* Adapted from glib */
bool
get_contents_stdio (FILE        *f,
                    char      **contents,
                    size_t       *length)
{
	char buf[4096];
	size_t bytes;
	char *str = NULL;
	size_t total_bytes = 0;
	size_t total_allocated = 0;
	char *tmp;

	while (!feof (f))
	{
		int save_errno;

		bytes = fread (buf, 1, sizeof (buf), f);
		save_errno = errno;

		while ((total_bytes + bytes + 1) > total_allocated)
		{
			if (str)
				total_allocated *= 2;
			else
				total_allocated = MIN (bytes + 1, sizeof (buf));

			tmp = realloc(str, total_allocated);

			if (tmp == NULL)
				goto error;

			str = tmp;
		}

		if (ferror (f))
			goto error;

		memcpy (str + total_bytes, buf, bytes);
		total_bytes += bytes;
	}

	fclose (f);

	if (total_allocated == 0)
		str = (char*)malloc(sizeof(char));

	str[total_bytes] = '\0';

	if (length)
		*length = total_bytes;

	*contents = str;

	return true;

	error:
		free (str);
		fclose (f);
	
		return false;
}

void sidecar_xmp_init()
{
	xmp_init();
	xmp_register_namespace(NS_CC, "cc", NULL);
}

char* sidecar_xmp_read( const char* filename )
{
	char *sidecar = sidecar_filename( filename );
	FILE *f = fopen(sidecar, "rb");
	free(sidecar);

	if ( f ) {
		char *buffer;
		size_t len;
		if ( get_contents_stdio(f,&buffer,&len) ) {
			XmpPtr xmp = xmp_new(buffer,len);
			free(buffer);
	
			char *uri_string = NULL;
			XmpStringPtr license_uri = xmp_string_new();
			if ( xmp_get_property(xmp, NS_CC, "license", license_uri) ) {
				uri_string = strdup(xmp_string_cstr(license_uri));
			}
	
			xmp_string_free(license_uri);
			xmp_free(xmp);
			return uri_string;
		}
	}

	return NULL;
}

int sidecar_xmp_write( const char* filename, const char* uri )
{
	printf("writing %s %s\n",filename,uri);

	int success = true;

	char *sidecar = sidecar_filename( filename );

	XmpPtr xmp = NULL;
	FILE *f = fopen(sidecar, "rb");
	if ( f ) {
		char *buffer;
		size_t len;
		if ( get_contents_stdio(f,&buffer,&len) ) { /* closes the file */
			xmp = xmp_new(buffer,len);
			free(buffer);
		}
	}
	
	if ( !xmp ) {
		xmp = xmp_new_empty();
	}

	xmp_set_property(xmp, NS_CC, "license", uri);

	XmpStringPtr xmp_string = xmp_string_new();
	xmp_serialize(xmp,xmp_string,XMP_SERIAL_OMITPACKETWRAPPER,2);

	const char *xmp_cstr = xmp_string_cstr(xmp_string);
	f = fopen(sidecar, "w");
	if (f) {
		fprintf(f,xmp_cstr);
		fclose(f);
	} else {
		fprintf(stderr, "Can't open sidecar for writing\n");
		success = false;
	}

	free(sidecar);
	xmp_string_free(xmp_string);
	xmp_free(xmp);

	return success;
}

LL_MODULE_DEFINE("sidecar_xmp.so","Write licenses in sidecar XMP files.","0.1",
  LL_FEATURES_NONE,
  NULL,
  sidecar_xmp_init,sidecar_xmp_read,sidecar_xmp_write);
