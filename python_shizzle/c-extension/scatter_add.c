#include "Python.h"

/* C function that will be wrapped as a Python function. */
static PyObject *scatter(PyObject *self, PyObject *args) {
  
}

static PyMethodDef scatter_func_table[] =
  {
   {
    "scatter",      // name exposed to Python
    scatter, // C wrapper function
    METH_VARARGS,          // received variable args (but really just 1)
    "scatter reduction using multiple threads." // documentation
   },
   {
    NULL, NULL, 0, NULL
   }
  };

static struct PyModuleDef scatter_add_module =
  {
   PyModuleDef_HEAD_INIT,       /* init a module. */
   "scatter_add",     // name of module exposed to Python
   "scatter addition sample.", // module documentation
   -1,
   scatter_func_table           /* function table for this module. */
  };

PyMODINIT_FUNC PyInit_scatter_add(void) {
  return PyModule_Create(&scatter_add_module);
}
