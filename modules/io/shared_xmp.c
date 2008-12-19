#include <string.h>
#include <stdbool.h>
#include <liblicense.h>
#include <exempi/xmp.h>
#include <exempi/xmpconsts.h>

#include "shared_xmp.h"

const char  _ll_shared_xmp_license[] = "license";
const char  _ll_shared_xmp_webstatement[] = "WebStatement";
const char  _ll_shared_xmp_morepermissions[] = "morePermissions";
const char  LL_NS_PURL_DC_ELEMENTS_11[] = "http://purl.org/dc/elements/1.1/";
const char  _ll_shared_xmp_creator_rest[] = "creator";

/**
 * FIXME: This could use a test or two!
 */
static void parse_ns_and_rest_from_uri(struct _ll_shared_xmp_ns_and_rest * etc,
				       const char * uri) {
	printf("Yeek\n");
	/* First look for # */
	char * match = NULL;
	int offset = 0;

	const char SEPARATORS[] = {'#', '/', '\0'};
	int sep_idx = 0;
	char this_sep = SEPARATORS[sep_idx];

	while (this_sep != '\0') {
		match = strrchr(uri, this_sep);
		if (match != NULL) {
			/* Got one. */
			/* The match includes the '#' we were looking
			 * for, so bump it forward by one. */
			match += 1;
			
			/* Did we fall off the end of the string?  If
			 * so, get out of here! */
			if (match[0] == '\0') {
				match = NULL;
				break;
			}
			
			/* Great, so we found a "#". At what index
			 * does it appear? */
			offset = match - uri;
			
			etc->namespace = strndup(uri, offset);
			etc->rest = strdup(match);
			printf("namespace: %s\n", etc->namespace);
			printf("rest: %s\n", etc->rest);
			return;
		}			       
		sep_idx += 1;
		this_sep = SEPARATORS[sep_idx];
	}
	/* No match?
	 * Oh, well, then fall off the end and do not change ret.
	 */
}

struct _ll_shared_xmp_ns_and_rest
_ll_shared_xmp_uri2struct (const char * uri) {
  struct _ll_shared_xmp_ns_and_rest ret;
  ret.namespace = NULL;
  ret.rest = NULL;

  if (strcmp(LL_LICENSE, uri) == 0) {
    ret.namespace = NS_CC;
    ret.rest = _ll_shared_xmp_license;
  } else if (strcmp(LL_WEBSTATEMENT, uri) == 0) {
    ret.namespace = NS_XAP_RIGHTS;
    ret.rest = _ll_shared_xmp_webstatement;
  } else if (strcmp(LL_MORE_PERMISSIONS, uri) == 0) {
    ret.namespace = NS_CC;
    ret.rest = _ll_shared_xmp_morepermissions;
  } else if (strcmp(LL_CREATOR, uri) == 0) {
	  printf("YOW\n");
    ret.namespace = LL_NS_PURL_DC_ELEMENTS_11;
    ret.rest = _ll_shared_xmp_creator_rest;
  } else {
    /* Make a WILD guess as to the predicate formatting to use.
     * Since XMP requires that we store every predciate as a namespace+rest
     * pair, we guess that the last "/" or "#" indicates the namespace
     * separator. This works fine for e.g. LL_NS_PURL_DC_ELEMENTS_11.
     *
     * I'll keep the hard-coding of LL_CREATOR in above since 
     * we actually know for sure the above is correct.
     *
     * Later we may want to distinguish the cases of "we knew 
     * what we were doing" and "we made a WILD guess that happened to work."
     */
	  parse_ns_and_rest_from_uri(&ret, uri);
  }
  return ret;
}
