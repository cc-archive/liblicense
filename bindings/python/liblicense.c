// Creative Commons has made the contents of this file
// available under a CC-GNU-GPL license:
//
// http://creativecommons.org/licenses/GPL/2.0/
//
// A copy of the full license can be found as part of this
// distribution in the file COPYING.
// 
// You may use the liblicense software in accordance with the
// terms of that license. You agree that you are solely 
// responsible for your use of the liblicense software and you
// represent and warrant to Creative Commons that your use
// of the liblicense software will comply with the CC-GNU-GPL.
//
// Copyright 2007, Creative Commons, www.creativecommons.org.
// Copyright 2007, Scott Shawcroft.

#include <Python.h>

#include <liblicense.h>

static PyObject* py_get_jurisdiction(PyObject* self, PyObject* args) { // (uri_t);
	const uri_t u;
	if (!PyArg_ParseTuple(args, "s", &u))
		return NULL;
	juris_t j = ll_get_jurisdiction(u);
	PyObject* result = Py_BuildValue("s",j);
	free(j);
	return result;
}

static PyObject* py_get_name(PyObject* self, PyObject* args) { //(uri_t)
	const uri_t u;
	if (!PyArg_ParseTuple(args, "s", &u))
		return NULL;
	char* name = ll_get_name(u);
	PyObject* result = Py_BuildValue("s",name);
	free(name);
	return result;
}

static PyObject* py_get_version(PyObject* self, PyObject* args) { //(uri_t)
	const uri_t u;
	if (!PyArg_ParseTuple(args, "s", &u))
		return NULL;
	version_t v = ll_get_version(u);
	PyObject* result = Py_BuildValue("(iii)",v[0],v[1],v[2]);
	free(v);
	return result;
}

static PyObject* py_get_permits(PyObject* self, PyObject* args) { //(uri_t)
	const uri_t u;
	if (!PyArg_ParseTuple(args, "s", &u))
		return NULL;
	locale_t* l = ll_get_permits(u);
	int i =0;
	PyObject* list = PyList_New(0);
	while (l!=NULL && l[i]!=NULL) {
		PyList_Append(list,PyString_FromString(l[i]));
		i++;
	}
	ll_free_list(l);
	return list;
}
static PyObject* py_get_prohibits(PyObject* self, PyObject* args) { //(uri_t)
	const uri_t u;
	if (!PyArg_ParseTuple(args, "s", &u))
		return NULL;
	locale_t* l = ll_get_prohibits(u);
	int i =0;
	PyObject* list = PyList_New(0);
	while (l!=NULL && l[i]!=NULL) {
		PyList_Append(list,PyString_FromString(l[i]));
		i++;
	}
	ll_free_list(l);
	return list;
}
static PyObject* py_get_requires(PyObject* self, PyObject* args) { //(uri_t)
	const uri_t u;
	if (!PyArg_ParseTuple(args, "s", &u))
		return NULL;
	locale_t* l = ll_get_requires(u);
	int i =0;
	PyObject* list = PyList_New(0);
	while (l!=NULL && l[i]!=NULL) {
		PyList_Append(list,PyString_FromString(l[i]));
		i++;
	}
	ll_free_list(l);
	return list;
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
	int b;
	if (!PyArg_ParseTuple(args,"ssi",&u,&a,&b))
		return NULL;
	char** avs = ll_get_attribute(u,a,b);
	int i =0;
	PyObject* list = PyList_New(0);
	while (avs!=NULL && avs[i]!=NULL) {
		PyList_Append(list,PyString_FromString(avs[i]));
		i++;
	}
	ll_free_list(avs);
	return list;
}

static PyObject* py_get_licenses(PyObject* self, PyObject* args)  { // juris_t
	juris_t j=NULL;
	uri_t* us;
	if (PyArg_ParseTuple(args,"|s",&j)) {
		if (j==NULL)
			us = ll_get_licenses(NULL);
		else
			us = ll_get_licenses(j);
	} else {
		return NULL;
	}
	int i =0;
	PyObject* list = PyList_New(0);
	while (us!=NULL && us[i]!=NULL) {
		PyList_Append(list,PyString_FromString(us[i]));
		i++;
	}
	ll_free_list(us);
	return list;
}
static PyObject* py_write(PyObject* self, PyObject* args)  { // filename_t, juris_t[,module_t]
	const filename_t f;
	const juris_t j;
	const module_t m = NULL;
	int result;
	if (PyArg_ParseTuple(args,"ss|s",&f,&j,&m))
		if (m!=NULL)
			result = ll_module_write(f,j,m);
		else
			result = ll_write(f,j);
	else
		return NULL;
	return PyBool_FromLong((long) result);
}

static PyObject* py_read(PyObject* self, PyObject* args)  { // filename_t[,module_t]
	const filename_t f;
	const module_t m = NULL;
	juris_t j;
	if (PyArg_ParseTuple(args,"s|s",&f,&m))
		if (m!=NULL)
			j = ll_module_read(f,m);
		else
			j = ll_read(f);
	else
		return NULL;
	PyObject* result = Py_BuildValue("s",j);
	free(j);
	return result;
}

static PyObject* py_set_default(PyObject* self, PyObject* args)  { // uri_t
	uri_t u;
	if(!PyArg_ParseTuple(args,"s",&u))
		return NULL;
	return PyBool_FromLong((long) ll_set_default(u));
}

static PyObject* py_get_default(PyObject* self, PyObject* args)  {
	uri_t u = ll_get_default();
	PyObject* result = Py_BuildValue("s",u);
	free(u);
	return result;
}

static PyObject* py_get_config_modules(PyObject* self, PyObject* args)  {
	module_t* ms = ll_get_config_modules();
	int i =0;
	PyObject* list = PyList_New(0);
	while (ms!=NULL && ms[i]!=NULL) {
		PyList_Append(list,PyString_FromString(ms[i]));
		i++;
	}
	ll_free_list(ms);
	return list;
}

static PyObject* py_get_io_modules(PyObject* self, PyObject* args)  {
	module_t* ms = ll_get_io_modules();
	int i =0;
	PyObject* list = PyList_New(0);
	while (ms!=NULL && ms[i]!=NULL) {
		PyList_Append(list,PyString_FromString(ms[i]));
		i++;
	}
	ll_free_list(ms);
	return list;
}

static PyObject* py_module_mime_types(PyObject* self, PyObject* args)  {
	module_t m;
	if(!PyArg_ParseTuple(args,"s",&m))
		return NULL;
	mime_type_t* mts = ll_module_mime_types(m);
	int i =0;
	PyObject* list = PyList_New(0);
	while (mts!=NULL && mts[i]!=NULL) {
		PyList_Append(list,PyString_FromString(mts[i]));
		i++;
	}
	ll_free_list(mts);
	return list;
}

static PyMethodDef LicenseMethods[] = {
		{"get_jurisdiction",  py_get_jurisdiction, METH_VARARGS,
	 "Get the jurisdiction of the given license uri."},
		{"get_name",  py_get_name, METH_VARARGS,
	 "Get the name of the given license uri."},
	{"get_version",  py_get_version, METH_VARARGS,
	 "Get the version of the given license uri."},
	{"get_permits",  py_get_permits, METH_VARARGS,
	 "Gets the uris of the actions permitted."},
	{"get_prohibits",  py_get_prohibits, METH_VARARGS,
	 "Gets the uris of the actions prohibited."},
	{"get_requires",  py_get_requires, METH_VARARGS,
	 "Gets the uris of the actions required."},
	{"verify_uri",  py_verify_uri, METH_VARARGS,
	 "Returns a boolean indicating if the uri is recognized by the system."},
	{"get_attribute", py_get_attribute, METH_VARARGS,
	 "Returns a list of values found for the given attribute."},
	{"get_licenses", py_get_licenses, METH_VARARGS,
	 "Returns a list of licenses in the optional jurisdiciton (otherwise all are given.)"},
	{"write", py_write, METH_VARARGS,
	 "Returns whether or not the license write succeeded."},
	{"read",py_read, METH_VARARGS,
	 "Returns the license for the given file."},
	{"set_default", py_set_default, METH_VARARGS,
	 "Sets the system default license to the given uri."},
	{"get_default", py_get_default, METH_VARARGS,
	 "Returns the system default license."},
	{"get_config_modules", py_get_config_modules, METH_VARARGS,
	 "Returns a list of the available config modules."},
	{"get_io_modules", py_get_io_modules, METH_VARARGS,
	 "Returns a list of the available io modules."},
	{"module_mime_types",py_module_mime_types, METH_VARARGS,
	 "Returns a list of the mime types supported by the given module."},
	{NULL, NULL, 0, NULL}		/* Sentinel */
};

PyMODINIT_FUNC initliblicense(void) {
  ll_init();
	(void) Py_InitModule("liblicense",LicenseMethods);
}
