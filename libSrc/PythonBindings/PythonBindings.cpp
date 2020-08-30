// Adam C. Burke
// PythonBindings.cpp
// 8/23/2020

/**
 * This is the master file that combines the various binding
 * implementations into the master mpsToolKit python3 module.
 * pybind11 required and shipped with this code.
 **/

#ifndef _WIN32
    #include "pybind11/pybind11.h"
#else
    #include "pybind11\pybind11.h"
#endif

#include "TransferFunctions_PyBindings.hpp"

namespace py = pybind11;

PYBIND11_MODULE(py_mpsToolkit, m){

    // call the module functions
    TransferFunctions(m);
}

