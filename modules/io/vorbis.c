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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if HAVE_STAT && HAVE_CHMOD
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include <liblicense.h>

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#include "vcedit.h"

void vorbis_init()
{
}

char* vorbis_read( const char* filename )
{
	OggVorbis_File vf;
	FILE *fh;
	char *license;
	char **comments;

	fh = fopen(filename,"r");
	if (fh) {
		if (ov_open(fh, &vf, NULL, 0) < 0) {
			fprintf(stderr,"Input does not appear to be an Ogg bitstream.\n");
			return NULL;
		}
	} else {
		fprintf(stderr,"Unable to open file for reading.\n");
		return NULL;
	}

	license = NULL;
	comments = ov_comment(&vf,-1)->user_comments;
	for (; *comments; ++comments ) {
		if (strncmp(*comments,"LICENSE=",8) == 0) {
			license = strdup(&(*comments)[8]);
			break;
		}
	}

	ov_clear(&vf);

	return license;
}

int vorbis_write( const char* filename, const char* uri )
{
	int ret = 0;

	FILE *fh_in = fopen(filename,"rb");
	if (fh_in) {
		vcedit_state *state = vcedit_new_state();
		if (vcedit_open(state, fh_in) < 0) {
			fprintf(stderr,"Input does not appear to be an Ogg bitstream.\n");
		} else {
			char *outfilename;
			FILE *fh_out;

			outfilename = malloc(strlen(filename)+8);
			strcpy(outfilename, filename);
			strcat(outfilename, ".vctemp");
			fh_out = fopen(outfilename,"wb");
			if (fh_out) {
				vorbis_comment *vc;
				char **comments;
				int output_written;

				vorbis_comment vc_new;
				vorbis_comment_init(&vc_new);

				vc = vcedit_comments(state);
				comments = vc->user_comments;
				for (; *comments; ++comments ) {
					if (strncmp(*comments,"LICENSE=",8) != 0) {
						vorbis_comment_add(&vc_new,*comments);
					}
				}

				if (uri) {
                                        /*
                                         * The vorbis prototypes need
                                         * const correctness work.
                                         */
					vorbis_comment_add_tag(&vc_new, "LICENSE", (char *)uri);
				}

				vorbis_comment_clear(vc);
				vorbis_comment_init(vc);
				comments = vc_new.user_comments;
				for (; *comments; ++comments ) {
					vorbis_comment_add(vc,*comments);
				}

				output_written = !vcedit_write(state, fh_out);
				fclose(fh_out);

				#if HAVE_STAT && HAVE_CHMOD
				struct stat st;
				stat (filename, &st);
				#endif

				if(output_written) {
					/* Some platforms fail to rename a file if the new name already
					* exists, so we need to remove, then rename. How stupid.
					*/
					if(rename(outfilename, filename)) {
						if(remove(filename))
							fprintf(stderr, "Error removing old file %s\n", filename);
						else if(rename(outfilename, filename))
							fprintf(stderr, "Error renaming %s to %s\n", outfilename,
							filename);
						else {
							ret = 1;
						}
					} else {
						ret = 1;
						#if HAVE_STAT && HAVE_CHMOD
						chmod (filename, st.st_mode);
						#endif
					}
				}
				else {
					if(remove(outfilename)) {
						fprintf(stderr, "Error removing erroneous temporary file %s\n",
							outfilename);
					}
				}
			} else {
				fprintf(stderr,"Unable to open file for writing.\n");
			}
			free(outfilename);
		}

		vcedit_clear(state);
		fclose(fh_in);
	} else {
		fprintf(stderr,"Unable to open file for reading.\n");
	}

	return ret;
}

const char * vorbis_supported_predicates[] = {NULL};
const char * vorbis_mime_types = {"audio/x-vorbis+ogg",
				  "audio/x-vorbis",
				  "application/ogg",
				  NULL};
  

LL_MODULE_DEFINE("vorbis.so",
		 "Write licenses in Vorbiscomments within an OGG stream.",
		 "0.1",
		 LL_FEATURES_EMBED,
		 vorbis_supported_predicates,
		 vorbis_mime_types,
		 vorbis_init,
		 vorbis_read,
		 vorbis_write,
		 NULL /* no shutdown */);
