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

#include <string.h>

#include <liblicense.h>

#include <fileref.h>

#include <mpcfile.h>
#include <vorbisfile.h>
#include <oggflacfile.h>
#include <flacfile.h>

#include <apetag.h>
#include <xiphcomment.h>

using namespace TagLib;

void taglib_init()
{
}


char* readXiphComment( Ogg::XiphComment *tag )
{
	if (tag) {
		const Ogg::FieldListMap fieldListMap =tag->fieldListMap();
		Ogg::FieldListMap::ConstIterator it = fieldListMap.find("LICENSE");
		if (it != fieldListMap.end()) {
			return strdup(it->second.front().toCString());
		}
	}

	return NULL;
}

char* taglib_read( const char* filename , const char * predicate)
{
	FileRef fileref(filename, false);

	File *file = fileref.file();
	if (file) {
		if (dynamic_cast<MPC::File*>(file)) {
			APE::Tag *tag = ((MPC::File*)(file))->APETag();
			if (tag) {
				const APE::ItemListMap itemListMap = tag->itemListMap();
				APE::ItemListMap::ConstIterator it = itemListMap.find("LICENSE");
				if (it != itemListMap.end()) {
					return strdup(it->second.value().data());
				}
			}
		} else if (dynamic_cast<Ogg::Vorbis::File*>(file)) {
			Ogg::XiphComment *tag = ((Ogg::Vorbis::File*)(file))->tag();
			return readXiphComment(tag);
		} else if (dynamic_cast<FLAC::File*>(file)) {
			Ogg::XiphComment *tag = ((FLAC::File*)(file))->xiphComment();
			return readXiphComment(tag);
		} else if (dynamic_cast<Ogg::FLAC::File*>(file)) {
			Ogg::XiphComment *tag = ((Ogg::FLAC::File*)(file))->tag();
			return readXiphComment(tag);
		}
	}

	return NULL;
}

int taglib_write( const char* filename, const char * predicate, const char* uri )
{
	FileRef fileref(filename, false);
	File *file = fileref.file();
	if (file) {
		if (dynamic_cast<MPC::File*>(file)) {
			APE::Tag *tag = ((MPC::File*)(file))->APETag(uri!=NULL);
			if (uri) {
				tag->addValue("LICENSE",uri);
			} else if (tag) {
				tag->removeItem("LICENSE");
			}
			return fileref.save();
		} else if (dynamic_cast<Ogg::Vorbis::File*>(file)) {
			Ogg::XiphComment *tag = ((Ogg::Vorbis::File*)(file))->tag();
			if (uri) {
				tag->addField("LICENSE",uri,true);
			} else if (tag) {
				tag->removeField("LICENSE");
			}
			return fileref.save();
		} else if (dynamic_cast<FLAC::File*>(file)) {
			Ogg::XiphComment *tag = ((FLAC::File*)(file))->xiphComment(uri!=NULL);
			if (uri) {
				tag->addField("LICENSE",uri,true);
			} else if (tag) {
				tag->removeField("LICENSE");
			}
			return fileref.save();
		} else if (dynamic_cast<Ogg::FLAC::File*>(file)) {
			Ogg::XiphComment *tag = ((Ogg::FLAC::File*)(file))->tag();
			if (uri) {
				tag->addField("LICENSE",uri,true);
			} else if (tag) {
				tag->removeField("LICENSE");
			}
			return fileref.save();
		}
	}

	return 0;
}

const char * taglib_supported_predicates[] = {LL_LICENSE,
};
const char * taglib_mime_types[] = {"audio/x-musepack",
				    "audio/x-vorbis+ogg",
				    "application/ogg",
				    "audio/x-flac", NULL};

void taglib_shutdown()
{
  // Do nothing!
}

LL_MODULE_DEFINE("taglib.so", // name
		 "Embeds licenses in tags of formats supported by TagLib.", // description
		 "0.1", // version
		 LL_FEATURES_EMBED, // features
		 taglib_supported_predicates, // supported predicates
		 taglib_mime_types, // supported MIME types
		 taglib_init,
		 taglib_read,
		 taglib_write,
		 taglib_shutdown);
