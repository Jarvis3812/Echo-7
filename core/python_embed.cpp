// Python embedding utilities for Riley Corpbrain
#include "python_embed.h"
#include <Python.h>
// ...implement Python interpreter initialization and function calls...

#include <Python.h>
#include <string>
#include <map>

class PythonEmbed {
public:
    PythonEmbed() {
        Py_Initialize();
        PyRun_SimpleString("import sys; sys.path.append('./core')");
        pModule = PyImport_ImportModule("riley_ai");
    }
    ~PythonEmbed() {
        Py_XDECREF(pModule);
        Py_Finalize();
    }
    PyObject* callFunction(const std::string& funcName, const std::map<std::string, double>& args) {
        if (!pModule) return nullptr;
        PyObject* pFunc = PyObject_GetAttrString(pModule, funcName.c_str());
        if (!pFunc || !PyCallable_Check(pFunc)) return nullptr;
        PyObject* pArgs = PyDict_New();
        for (const auto& kv : args) {
            PyDict_SetItemString(pArgs, kv.first.c_str(), PyFloat_FromDouble(kv.second));
        }
        PyObject* pValue = PyObject_CallFunctionObjArgs(pFunc, pArgs, NULL);
        Py_XDECREF(pFunc);
        Py_XDECREF(pArgs);
        return pValue;
    }
private:
    PyObject* pModule;
};
