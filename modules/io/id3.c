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
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <liblicense.h>

#include <id3.h>

#define MAX_URI_LENGTH 50

static int id3_predicate2field(const char * predicate) {
	if (strcmp(predicate, LL_LICENSE) == 0) {
		return ID3FID_WWWCOPYRIGHT /* wcop */;
	} else if (strcmp(predicate, LL_WEBSTATEMENT) == 0) {
		return ID3FID_WWWAUDIOFILE /* woaf */;
	} else {
		return -LL_E_MODULE_WRITE_FAIL;
	}
}


void id3_init()
{
}

char* id3_read( const char* filename, const ll_uri_t predicate )
{
	int woaf_or_wcop;
	char *buffer = NULL;
	ID3Frame *frame;
	ID3Tag *tag;

	woaf_or_wcop = id3_predicate2field(predicate);
	if (woaf_or_wcop < 0) {
		return NULL;
	}

	tag = ID3Tag_New();
	ID3Tag_Link(tag,filename);

	frame = ID3Tag_FindFrameWithID(tag, woaf_or_wcop);
	if (frame) {
		ID3Field *field = ID3Frame_GetField(frame,ID3FN_URL);

		buffer = malloc(sizeof(char)*(MAX_URI_LENGTH+1));
		ID3Field_GetASCII(field,buffer,MAX_URI_LENGTH);
	}

	ID3Tag_Delete(tag);
	return buffer;
}

int id3_write( const char* filename, const char * predicate, const char* uri )
{
	/* Figure out which frame to use */
	int woaf_or_wcop;
	int err;
	ID3Frame *frame;
	ID3Tag *tag;

	woaf_or_wcop = id3_predicate2field(predicate);
	if (woaf_or_wcop < 0) {
		return -LL_E_MODULE_WRITE_FAIL;
	}

	tag = ID3Tag_New();
	ID3Tag_Link(tag,filename);

	frame = ID3Tag_FindFrameWithID(tag, woaf_or_wcop);

	if ( uri ) {
		ID3Field * field;
		if (!frame) {
			frame = ID3Frame_NewID(woaf_or_wcop);
			ID3Tag_AttachFrame(tag, frame);
		}
	
		field = ID3Frame_GetField(frame, ID3FN_URL);
		ID3Field_SetASCII(field, uri);
	} else {
		if (frame) {
			ID3Tag_RemoveFrame(tag,frame);
		}
	}

	err = !ID3Tag_Update(tag);

	ID3Tag_Delete(tag);
	return err;
}

const char * id3_supported_predicates[] = {LL_LICENSE, LL_WEBSTATEMENT, NULL};
const char * id3_mime_types[] = {"audio/mpeg",
				 NULL};

LL_MODULE_DEFINE("id3.so",
		 "Write licenses within ID3 tags.",
		 "0.1",
		 LL_FEATURES_EMBED,
		 id3_supported_predicates,
		 id3_mime_types,
		 id3_init,
		 id3_read,
		 id3_write,
		 NULL /* no shutdown */);
