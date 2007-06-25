#include <Python.h>
#include "system_licenses.h"

static PyObject* py_get_jurisdiction(PyObject* self, PyObject* args) { // (uri_t);
	const uri_t u;
	if (!PyArg_ParseTuple(args, "s", &u))
		return NULL;
	juris_t j = ll_get_jurisdiction(u);
	return Py_BuildValue("s",j);
}

static PyObject* py_get_locales(PyObject* self, PyObject* args) { //(uri_t)
	const uri_t u;
	if (!PyArg_ParseTuple(args, "s", &u))
		return NULL;
	locale_t* l = ll_get_locales(u);
	int i =0;
	PyObject* list = PyList_New(0);
	while (l!=NULL && l[i]!=NULL) {
		PyList_Append(list,PyString_FromString(l[i]));
		i++;
	}
	return list;
}

static PyObject* py_get_name(PyObject* self, PyObject* args) { //(uri_t)
	const uri_t u;
	if (!PyArg_ParseTuple(args, "s", &u))
		return NULL;
	char* name = ll_get_name(u);
	return Py_BuildValue("s",name);
}

static PyObject* py_get_version(PyObject* self, PyObject* args) { //(uri_t)
	const uri_t u;
	if (!PyArg_ParseTuple(args, "s", &u))
		return NULL;
	version_t v = ll_get_version(u);
	return Py_BuildValue("(iii)",v[0],v[1],v[2]);
}

static PyObject* py_get_notification(PyObject* self, PyObject* args) { // uri_t, char*
	const uri_t u;
	const char* url;
	char* s;
	if (PyArg_ParseTuple(args,"ss",&u,&url))
		s = ll_get_verifiable_notification(u,url);
	else if (PyArg_ParseTuple(args,"s",&u))
		s = ll_get_notification(u);
	else
		return NULL;
	return Py_BuildValue("s",s);
}

static PyObject* py_verify_uri(PyObject* self, PyObject* args) { // uri_t
	const uri_t u;
	if (!PyArg_ParseTuple(args,"s",&u))
		return NULL;
	int b = ll_verify_uri(u);
	return PyBool_FromLong((long) b);
}
static PyObject* py_get_attribute(PyObject* self, PyObject* args)  { // uri_t,attribute_t
	const uri_t u;
	const attribute_t a;
	if (!PyArg_ParseTuple(args,"ss",&u,&a))
		return NULL;
	char** avs = ll_get_attribute(u,a);
	int i =0;
	PyObject* list = PyList_New(0);
	while (avs!=NULL && avs[i]!=NULL) {
		PyList_Append(list,PyString_FromString(avs[i]));
		i++;
	}
	return list;
}

static PyObject* py_get_licenses(PyObject* self, PyObject* args)  { // juris_t
	const juris_t j;
	uri_t* us;
	if (PyArg_ParseTuple(args,"s",&j))
		us = ll_get_licenses(j);
	else
		us = ll_get_all_licenses();
	int i =0;
	PyObject* list = PyList_New(0);
	while (us!=NULL && us[i]!=NULL) {
		PyList_Append(list,PyString_FromString(us[i]));
		i++;
	}
	return list;
}
//uri_t* ll_get_all_licenses();
//uri_t* ll_get_licenses_in_juris(const juris_t);

static PyMethodDef LicenseMethods[] = {
		{"get_jurisdiction",  py_get_jurisdiction, METH_VARARGS,
     "Get the jurisdiction of the given license uri."},
		{"get_locales",  py_get_locales, METH_VARARGS,
     "Get the available locales of the given license uri."},
		{"get_name",  py_get_name, METH_VARARGS,
     "Get the name of the given license uri."},
    {"get_version",  py_get_version, METH_VARARGS,
     "Get the version of the given license uri."},
    {"get_notification",  py_get_notification, METH_VARARGS,
     "Get the copyright notification for the given uri with a possible verification url."},
    {"verify_uri",  py_verify_uri, METH_VARARGS,
     "Returns a boolean indicating if the uri is recognized by the system."},
    {"get_attribute", py_get_attribute, METH_VARARGS,
     "Returns a list of values found for the given attribute."},
    {"get_licenses", py_get_licenses, METH_VARARGS,
     "Returns a list of licenses in the optional jurisdiciton (otherwise all are given.)"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC initliblicense(void) {
	(void) Py_InitModule("liblicense",LicenseMethods);
}
