#include <stdio.h>

#include <exempi/xmp.h>

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
}

void shutdown()
{
	printf("exempi: shutdown()\n");
	xmp_terminate();
}

void read( const char* filename )
{
	printf("exempi: read(%s)\n",filename);

	XmpFilePtr f;

	f = xmp_files_open_new(filename, XMP_OPEN_OPNLYXMP);
	XmpPtr xmp = xmp_files_get_new_xmp(f);

	XmpIteratorPtr iter = xmp_iterator_new(xmp, NULL, NULL, XMP_ITER_JUSTLEAFNODES);
	XmpStringPtr the_schema = xmp_string_new();
	XmpStringPtr the_path = xmp_string_new();
	XmpStringPtr the_prop = xmp_string_new();

	while( xmp_iterator_next(iter, the_schema, the_path, the_prop, NULL) )
	{
		const char *schema = xmp_string_cstr(the_schema);
		const char *name = xmp_string_cstr(the_path);
		const char *value = xmp_string_cstr(the_prop);
		printf("============================================\n");
		printf("name: %s\n", name);
		printf("value: %s\n", value);
		printf("schema: %s\n", schema);
		printf("============================================\n");
	}

	xmp_string_free(the_prop);
	xmp_string_free(the_path);
	xmp_string_free(the_schema);
	xmp_iterator_free(iter);

	xmp_free(xmp);

	xmp_files_close(f, XMP_CLOSE_SAFEUPDATE);
}

void write( const char* filename, const char* uri )
{
	printf("exempi: write(%s,%s)\n",filename,uri);
}
