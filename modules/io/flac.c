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
 * Copyright 2007, Jason Kivlighn.
 * Copyright (C) 2007 Peter Miller
 */

#include <stdio.h>
#include <string.h>

#include <liblicense.h>

#include <FLAC/metadata.h>
#include <FLAC/format.h>

void flac_init()
{
}

char* flac_read( const char* filename )
{
	char *license = NULL;

	FLAC__StreamMetadata *vc;
	if ( FLAC__metadata_get_tags(filename,&vc) ) {
		int idx = FLAC__metadata_object_vorbiscomment_find_entry_from(vc, 0, "LICENSE");
		if (idx != -1) {
			license = strdup((char*)&vc->data.vorbis_comment.comments[idx].entry[8]);
		}
		FLAC__metadata_object_delete(vc);
	}

	return license;
}

int flac_write( const char* filename, const char* uri )
{
	int ret = 1;

	FLAC__Metadata_SimpleIterator *iter = FLAC__metadata_simple_iterator_new();
	if (FLAC__metadata_simple_iterator_init(iter,filename,false,false) && FLAC__metadata_simple_iterator_is_writable(iter)) {
		FLAC__bool found_vc = false;
		do {
			if (FLAC__metadata_simple_iterator_get_block_type(iter) == FLAC__METADATA_TYPE_VORBIS_COMMENT) {
				FLAC__StreamMetadata *vc;

				found_vc = true;
				vc = FLAC__metadata_simple_iterator_get_block(iter);

				if (uri) {
					FLAC__StreamMetadata_VorbisComment_Entry entry;
					FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair(&entry, "LICENSE", uri);
					FLAC__metadata_object_vorbiscomment_replace_comment(vc,entry,true,false);
				} else {
					int i = FLAC__metadata_object_vorbiscomment_find_entry_from(vc,0,"LICENSE");
					if (i != -1) {
						FLAC__metadata_object_vorbiscomment_delete_comment(vc,i);
					}
				}

				ret = FLAC__metadata_simple_iterator_set_block(iter, vc, true);
				FLAC__metadata_object_delete(vc);
				break;
			}
		} while (FLAC__metadata_simple_iterator_next(iter));

		if (!found_vc && uri) {
			FLAC__StreamMetadata *vc = FLAC__metadata_object_new(FLAC__METADATA_TYPE_VORBIS_COMMENT);

			FLAC__StreamMetadata_VorbisComment_Entry entry;
			FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair(&entry, "LICENSE", uri);
			FLAC__metadata_object_vorbiscomment_append_comment(vc,entry,false);

			ret = FLAC__metadata_simple_iterator_insert_block_after(iter,vc,true);
			FLAC__metadata_object_delete(vc);
		}
	} else {
		ret = 0;
	}

	FLAC__metadata_simple_iterator_delete(iter);

	return ret;
}

char * flac_supported_predicates[] = {NULL};

LL_MODULE_DEFINE("flac.so","Write licenses in a FLAC comment.","0.1",
  LL_FEATURES_EMBED,
		 flac_supported_predicates,
  "audio/x-flac",
  flac_init,flac_read,flac_write,NULL);
