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
// Copyright 2007, Alex Roberts.

#include "license.h"

VALUE cLiblicense;

/* 
 * Get all the useful data out of the uri and store in
 * Ruby friendly variables
 */
static void _rbll_cache_info(ruby_liblicense *license, VALUE uri) {
	juris_t j;
	uri_t u;
	version_t v;
	locale_t* l;
	
	int i;

	/* URI info */
	u = StringValueCStr(uri);

	license->uri = uri;

	/* name */
	license->name = rb_str_new2(ll_get_name(u));

	/* Jurisdiction info */	
	j = ll_get_jurisdiction(u);
	
	/* in the event of no jurisdiction. */
	if (j == NULL) {
		j = "Unported";
	}
	
	license->jurisdiction = rb_str_new2(j);
	
	/* Version info */
	v = ll_get_version(u);
	
	license->version = rb_ary_new();
	license->version = rb_ary_push(license->version, INT2NUM(v[0]));
	license->version = rb_ary_push(license->version, INT2NUM(v[1]));
	license->version = rb_ary_push(license->version, INT2NUM(v[2]));	
	
	/* Permits */
	l = ll_get_permits(u);
	license->permits = rb_ary_new();
	
	i = 0;
	while (l != NULL && l[i] != NULL) {
		license->permits = rb_ary_push(license->permits, rb_str_new2(l[i]));
		i++;
	}
	ll_free_list(l);
	
	/* Prohibits */
	l = ll_get_prohibits(u);
	license->prohibits = rb_ary_new();
	
	i = 0;
	while (l != NULL && l[i] != NULL) {
		license->prohibits = rb_ary_push(license->prohibits, rb_str_new2(l[i]));
		i++;
	}
	ll_free_list(l);
	
	/* Requires */
	l = ll_get_requires(u);
	license->requires = rb_ary_new();
	
	i = 0;
	while (l != NULL && l[i] != NULL) {
		license->requires = rb_ary_push(license->requires, rb_str_new2(l[i]));
		i++;
	}
	ll_free_list(l);
	
}

/* GC helper functions */
void rbll_mark(ruby_liblicense *license) {
	rb_gc_mark(license->uri);
	rb_gc_mark(license->name);
	rb_gc_mark(license->jurisdiction);
	rb_gc_mark(license->version);
	rb_gc_mark(license->permits);
	rb_gc_mark(license->prohibits);
	rb_gc_mark(license->requires);
	rb_gc_mark(license->filename);
}

void rbll_free(ruby_liblicense *license) {
	free(license);
}

/* Allocate new License object */
static VALUE rbll_alloc(VALUE klass) {
	VALUE obj;
	
	ruby_liblicense *license = ALLOC(ruby_liblicense);	

	license->uri = Qnil;					/* STRING */
	license->name = Qnil;					/* STRING */
	license->jurisdiction = Qnil;	/* STRING */
	license->version = Qnil;			/* ARRAY  */
	license->permits = Qnil;			/* ARRAY  */
	license->prohibits = Qnil;		/* ARRAY  */
	license->requires = Qnil;			/* ARRAY  */
	license->filename = Qnil;			/* STRING */
		
	obj = Data_Wrap_Struct(klass, rbll_mark, rbll_free, license);
	
	return obj;
}

static VALUE rbll_initialize(VALUE self, VALUE uri) {
	ruby_liblicense *license;
	
	Data_Get_Struct(self, ruby_liblicense, license);
	
	_rbll_cache_info (license, uri);
	
	return self;
}

static VALUE rbll_initialize_copy(VALUE copy, VALUE orig) {
	ruby_liblicense *l_orig, *l_copy;
	
	if (copy == orig)
		return copy;
	
	if (TYPE(orig) != T_DATA || RDATA(orig)->dfree != (RUBY_DATA_FUNC)rbll_free) {
		rb_raise(rb_eTypeError, "wrong argument type");
	}
	
	// copy license details
	Data_Get_Struct(orig, ruby_liblicense, l_orig);
	Data_Get_Struct(copy, ruby_liblicense, l_copy);
	MEMCPY(l_copy, l_orig, ruby_liblicense, 1);
	
	return copy;
}

/* Class Methods */

static VALUE rbll_read(int argc, VALUE *argv, VALUE klass) {
	VALUE obj;
	VALUE file, module;
	uri_t uri;
	ruby_liblicense *license;

	rb_scan_args(argc, argv, "11", &file, &module);

	if (module != Qnil) 
		uri = ll_module_read(StringValueCStr(file), StringValueCStr(module));
	else
		uri = ll_read(StringValueCStr(file));
	
	if (uri == NULL) 
		return Qnil;

		
	obj = rbll_alloc(klass);
	Data_Get_Struct(obj, ruby_liblicense, license);

	_rbll_cache_info(license, rb_str_new2(uri));


	license->filename = file;


	return obj;
}

static VALUE rbll_licenses_get(int argc, VALUE *argv, VALUE klass) {
	VALUE licenses, juris;
	uri_t *l;
	int i;
	
	rb_scan_args(argc, argv, "01", &juris);
	
	if (juris != Qnil)
		l = ll_get_licenses(StringValueCStr(juris));
	else
		l = ll_get_licenses(NULL);
	
	licenses = rb_ary_new();
	i = 0;
	while (l != NULL && l[i] != NULL) {
		licenses = rb_ary_push(licenses, rb_str_new2(l[i]));
		i++;
	}
	ll_free_list(l);
	
	return licenses;
}

static VALUE rbll_verify(VALUE self, VALUE uri) {
	
	if (ll_verify_uri(StringValueCStr(uri)))
		return Qtrue;
	else
		return Qfalse;
}

static VALUE rbll_modules(VALUE self) {
	
	ll_print_module_info();
	
	return Qnil;
}

static VALUE rbll_modules_config(VALUE self) {
	module_t *m = ll_get_config_modules();
	VALUE modules;
	int i;
	
	modules = rb_ary_new();
	i = 0;
	while (m != NULL && m[i] != NULL) {
		modules = rb_ary_push(modules, rb_str_new2(m[i]));
		i++;
	}
	ll_free_list(m);
	
	return modules;
}

static VALUE rbll_modules_io(VALUE self) {
	module_t *m = ll_get_io_modules();
	VALUE modules;
	int i;
	
	modules = rb_ary_new();
	i = 0;
	while (m != NULL && m[i] != NULL) {
		modules = rb_ary_push(modules, rb_str_new2(m[i]));
		i++;
	}
	ll_free_list(m);
	
	return modules;
}

/* Instance Methods */

static VALUE rbll_uri_get(VALUE self) {
	ruby_liblicense *license;
	Data_Get_Struct(self, ruby_liblicense, license);
		
	return license->uri;
}

static VALUE rbll_uri_set(VALUE self, VALUE uri) {
	ruby_liblicense *license;
	Data_Get_Struct(self, ruby_liblicense, license);
	
	_rbll_cache_info (license, uri);
	
	return Qnil;
}

static VALUE rbll_filename_get(VALUE self) {
	ruby_liblicense *license;
	Data_Get_Struct(self, ruby_liblicense, license);
		
	return license->filename;
}


static VALUE rbll_name_get(VALUE self) {
	ruby_liblicense *license;
	Data_Get_Struct(self, ruby_liblicense, license);
		
	return license->name;
}

static VALUE rbll_version_get(VALUE self) {
	ruby_liblicense *license;
	Data_Get_Struct(self, ruby_liblicense, license);
		
	return license->version;
}

static VALUE rbll_jurisdiction_get(VALUE self) {
	ruby_liblicense *license;
	
	Data_Get_Struct(self, ruby_liblicense, license);
	
	return license->jurisdiction;
}

static VALUE rbll_attribute_get(int argc, VALUE *argv, VALUE self) {
	VALUE attribute, locale;
	VALUE attribs;
	int i;
	char **avs;
	ruby_liblicense *license;
	Data_Get_Struct(self, ruby_liblicense, license);
	
	rb_scan_args(argc, argv, "11", &attribute, &locale);
	
	
	avs = ll_get_attribute(
	                  StringValueCStr(license->uri), 
	                  StringValueCStr(attribute),
	                  locale);

	attribs = rb_ary_new();
	i = 0;
	while (avs != NULL && avs[i] != NULL) {
		attribs = rb_ary_push(attribs, rb_str_new2(avs[i]));
		i++;
	}
	ll_free_list(avs);
	
	return attribs;
}

static VALUE rbll_permits_get(VALUE self) {
	ruby_liblicense *license;
	Data_Get_Struct(self, ruby_liblicense, license);
		
	return license->permits;
}

static VALUE rbll_prohibits_get(VALUE self) {
	ruby_liblicense *license;
	Data_Get_Struct(self, ruby_liblicense, license);
		
	return license->prohibits;
}

static VALUE rbll_requires_get(VALUE self) {
	ruby_liblicense *license;
	Data_Get_Struct(self, ruby_liblicense, license);
		
	return license->requires;
}

static VALUE rbll_write(int argc, VALUE *argv, VALUE self) {
	VALUE file, module;
	int result;
	ruby_liblicense *license;
	Data_Get_Struct(self, ruby_liblicense, license);
	
	rb_scan_args(argc, argv, "11", &file, &module);
	
	if (module != Qnil)
		result = ll_module_write(
		                StringValueCStr(file), 
		                StringValueCStr(license->uri), 
		                StringValueCStr(module));
	else
		result = ll_write(
		                StringValueCStr(file), 
		                StringValueCStr(license->uri));
		
	
	return INT2NUM(result);
}

void Init_liblicense() {
	ll_init();

	cLiblicense = rb_define_class("Liblicense", rb_cObject);
	rb_define_alloc_func(cLiblicense, rbll_alloc);
	
	rb_define_method(cLiblicense, "initialize", rbll_initialize, 1);
	rb_define_method(cLiblicense, "initialize_copy", rbll_initialize_copy, 1);

	rb_define_method(cLiblicense, "uri", rbll_uri_get, 0);
	rb_define_method(cLiblicense, "uri=", rbll_uri_set, 1);
	rb_define_method(cLiblicense, "filename", rbll_filename_get, 0);

	rb_define_method(cLiblicense, "name", rbll_name_get, 0);
	rb_define_method(cLiblicense, "version", rbll_version_get, 0);	
	rb_define_method(cLiblicense, "jurisdiction", rbll_jurisdiction_get, 0);
	
	rb_define_method(cLiblicense, "attribute", rbll_attribute_get, -1);
	
	rb_define_method(cLiblicense, "permits", rbll_permits_get, 0);	
	rb_define_method(cLiblicense, "prohibits", rbll_prohibits_get, 0);		
	rb_define_method(cLiblicense, "requires", rbll_requires_get, 0);	

	rb_define_method(cLiblicense, "write", rbll_write, -1);
	rb_define_singleton_method(cLiblicense, "read", rbll_read, -1);

	rb_define_singleton_method(cLiblicense, "licenses", rbll_licenses_get, -1);
	rb_define_singleton_method(cLiblicense, "verify", rbll_verify, 1);

	rb_define_singleton_method(cLiblicense, "modules", rbll_modules, 0);
	rb_define_singleton_method(cLiblicense, "modules_config", rbll_modules_config, 0);
	rb_define_singleton_method(cLiblicense, "modules_io", rbll_modules_io, 0);
}

