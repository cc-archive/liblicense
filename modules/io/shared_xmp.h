#ifndef __LL_SHARED_XMP_H
#define __LL_SHARED_XMP_H

struct _ll_shared_xmp_ns_and_rest {
  const char * namespace;
  const char * rest;
};

extern const char  _ll_shared_xmp_license[];
extern const char  _ll_shared_xmp_webstatement[];
const char  _ll_shared_xmp_morepermissions[];
extern const char LL_NS_PURL_DC_ELEMENTS_11[];

struct _ll_shared_xmp_ns_and_rest
_ll_shared_xmp_uri2struct (const char * uri);

#endif /* define __LL_SHARED_XMP_H */
