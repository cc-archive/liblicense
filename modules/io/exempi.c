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
// Copyright (C) 2007 Peter Miller

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <liblicense.h>

#include <exempi/xmp.h>
#include <exempi/xmpconsts.h>

void exempi_init()
{
	xmp_init();
	xmp_register_namespace(NS_CC, "cc", NULL);
}


void exempi_shutdown()
{
	xmp_terminate();
}

/* this is a work around a bad bug in exempi 1.99.8 
 * should be retro compatible. Remove when you require
 * exempi 1.99.8 and up
 */
#ifndef XMP_OPEN_ONLYXMP
#define XMP_OPEN_ONLYXMP XMP_OPEN_OPNLYXMP
#endif

char* exempi_read( const char* filename )
{
	XmpFilePtr f;
	XmpPtr xmp;
	char *uri_string;

	f = xmp_files_open_new(filename, XMP_OPEN_ONLYXMP);
	if (f == NULL) {
	  // Golly gee whiz, you can't open that file.  xmp_err() or
	  // something like it could tell us why, if only we wanted to
	  // now.  We'll return NULL as allowed in liblicense.h's
	  // doxygen for ll_read().

	  return NULL; // again, magic return value.  Sucks.  FIXME.
	}

	// Otherwise, it's okay to continue.
	  
	xmp = xmp_files_get_new_xmp(f);

	uri_string = NULL;

	if ( xmp ) {
		XmpStringPtr license_uri = xmp_string_new();
		if ( xmp_get_property(xmp, NS_CC, "license", license_uri, NULL) ) {
			uri_string = strdup(xmp_string_cstr(license_uri));
		}

		xmp_string_free(license_uri);

		xmp_free(xmp);
	}

	xmp_files_close(f, XMP_CLOSE_NOOPTION);

	return uri_string;
}

int exempi_write( const char* filename, const char* uri )
{
	int success = true;
	XmpFilePtr f;
	XmpPtr xmp;

	f = xmp_files_open_new(filename, XMP_OPEN_FORUPDATE | XMP_OPEN_ONLYXMP);
	if (f == NULL) {
	  // Then we're doomed.
	  // WTF is with the magic-number return codes?  Maybe it's 1,
	  // maybe it's 0.  I don't remember.
	  
	  // FIXME!
	  return 1;
	}
	xmp = xmp_files_get_new_xmp(f);

	if ( xmp == NULL ) {
		if (uri == NULL) return 1;

		xmp = xmp_new_empty();
	}

	//FIXME: I think exempi needs a xmp_del_property().  For now, set the uri to ""
	// FIXME: It just got one recently, curiously enough!
	if ( uri == NULL ) {
		uri = "";
	}

	if ( xmp_files_can_put_xmp(f, xmp) ) {
		xmp_set_property(xmp, NS_CC, "license", uri, 0);
		xmp_files_put_xmp(f, xmp);
	} else {
		fprintf(stderr,"Unable to write XMP to this file.\n");
		success = false;
	}

	xmp_files_close(f, XMP_CLOSE_SAFEUPDATE);
	xmp_free(xmp);

	return success;
}

LL_MODULE_DEFINE("exempi.so","Embeds licenses in formats recognized by Adobe's SDK.","0.1",
  LL_FEATURES_EMBED,
  "image/jpeg image/png image/tiff application/pdf video/x-msvideo video/quicktime audio/x-wav",
  exempi_init,exempi_read,exempi_write,exempi_shutdown);
