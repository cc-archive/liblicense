#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <exempi/xmp.h>
#include <exempi/xmpconsts.h>

const char *mime_types[] = {
  "image/jpeg",
  "image/png",
  "image/tiff",
  "application/pdf",
  "video/x-msvideo",
  "video/quicktime",
  0
};

void init()
{
	printf("exempi: init()\n");
	xmp_init();
	xmp_register_namespace(NS_CC, "cc", NULL);
}

void shutdown()
{
	printf("exempi: shutdown()\n");
	xmp_terminate();
}

char* read( const char* filename )
{
	printf("exempi: read(%s)\n",filename);

	XmpFilePtr f;

	f = xmp_files_open_new(filename, XMP_OPEN_OPNLYXMP);
	XmpPtr xmp = xmp_files_get_new_xmp(f);

	char *uri_string = NULL;

	if ( xmp ) {
		XmpStringPtr license_uri = xmp_string_new();
		if ( xmp_get_property(xmp, NS_CC, "license", license_uri) ) {
			uri_string = strdup(xmp_string_cstr(license_uri));
		}

		xmp_string_free(license_uri);
	
		xmp_free(xmp);
	}

	xmp_files_close(f, XMP_CLOSE_NOOPTION);

	return uri_string;
}

int write( const char* filename, const char* uri )
{
	printf("exempi: write(%s,%s)\n",filename,uri);

	int success = true;

	XmpFilePtr f;
	
	f = xmp_files_open_new(filename, XMP_OPEN_FORUPDATE | XMP_OPEN_OPNLYXMP);
	XmpPtr xmp = xmp_files_get_new_xmp(f);
	
	if ( xmp == NULL ) {
		xmp = xmp_new_empty();
	}
	
	if ( xmp_files_can_put_xmp(f, xmp) ) {
		xmp_set_property(xmp, NS_CC, "license", uri);
		xmp_files_put_xmp(f, xmp);
	} else {
		fprintf(stderr,"Unable to write XMP to this file.\n");
		success = false;
	}
	
	xmp_files_close(f, XMP_CLOSE_SAFEUPDATE);
	xmp_free(xmp);

	return success;
}
