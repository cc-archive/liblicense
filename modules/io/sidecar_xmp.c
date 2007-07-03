#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

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

void init()
{
	printf("sidecar_xmp: init()\n");
	xmp_init();
	xmp_register_namespace(NS_CC, "cc", NULL);
}

void shutdown()
{
	printf("sidecar_xmp: shutdown()\n");
	xmp_terminate();
}

char* read( const char* filename )
{
	printf("sidecar_xmp: read(%s)\n",filename);

	char *sidecar = sidecar_filename( filename );

	FILE *f = fopen(sidecar, "rb");
	if ( f ) {
		char *buffer;
		size_t len;
		get_contents_stdio(f,&buffer,&len);

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

	free(sidecar);

	return NULL;
}

int write( const char* filename, const char* uri )
{
	printf("sidecar_xmp: write(%s,%s)\n",filename,uri);

	int success = true;

	char *sidecar = sidecar_filename( filename );

	XmpPtr xmp = NULL;
	FILE *f = fopen(sidecar, "rb");
	if ( f ) {
		char *buffer;
		size_t len;
		get_contents_stdio(f,&buffer,&len); /* closes the file */

		xmp = xmp_new(buffer,len);
	}
	
	if ( !xmp ) {
		xmp = xmp_new_empty();
	}

	xmp_set_property(xmp, NS_CC, "license", uri);

	XmpStringPtr xmp_string = xmp_string_new();
	xmp_to_string(xmp,xmp_string,XMP_SERIAL_OMITPACKETWRAPPER,2);

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
