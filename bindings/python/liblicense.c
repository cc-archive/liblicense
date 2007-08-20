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
	int i=1;
	PyObject* list = PyList_New(0);
	while (v!=NULL && i <= v[0]) {
		PyList_Append(list,PyInt_FromLong((long)v[i]));
		i++;
	}
	free(v);
	return list;
}

static PyObject* py_get_permits(PyObject* self, PyObject* args) { //(uri_t)
	const uri_t u;
	if (!PyArg_ParseTuple(args, "s", &u))
		return NULL;
	ll_locale_t* l = ll_get_permits(u);
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
	ll_locale_t* l = ll_get_prohibits(u);
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
	ll_locale_t* l = ll_get_requires(u);
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

static PyObject* py_get_jurisdictions(PyObject* self)  {
	juris_t *j = ll_get_jurisdictions();

	int i =0;
	PyObject* list = PyList_New(0);
	while (j!=NULL && j[i]!=NULL) {
		PyList_Append(list,PyString_FromString(j[i]));
		i++;
	}
	ll_free_list(j);
	return list;
}

static PyObject* py_jurisdiction_name(PyObject* self, PyObject* args) { //(juris_t)
	const juris_t j;
	if (!PyArg_ParseTuple(args, "s", &j))
		return NULL;
	char* name = ll_jurisdiction_name(j);
	PyObject* result = Py_BuildValue("s",name);
	free(name);
	return result;
}

static PyObject* py_write(PyObject* self, PyObject* args)  { // filename_t, uri_t[,module_t]
	const filename_t f;
	const uri_t u;
	const module_t m = NULL;
	int result;
	if (PyArg_ParseTuple(args,"sz|s",&f,&u,&m))
		if (m!=NULL)
			result = ll_module_write(f,u,m);
		else
			result = ll_write(f,u);
	else
		return NULL;
	if (result==-1) // just in case PyBool_FromLong thinks its true
		result = 0;
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
	if(!PyArg_ParseTuple(args,"z",&u))
		return NULL;
	return PyBool_FromLong((long) ll_set_default(u));
}

static PyObject* py_get_default(PyObject* self, PyObject* args)  {
	uri_t u = ll_get_default();
	PyObject* result = Py_BuildValue("z",u);
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

static PyObject* py_print_module_info(PyObject* self, PyObject* args)  {
	ll_print_module_info();
	return Py_BuildValue("");
}

typedef struct {
	PyObject_HEAD
	ll_license_chooser_t *chooser;
} LicenseChooser;

static void
LicenseChooser_dealloc(LicenseChooser* self)
{
	if (self->chooser)
		ll_free_license_chooser(self->chooser);
	self->ob_type->tp_free((PyObject*)self);
}

static int
LicenseChooser_init(LicenseChooser *self, PyObject *args, PyObject *kwds)
{
	juris_t j = NULL;
	PyObject *attrs = NULL;

	if (! PyArg_ParseTuple(args,"zO",&j,&attrs))
			return -1; 

	if (!PyList_Check(attrs))
		return -1;

	const char *attributes[PyList_Size(attrs)+1];
	attributes[PyList_Size(attrs)] = NULL;

	int i;
	for (i=0; i<PyList_Size(attrs); ++i) {
		PyObject *attr = PyList_GetItem(attrs,i);
		if (!PyString_Check(attr))
			return -1;
		char *string = PyString_AsString(attr);
		attributes[i] = string;
	}

	ll_license_chooser_t *chooser = ll_new_license_chooser(j,attributes);
	self->chooser = chooser;

	return 0;
}

static PyObject *
LicenseChooser_get_licenses(LicenseChooser* self, PyObject *args, PyObject *kwds )
{
	static char *kwlist[] = {"permits", "requires", "prohibits", NULL};

	int permits = 0, requires = 0, prohibits = 0;
	if (!PyArg_ParseTupleAndKeywords(args,kwds,"|iii",kwlist,&permits,&requires,&prohibits))
		return NULL;

	const ll_license_list_t *license_list = ll_get_licenses_from_flags(self->chooser, permits, requires, prohibits);

	const ll_license_list_t *curr = license_list;
	PyObject* list = PyList_New(0);
	while (curr!=NULL) {
		PyList_Append(list,PyString_FromString(curr->license));
		curr = curr->next;
	}

	return list;
}

static PyObject *
LicenseChooser_attribute_flag(LicenseChooser* self, PyObject *args)
{
	char *attr;
	if (!PyArg_ParseTuple(args,"s",&attr))
		return NULL;

	return PyInt_FromLong((long)ll_attribute_flag(self->chooser,attr));
}

static PyMethodDef LicenseChooser_methods[] = {
	{"get_licenses", (PyCFunction)LicenseChooser_get_licenses, METH_VARARGS | METH_KEYWORDS,
		"Return a list of licenses matching the given flags"
	},
	{"attribute_flag", (PyCFunction)LicenseChooser_attribute_flag, METH_VARARGS,
		"Return the appropriate permits/requires/prohibits flag to be passed to get_licenses"
	},
	{NULL}  /* Sentinel */
};

static PyTypeObject LicenseChooserType = {
	PyObject_HEAD_INIT(NULL)
	0,                         /*ob_size*/
	"Liblicense.LicenseChooser",             /*tp_name*/
	sizeof(LicenseChooser),             /*tp_basicsize*/
	0,                         /*tp_itemsize*/
	(destructor)LicenseChooser_dealloc, /*tp_dealloc*/
	0,                         /*tp_print*/
	0,                         /*tp_getattr*/
	0,                         /*tp_setattr*/
	0,                         /*tp_compare*/
	0,                         /*tp_repr*/
	0,                         /*tp_as_number*/
	0,                         /*tp_as_sequence*/
	0,                         /*tp_as_mapping*/
	0,                         /*tp_hash */
	0,                         /*tp_call*/
	0,                         /*tp_str*/
	0,                         /*tp_getattro*/
	0,                         /*tp_setattro*/
	0,                         /*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	"Object for searching for licenses based on what the license permits, requires, and/or prohibits",           /* tp_doc */
	0,		               /* tp_traverse */
	0,		               /* tp_clear */
	0,		               /* tp_richcompare */
	0,		               /* tp_weaklistoffset */
	0,		               /* tp_iter */
	0,		               /* tp_iternext */
	LicenseChooser_methods,             /* tp_methods */
	0,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)LicenseChooser_init,      /* tp_init */
};

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
	{"get_jurisdictions", py_get_jurisdictions, METH_NOARGS,
	 "Returns a list of all jurisdictions."},
	{"jurisdiction_name", py_jurisdiction_name, METH_VARARGS,
	 "Returns the jurisdiction name from the jurisdiction code."},
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
	{"print_module_info",py_print_module_info, METH_VARARGS,
	 "Print available modules and their capabilities"},
	{NULL, NULL, 0, NULL}		/* Sentinel */
};

PyMODINIT_FUNC initliblicense(void) {
  ll_init();
	PyObject *m = Py_InitModule("liblicense",LicenseMethods);
	PyModule_AddIntConstant(m, "UNSPECIFIED",LL_UNSPECIFIED);

	LicenseChooserType.tp_new = PyType_GenericNew;
	if (PyType_Ready(&LicenseChooserType) < 0)
			return;

	Py_INCREF(&LicenseChooserType);
	PyModule_AddObject(m, "LicenseChooser", (PyObject*)&LicenseChooserType);
}
