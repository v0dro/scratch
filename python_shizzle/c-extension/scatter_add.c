#include "Python.h"

static struct PyModuleDef scatter_add_module = {
    PyModuleDef_HEAD_INIT,
    "scatter_add",     // name of module exposed to Python
    "scatter addition sample.", // module documentation
    -1,
    NULL//DemoLib_FunctionsTable
};

PyMODINIT_FUNC PyInit_scatter_add(void) {
    return PyModule_Create(&scatter_add_module);
}
