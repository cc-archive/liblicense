/*
 * Creative Commons has made the contents of this file
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
 * Copyright 2007, Scott Shawcroft.
 * Copyright (C) 2007 Peter Miller
 */

#include <Python.h>

#include "liblicense.h"

static PyObject *
py_get_jurisdiction (PyObject *self, PyObject *args)
{
  /* (ll_uri_t); */
  const ll_uri_t u;
  ll_juris_t j;
  PyObject *result;

  (void) self;
  if (!PyArg_ParseTuple (args, "s", &u))
    return NULL;
  j = ll_get_jurisdiction (u);
  result = Py_BuildValue ("s", j);
  free (j);
  return result;
}

static PyObject *
py_get_name (PyObject *self, PyObject *args)
{
  /* (ll_uri_t) */
  const ll_uri_t u;
  char *name;
  PyObject *result;

  (void) self;
  if (!PyArg_ParseTuple (args, "s", &u))
    return NULL;
  name = ll_get_name (u);
  result = Py_BuildValue ("s", name);
  free (name);
  return result;
}

static PyObject *
py_get_version (PyObject *self, PyObject *args)
{
  /* (ll_uri_t) */
  const ll_uri_t u;
  ll_version_t v;
  int i;
  PyObject *list;

  (void) self;
  if (!PyArg_ParseTuple (args, "s", &u))
    return NULL;
  v = ll_get_version (u);
  i = 1;
  list = PyList_New (0);
  while (v != NULL && i <= v[0])
    {
      PyList_Append (list, PyInt_FromLong ((long) v[i]));
      i++;
    }
  free (v);
  return list;
}

static PyObject *
py_get_permits (PyObject *self, PyObject *args)
{
  /* (ll_uri_t) */
  const ll_uri_t u;
  ll_locale_t *l;
  int i;
  PyObject *list;

  (void) self;
  if (!PyArg_ParseTuple (args, "s", &u))
    return NULL;
  l = ll_get_permits (u);
  i = 0;
  list = PyList_New (0);
  while (l != NULL && l[i] != NULL)
    {
      PyList_Append (list, PyString_FromString (l[i]));
      i++;
    }
  ll_free_list (l);
  return list;
}

static PyObject *
py_get_prohibits (PyObject *self, PyObject *args)
{
  /* (ll_uri_t) */
  const ll_uri_t u;
  ll_locale_t *l;
  int i;
  PyObject *list;

  (void) self;
  if (!PyArg_ParseTuple (args, "s", &u))
    return NULL;
  l = ll_get_prohibits (u);
  i = 0;
  list = PyList_New (0);
  while (l != NULL && l[i] != NULL)
    {
      PyList_Append (list, PyString_FromString (l[i]));
      i++;
    }
  ll_free_list (l);
  return list;
}

static PyObject *
py_get_requires (PyObject *self, PyObject *args)
{
  /* (ll_uri_t) */
  const ll_uri_t u;
  ll_locale_t *l;
  int i;
  PyObject *list;

  (void) self;
  if (!PyArg_ParseTuple (args, "s", &u))
    return NULL;
  l = ll_get_requires (u);
  i = 0;
  list = PyList_New (0);
  while (l != NULL && l[i] != NULL)
    {
      PyList_Append (list, PyString_FromString (l[i]));
      i++;
    }
  ll_free_list (l);
  return list;
}

static PyObject *
py_verify_uri (PyObject *self, PyObject *args)
{
  /* ll_uri_t */
  const ll_uri_t u;
  int b;

  (void) self;
  if (!PyArg_ParseTuple (args, "s", &u))
    return NULL;
  b = ll_verify_uri (u);
  return PyBool_FromLong ((long) b);
}

static PyObject *
py_get_attribute (PyObject *self, PyObject *args)
{
  /* ll_uri_t, ll_attribute_t */
  const ll_uri_t u;
  const ll_attribute_t a;
  int b;
  char **avs;
  int i;
  PyObject *list;

  (void) self;
  if (!PyArg_ParseTuple (args, "ssi", &u, &a, &b))
    return NULL;
  avs = ll_get_attribute (u, a, b);
  i = 0;
  list = PyList_New (0);
  while (avs != NULL && avs[i] != NULL)
    {
      PyList_Append (list, PyString_FromString (avs[i]));
      i++;
    }
  ll_free_list (avs);
  return list;
}

static PyObject *
py_get_licenses (PyObject *self, PyObject *args)
{
  /* ll_juris_t */
  ll_juris_t j = NULL;
  ll_uri_t *us;
  int i;
  PyObject *list;

  (void) self;
  if (PyArg_ParseTuple (args, "|s", &j))
    {
      if (j == NULL)
        us = ll_get_licenses (NULL);
      else
        us = ll_get_licenses (j);
    }
  else
    {
      return NULL;
    }
  i = 0;
  list = PyList_New (0);
  while (us != NULL && us[i] != NULL)
    {
      PyList_Append (list, PyString_FromString (us[i]));
      i++;
    }
  ll_free_list (us);
  return list;
}

static PyObject *
py_get_jurisdictions (PyObject *self, PyObject *args)
{
  ll_juris_t *j;
  int i;
  PyObject *list;

  (void) self;
  (void) args;
  j = ll_get_jurisdictions ();
  i = 0;
  list = PyList_New (0);
  while (j != NULL && j[i] != NULL)
    {
      PyList_Append (list, PyString_FromString (j[i]));
      i++;
    }
  ll_free_list (j);
  return list;
}

static PyObject *
py_jurisdiction_name (PyObject *self, PyObject *args)
{
  /* (ll_juris_t) */
  const ll_juris_t j;
  char *name;
  PyObject *result;

  (void) self;
  if (!PyArg_ParseTuple (args, "s", &j))
    return NULL;
  name = ll_jurisdiction_name (j);
  result = Py_BuildValue ("s", name);
  free (name);
  return result;
}

static PyObject *
py_write (PyObject *self, PyObject *args)
{
  /* ll_filename_t, ll_uri_t[,ll_module_t] */
  const ll_filename_t f;
  const ll_uri_t u;
  const ll_module_t m = NULL;
  int result;
  (void) self;
  if (PyArg_ParseTuple (args, "sz|s", &f, &u, &m))
    if (m != NULL)
      result = ll_module_write (f, u, m);
    else
      result = ll_write (f, u);
  else
    return NULL;
  /* just in case PyBool_FromLong thinks its true */
  if (result == -1)
    result = 0;
  return PyBool_FromLong ((long) result);
}

static PyObject *
py_read (PyObject *self, PyObject *args)
{
  /* ll_filename_t[,ll_module_t] */
  const ll_filename_t f;
  const ll_module_t m = NULL;
  ll_juris_t j;
  PyObject *result;

  (void) self;
  if (PyArg_ParseTuple (args, "s|s", &f, &m))
    if (m != NULL)
      j = ll_module_read (f, m);
    else
      j = ll_read (f);
  else
    return NULL;
  result = Py_BuildValue ("s", j);
  free (j);
  return result;
}

static PyObject *
py_license_default_set (PyObject *self, PyObject *args)
{
  /* ll_uri_t */
  ll_uri_t u;
  (void) self;
  if (!PyArg_ParseTuple (args, "z", &u))
    return NULL;
  return PyBool_FromLong ((long) ll_license_default_set (u));
}

static PyObject *
py_license_default_get (PyObject *self, PyObject *args)
{
  ll_uri_t u;
  PyObject *result;

  (void) self;
  (void) args;
  u = ll_license_default_get ();
  result = Py_BuildValue ("z", u);
  free (u);
  return result;
}

static PyObject *
py_get_config_modules (PyObject *self, PyObject *args)
{
  ll_module_t *ms;
  int i;
  PyObject *list;

  (void) self;
  (void) args;
  ms = ll_get_config_modules ();
  i = 0;
  list = PyList_New (0);
  while (ms != NULL && ms[i] != NULL)
    {
      PyList_Append (list, PyString_FromString (ms[i]));
      i++;
    }
  ll_free_list (ms);
  return list;
}

static PyObject *
py_get_io_modules (PyObject *self, PyObject *args)
{
  ll_module_t *ms;
  int i;
  PyObject *list;

  (void) self;
  (void) args;
  ms = ll_get_io_modules ();
  i = 0;
  list = PyList_New (0);
  while (ms != NULL && ms[i] != NULL)
    {
      PyList_Append (list, PyString_FromString (ms[i]));
      i++;
    }
  ll_free_list (ms);
  return list;
}

static PyObject *
py_print_module_info (PyObject *self, PyObject *args)
{
  (void) self;
  (void) args;
  ll_print_module_info ();
  return Py_BuildValue ("");
}

typedef struct LicenseChooser LicenseChooser;
struct LicenseChooser
{
  PyObject_HEAD ll_license_chooser_t *chooser;
};

static void
LicenseChooser_dealloc (LicenseChooser *self)
{
  if (self->chooser)
    ll_free_license_chooser (self->chooser);
  self->ob_type->tp_free ((PyObject *) self);
}

static int
LicenseChooser_init (LicenseChooser *self, PyObject *args, PyObject *kwds)
{
  ll_juris_t j;
  PyObject *attrs;
  const char **attributes;
  int i;
  ll_license_chooser_t *chooser;

  (void) kwds;
  j = NULL;
  attrs = NULL;

  if (!PyArg_ParseTuple (args, "zO", &j, &attrs))
    return -1;

  if (!PyList_Check (attrs))
    return -1;

  attributes = malloc ((PyList_Size (attrs) + 1) * sizeof (char *));
  attributes[PyList_Size (attrs)] = NULL;

  for (i = 0; i < PyList_Size (attrs); ++i)
    {
      PyObject *attr;
      char *string;

      attr = PyList_GetItem (attrs, i);
      if (!PyString_Check (attr))
        {
          free (attributes);
          return -1;
        }
      string = PyString_AsString (attr);
      attributes[i] = string;
    }

  chooser = ll_new_license_chooser (j, attributes);
  self->chooser = chooser;
  free (attributes);

  return 0;
}

static PyObject *
LicenseChooser_get_licenses (LicenseChooser *self, PyObject *args,
                             PyObject *kwds)
{
  static char *kwlist[] = { "permits", "requires", "prohibits", NULL };

  int permits = 0, requires = 0, prohibits = 0;
  const ll_license_list_t *license_list;
  const ll_license_list_t *curr;
  PyObject *list;

  if (!PyArg_ParseTupleAndKeywords
      (args, kwds, "|iii", kwlist, &permits, &requires, &prohibits))
    return NULL;

  license_list =
    ll_get_licenses_from_flags (self->chooser, permits, requires, prohibits);

  curr = license_list;
  list = PyList_New (0);
  while (curr != NULL)
    {
      PyList_Append (list, PyString_FromString (curr->license));
      curr = curr->next;
    }

  return list;
}

static PyObject *
LicenseChooser_attribute_flag (LicenseChooser *self, PyObject *args)
{
  char *attr;
  if (!PyArg_ParseTuple (args, "s", &attr))
    return NULL;

  return PyInt_FromLong ((long) ll_attribute_flag (self->chooser, attr));
}

static PyMethodDef LicenseChooser_methods[] =
{
  {"get_licenses", (PyCFunction) LicenseChooser_get_licenses,
   METH_VARARGS | METH_KEYWORDS,
   "Return a list of licenses matching the given flags"},
  {"attribute_flag", (PyCFunction) LicenseChooser_attribute_flag, METH_VARARGS,
   "Return the appropriate permits/requires/prohibits flag to be passed "
       "to get_licenses"},
  {NULL, 0, 0, 0} /* Sentinel */
};

static PyTypeObject LicenseChooserType =
{
  PyObject_HEAD_INIT(NULL)
  0,                            /*ob_size */
  "Liblicense.LicenseChooser",  /*tp_name */
  sizeof (LicenseChooser),      /*tp_basicsize */
  0,                            /*tp_itemsize */
  (destructor) LicenseChooser_dealloc, /*tp_dealloc */
  0,                            /*tp_print */
  0,                            /*tp_getattr */
  0,                            /*tp_setattr */
  0,                            /*tp_compare */
  0,                            /*tp_repr */
  0,                            /*tp_as_number */
  0,                            /*tp_as_sequence */
  0,                            /*tp_as_mapping */
  0,                            /*tp_hash */
  0,                            /*tp_call */
  0,                            /*tp_str */
  0,                            /*tp_getattro */
  0,                            /*tp_setattro */
  0,                            /*tp_as_buffer */
  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags */
  "Object for searching for licenses based on what the license permits, "
    "requires, and/or prohibits", /* tp_doc */
  0,                            /* tp_traverse */
  0,                            /* tp_clear */
  0,                            /* tp_richcompare */
  0,                            /* tp_weaklistoffset */
  0,                            /* tp_iter */
  0,                            /* tp_iternext */
  LicenseChooser_methods,       /* tp_methods */
  0,                            /* tp_members */
  0,                            /* tp_getset */
  0,                            /* tp_base */
  0,                            /* tp_dict */
  0,                            /* tp_descr_get */
  0,                            /* tp_descr_set */
  0,                            /* tp_dictoffset */
  (initproc) LicenseChooser_init, /* tp_init */
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
};

static PyMethodDef LicenseMethods[] =
{
  {"get_jurisdiction", py_get_jurisdiction, METH_VARARGS,
   "Get the jurisdiction of the given license uri."},
  {"get_name", py_get_name, METH_VARARGS,
   "Get the name of the given license uri."},
  {"get_version", py_get_version, METH_VARARGS,
   "Get the version of the given license uri."},
  {"get_permits", py_get_permits, METH_VARARGS,
   "Gets the uris of the actions permitted."},
  {"get_prohibits", py_get_prohibits, METH_VARARGS,
   "Gets the uris of the actions prohibited."},
  {"get_requires", py_get_requires, METH_VARARGS,
   "Gets the uris of the actions required."},
  {"verify_uri", py_verify_uri, METH_VARARGS,
   "Returns a boolean indicating if the uri is recognized by the system."},
  {"get_attribute", py_get_attribute, METH_VARARGS,
   "Returns a list of values found for the given attribute."},
  {"get_licenses", py_get_licenses, METH_VARARGS,
   "Returns a list of licenses in the optional jurisdiciton (otherwise "
     "all are given.)"},
  {"get_jurisdictions", py_get_jurisdictions, METH_NOARGS,
   "Returns a list of all jurisdictions."},
  {"jurisdiction_name", py_jurisdiction_name, METH_VARARGS,
   "Returns the jurisdiction name from the jurisdiction code."},
  {"write", py_write, METH_VARARGS,
   "Returns whether or not the license write succeeded."},
  {"read", py_read, METH_VARARGS,
   "Returns the license for the given file."},
  {"license_default_set", py_license_default_set, METH_VARARGS,
   "Sets the system default license to the given uri."},
  {"set_default", py_license_default_set, METH_VARARGS,
   "Sets the system default license to the given uri "
     "[DEPRECATED: use license_default_set instead]"},
  {"license_default_get", py_license_default_get, METH_VARARGS,
   "Returns the system default license."},
  {"get_default", py_license_default_get, METH_VARARGS,
   "Returns the system default license "
     "[DEPRECATED: use license_default_get instead]"},
  {"get_config_modules", py_get_config_modules, METH_VARARGS,
   "Returns a list of the available config modules."},
  {"get_io_modules", py_get_io_modules, METH_VARARGS,
   "Returns a list of the available io modules."},
  {"print_module_info", py_print_module_info, METH_VARARGS,
   "Print available modules and their capabilities"},
  {NULL, NULL, 0, NULL}         /* Sentinel */
};

PyMODINIT_FUNC
initliblicense (void)
{
  PyObject *m;

  ll_init ();
  m = Py_InitModule ("liblicense", LicenseMethods);
  PyModule_AddIntConstant (m, "UNSPECIFIED", LL_UNSPECIFIED);

  LicenseChooserType.tp_new = PyType_GenericNew;
  if (PyType_Ready (&LicenseChooserType) < 0)
    return;

  Py_INCREF (&LicenseChooserType);
  PyModule_AddObject (m, "LicenseChooser", (void *) &LicenseChooserType);
}
