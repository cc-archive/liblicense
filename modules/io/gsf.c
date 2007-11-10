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

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <liblicense.h>

#include <glib.h>
#include <glib-object.h>

#include <gsf/gsf.h>
#include <gsf/gsf-msole-utils.h>
#include <gsf/gsf-doc-meta-data.h>
#include <gsf/gsf-utils.h>

#include <gsf/gsf-input-stdio.h>
#include <gsf/gsf-infile.h>
#include <gsf/gsf-infile-msole.h>

#include <gsf/gsf-output-stdio.h>
#include <gsf/gsf-outfile.h>
#include <gsf/gsf-outfile-msole.h>

#include <stdio.h>

void _gsf_init()
{
	gsf_init();
}

char* gsf_read( const char* filename )
{
	char *license = NULL;

	GsfInput	*input;
	GsfInfile	*infile;
	GsfInput	*stream;
	GsfDocMetaData	*md;

	input = gsf_input_stdio_new (filename, NULL);

	if (!input) {
		return NULL;
	}

	infile = gsf_infile_msole_new (input, NULL);
	g_object_unref (G_OBJECT (input));

	if (!infile) {
		return NULL;
	}

	stream = gsf_infile_child_by_name (infile, "\05DocumentSummaryInformation");
	if (stream) {
		GsfDocProp * prop;
		md = gsf_doc_meta_data_new();
	
		if (gsf_msole_metadata_read (stream, md)) {
			return NULL;
		}

		prop = gsf_doc_meta_data_lookup(md,"CreativeCommons_LicenseURL");
		if (prop) {
			license = g_value_dup_string(gsf_doc_prop_get_val(prop));
		}
	
		g_object_unref (G_OBJECT (md));
		g_object_unref (G_OBJECT (stream));
	}

	g_object_unref (G_OBJECT (infile));

	return license;
}


static void clone_dir (GsfInfile *in, GsfOutfile *out, const char *uri);

static void
clone (GsfInput *input, GsfOutput *output, const char *uri)
{
	if (gsf_input_size (input) > 0) {
		guint8 const *data;
		size_t len;

		while ((len = gsf_input_remaining (input)) > 0) {
			/* copy in odd sized chunks to exercise system */
			if (len > 314)
				len = 314;
			if (NULL == (data = gsf_input_read (input, len, NULL))) {
				g_warning ("error reading ?");
				return;
			}
			if (!gsf_output_write (output, len, data)) {
				g_warning ("error writing ?");
				return;
			}
		}
	} else if (GSF_IS_INFILE(input)) {
		clone_dir (GSF_INFILE(input), GSF_OUTFILE(output), uri);
	}

	gsf_output_close (output);
	g_object_unref (G_OBJECT (output));
	g_object_unref (G_OBJECT (input));
}

static void
clone_dir (GsfInfile *in, GsfOutfile *out, const char *uri)
{
	GsfInput *new_input;
	GsfOutput *new_output;
	gboolean is_dir;
	const gchar *name;
	int i;

	for (i = 0 ; i < gsf_infile_num_children (in) ; i++) {
		new_input = gsf_infile_child_by_index (in, i);

		/* In theory, if new_file is a regular file (not directory),
		 * it should be GsfInput only, not GsfInfile, as it is not
		 * structured.  However, having each Infile define a 2nd class
		 * that only inherited from Input was cumbersome.  So in
		 * practice, the convention is that new_input is always
		 * GsfInfile, but regular file is distinguished by having -1
		 * children.
		 */
		is_dir = GSF_IS_INFILE (new_input) &&
			gsf_infile_num_children (GSF_INFILE (new_input)) >= 0;

		name = gsf_infile_name_by_index(in, i);
		new_output = gsf_outfile_new_child  (out,
				name,
				is_dir);

		if (strcmp(name,"\05DocumentSummaryInformation") == 0) {
			GsfDocMetaData *md = gsf_doc_meta_data_new();
			gsf_msole_metadata_read(new_input, md);
			
			if (uri) {
				GValue *uri_value = g_value_init(g_new0(GValue,1),G_TYPE_STRING);
				g_value_set_string(uri_value,uri);
				gsf_doc_meta_data_insert(md,strdup("CreativeCommons_LicenseURL"),uri_value);	
			} else {
				gsf_doc_meta_data_remove(md,"CreativeCommons_LicenseURL");
			}
			gsf_msole_metadata_write(new_output,md,true);
		
			g_object_unref (G_OBJECT (md));
		} else {
			clone (new_input, new_output, uri);
		}
	}
	/* An observation: when you think about the explanation to is_dir
	 * above, you realize that clone_dir is called even for regular files.
	 * But nothing bad happens, as the loop is never entered.
	 */
}

int gsf_write( const char* filename, const char* uri )
{
	GsfInput   *input;
	GsfInfile  *infile;
	GsfOutput  *output;
	GsfOutfile *outfile;
	GError    *err = NULL;

	input = gsf_input_stdio_new (filename, &err);
	if (input == NULL) {
		g_return_val_if_fail (err != NULL, 1);

		g_warning ("'%s' error: %s", filename, err->message);
		g_error_free (err);
		return TRUE;
	}

	infile = gsf_infile_msole_new (input, &err);
	g_object_unref (G_OBJECT (input));

	if (infile == NULL) {
		g_return_val_if_fail (err != NULL, 1);

		g_warning ("'%s' Not an OLE file: %s", filename, err->message);
		g_error_free (err);
		return FALSE;
	}

	output = gsf_output_stdio_new (filename, &err);
	if (output == NULL) {
		g_return_val_if_fail (err != NULL, 1);

		g_warning ("'%s' error: %s", filename, err->message);
		g_error_free (err);
		g_object_unref (G_OBJECT (infile));
		return FALSE;
	}

	outfile = gsf_outfile_msole_new (output);
	g_object_unref (G_OBJECT (output));
	clone (GSF_INPUT (infile), GSF_OUTPUT (outfile), uri);

	return TRUE;
}

LL_MODULE_DEFINE("gsf.so","Embeds licenses in MS Office (pre-2007).","0.1",
  LL_FEATURES_EMBED,
  "application/msword",
  _gsf_init,gsf_read,gsf_write);
