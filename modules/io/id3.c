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

#include <liblicense.h>

#include <id3.h>

#define MAX_URI_LENGTH 50

void id3_init()
{
}

char* id3_read( const char* filename )
{
	ID3Tag *tag = ID3Tag_New();
	ID3Tag_Link(tag,filename);

	char *buffer = NULL;

	ID3Frame *frame = ID3Tag_FindFrameWithID(tag, ID3FID_WWWCOPYRIGHT);
	if (frame) {
		ID3Field *field = ID3Frame_GetField(frame,ID3FN_URL);

		buffer = malloc(sizeof(char)*(MAX_URI_LENGTH+1));
		ID3Field_GetASCII(field,buffer,MAX_URI_LENGTH);
	}

	ID3Tag_Delete(tag);
	return buffer;
}

int id3_write( const char* filename, const char* uri )
{
	ID3Tag *tag = ID3Tag_New();
	ID3Tag_Link(tag,filename);

	ID3Frame *frame = ID3Tag_FindFrameWithID(tag, ID3FID_WWWCOPYRIGHT);
	if (!frame) {
		frame = ID3Frame_NewID(ID3FID_WWWCOPYRIGHT);
		ID3Tag_AddFrame(tag, frame);
	}

	ID3Field *field = ID3Frame_GetField(frame, ID3FN_URL);
	ID3Field_SetASCII(field, uri);

	int err = ID3Tag_Update(tag);
	ID3Tag_Delete(tag);
	return err;
}

LL_MODULE_DEFINE("id3.so","Write licenses within ID3 tags.","0.1",
  LL_FEATURES_EMBED,
  "audio/mpeg",
  id3_init,id3_read,id3_write);
