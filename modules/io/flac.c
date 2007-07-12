/* Creative Commons has made the contents of this file
 * available under a CC-GNU-GPL license:
 *
 * http: *creativecommons.org/licenses/GPL/2.0/
 *
 * A copy of the full license can be found as part of this
 * distribution in the file COPYING.
 * 
 * You may use the liblicense software in accordance with the
 * terms of that license. You agree that you are solely 
 * responsible for your use of the liblicense software and you
 * represent and warrant to Creative Commons that your use
 * of the liblicense software will comply with the CC-GNU-GPL.
 *
 * Copyright 2007, Creative Commons, www.creativecommons.org.
 * Copyright 2007, Jason Kivlighn.
 */

#include <stdio.h>
#include <string.h>

#include <FLAC/metadata.h>
#include <FLAC/format.h>

void init()
{
}

void shutdown()
{
}

char* read( const char* filename )
{
	char *license = NULL;

	FLAC__StreamMetadata *vc;
	if ( FLAC__metadata_get_tags(filename,&vc) ) {
		int index = FLAC__metadata_object_vorbiscomment_find_entry_from(vc, 0, "LICENSE");
		if (index != -1) {
			license = strdup((char*)&vc->data.vorbis_comment.comments[index].entry[8]);
		}
		FLAC__metadata_object_delete(vc);
	}

	return license;
}

int write( const char* filename, const char* uri )
{
	int ret = 1;

	FLAC__Metadata_SimpleIterator *iter = FLAC__metadata_simple_iterator_new();
	if (FLAC__metadata_simple_iterator_init(iter,filename,false,false) && FLAC__metadata_simple_iterator_is_writable(iter)) {
		FLAC__bool found_vc = false;
		do {
			if (FLAC__metadata_simple_iterator_get_block_type(iter) == FLAC__METADATA_TYPE_VORBIS_COMMENT) {
				found_vc = true;
				FLAC__StreamMetadata *vc = FLAC__metadata_simple_iterator_get_block(iter);

				FLAC__StreamMetadata_VorbisComment_Entry entry;
				FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair(&entry, "LICENSE", uri);
				FLAC__metadata_object_vorbiscomment_replace_comment(vc,entry,true,false);

				ret = FLAC__metadata_simple_iterator_set_block(iter, vc, true);
				FLAC__metadata_object_delete(vc);
				break;
			}
		} while (FLAC__metadata_simple_iterator_next(iter));
		
		if (!found_vc) {
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