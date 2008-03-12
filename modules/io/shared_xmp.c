#include <stdbool.h>
#include <liblicense.h>
#include <exempi/xmp.h>
#include <exempi/xmpconsts.h>

#include "shared_xmp.h"

const char  _ll_shared_xmp_license[] = "license";

struct _ll_shared_xmp_ns_and_rest
_ll_shared_xmp_uri2struct (const char * uri) {
  struct _ll_shared_xmp_ns_and_rest ret;
  ret.namespace = NULL;
  ret.rest = NULL;

  if (strcmp(LL_LICENSE, uri) == 0) {
    ret.namespace = NS_CC;
    ret.rest = _ll_shared_xmp_license;
  }
  return ret;
}
