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
    ret.namespace = LL_NS_PURL_DC_ELEMENTS_11;
    ret.rest = _ll_shared_xmp_morepermissions;
  }
  return ret;
}
