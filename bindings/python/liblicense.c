#include <Python.h>
#include "liblicense.h"

static PyObject* py_get_jurisdiction(PyObject* self, PyObject* args) { // (uri_t);
	const uri_t u;
	if (!PyArg_ParseTuple(args, "s", &u))
		return NULL;
	juris_t j = get_jurisdiction(u);
	return Py_BuildValue("s",j);
}

static PyObject* py_get_jurisdictions(PyObject* self, PyObject* args) { //(short_t)(bitcode_t);
	const short_t s;
	const bitcode_t b;
	juris_t* js;
	if (PyArg_ParseTuple(args, "s", &s))
		js = get_short_jurisdictions(s);
	else if (PyArg_ParseTuple(args, "i", &b))
		js = get_bitcode_jurisdictions(b);
	else
		return NULL;

	int i =0;
	PyObject* list = PyList_New(0);
	while (js[i]!=NULL) {
		PyList_Append(list,PyString_FromString(js[i]));
		i++;
	}
	return list;
}

static PyObject* py_get_locale(PyObject* self, PyObject* args) { //(uri_t)
	const uri_t u;
	if (!PyArg_ParseTuple(args, "s", &u))
		return NULL;
	locale_t l = get_locale(u);
	return Py_BuildValue("s",l);
}

static PyObject* py_get_locales(PyObject* self, PyObject* args) { //(short_t)(bitcode_t);
	const short_t s;
	const bitcode_t b;
	const juris_t j;
	locale_t* ls;
	if (PyArg_ParseTuple(args, "ss", &s, &j))
		ls = get_short_locales(s,j);
	else if (PyArg_ParseTuple(args, "is", &b, &j))
		ls = get_bitcode_locales(b,j);
	else
		return NULL;

	int i =0;
	PyObject* list = PyList_New(0);
	while (ls[i]!=NULL) {
		PyList_Append(list,PyString_FromString(ls[i]));
		i++;
	}
	return list;
}

static PyObject* py_get_name(PyObject* self, PyObject* args) { //(uri_t)
	const uri_t u;
	if (!PyArg_ParseTuple(args, "s", &u))
		return NULL;
	char* name = get_name(u);
	return Py_BuildValue("s",name);
}

static PyObject* py_get_version(PyObject* self, PyObject* args) { //(uri_t)
	const uri_t u;
	if (!PyArg_ParseTuple(args, "s", &u))
		return NULL;
	version_t v = get_version(u);
	return Py_BuildValue("(iii)",v[0],v[1],v[2]);
}

static PyObject* py_get_versions(PyObject* self, PyObject* args) { //(short or bitcode, uri_t)
	const uri_t u;
	const bitcode_t b;
	const short_t s;
	version_t* vs;
	if (PyArg_ParseTuple(args, "ss", &s, &u))
		vs = get_short_versions(s,u);
	else if (PyArg_ParseTuple(args, "is", &b, &u))
		vs = get_bitcode_versions(b,u);
	else
		return NULL;
	int i =0;
	PyObject* list = PyList_New(0);
	while (vs[i]!=NULL) {
		PyList_Append(list,Py_BuildValue("(iii)",vs[i][0],vs[i][1],vs[i][2]));
		i++;
	}
	return list;
}

static PyObject* py_get_short(PyObject* self, PyObject* args) { // uri_t
	const uri_t u;
	if (!PyArg_ParseTuple(args, "s", &u))
		return NULL;
	short_t s = get_short(u);
	return Py_BuildValue("s",s);
}

static PyObject* py_has_flag(PyObject* self, PyObject* args) { // flag_t, uri_t
	const flag_t f;
	const uri_t u;
	if (!PyArg_ParseTuple(args,"is",&f,&u))
		return NULL;
	int b = has_flag(f,u);
	return PyBool_FromLong((long) b);
}

static PyObject* py_family(PyObject* self, PyObject* args) { // uri_t
	const uri_t u;
	if (!PyArg_ParseTuple(args,"s",&u))
		return NULL;
	family_t f = family(u);
	return Py_BuildValue("s",f);
}

static PyObject* py_get_notification(PyObject* self, PyObject* args) { // uri_t, char*
	const uri_t u;
	const char* url;
	char* s;
	if (PyArg_ParseTuple(args,"ss",&u,&url))
		s = get_verifiable_notification(u,url);
	else if (PyArg_ParseTuple(args,"s",&u))
		s = get_notification(u);
	else
		return NULL;
	return Py_BuildValue("s",s);
}

static PyObject* py_verify_uri(PyObject* self, PyObject* args) { // uri_t
	const uri_t u;
	if (!PyArg_ParseTuple(args,"s",&u))
		return NULL;
	int b = verify_uri(u);
	return PyBool_FromLong((long) b);
}

static PyObject* py_get_license(PyObject* self, PyObject* args) { // (family_t,bitcode_t, juris_t,locale_t);
	const family_t f;
	const bitcode_t b;
	const short_t s;
	const juris_t j;
	const locale_t l;
	uri_t u;
	if (PyArg_ParseTuple(args,"siss",&f,&b,&j,&l))
		u = get_bitcode_license(f,b,j,l);
	else if (PyArg_ParseTuple(args,"ssss",&f,&s,&j,&l))
		u = get_short_license(f,s,j,l);
	else
		return NULL;
	return Py_BuildValue("s",u);
}

static PyObject* py_get_families(PyObject* self, PyObject* args) {
	family_t* fs = get_families();
	int i =0;
	PyObject* list = PyList_New(0);
	while (fs[i]!=NULL) {
		PyList_Append(list,Py_BuildValue("s",fs[i]));
		i++;
	}
	return list;
}
// FlagInfo* family_flags(family_t);
// LicenseInfo* get_all_licenses();
// LicenseInfo* get_general_licenses(family_t);

static PyMethodDef LicenseMethods[] = {
		{"get_jurisdiction",  py_get_jurisdiction, METH_VARARGS,
     "Get the jurisdiction of the given license uri."},
		{"get_jurisdictions", py_get_jurisdictions, METH_VARARGS,
		 "Get the available jurisdictions for the given short name or bitcode."},
		{"get_locale",  py_get_locale, METH_VARARGS,
     "Get the locale of the given license uri."},
		{"get_locales", py_get_locales, METH_VARARGS,
		 "Get the available locales for the given short name or bitcode and jurisdiction."},
		{"get_name",  py_get_name, METH_VARARGS,
     "Get the name of the given license uri."},
    {"get_version",  py_get_version, METH_VARARGS,
     "Get the version of the given license uri."},
    {"get_versions",  py_get_versions, METH_VARARGS,
     "Get available versions given short name or bitcode and jurisdiction."},
    {"get_short",  py_get_short, METH_VARARGS,
     "Get the short name for the given uri."},
    {"has_flag",  py_has_flag, METH_VARARGS,
     "Returns whether or not the given uri satisfies the given flag."},
    {"family",  py_family, METH_VARARGS,
     "Returns the family the uri is in."},
    {"get_notification",  py_get_notification, METH_VARARGS,
     "Get the copyright notification for the given uri with a possible verification url."},
    {"verify_uri",  py_verify_uri, METH_VARARGS,
     "Returns a boolean indicating if the uri is recognized by the system."},
    {"get_license",  py_get_license, METH_VARARGS,
     "Get the uri of the license which satisfies the given conditions."},
    {"get_families",  py_get_families, METH_VARARGS,
     "Gets a list of all the available license families."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC initliblicense(void) {
	(void) Py_InitModule("liblicense",LicenseMethods);
}
