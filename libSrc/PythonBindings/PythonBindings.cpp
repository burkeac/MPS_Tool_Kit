// Adam C. Burke
// PythonBindings.cpp
// 8/23/2020

/**
 * This is the master file that combines the various binding
 * implementations into the master mpsToolKit python3 module.
 * pybind11 required and shipped with this code.
 **/

#include "pybind11/pybind11.h"


    // include the hpps with module functions
#include "TransferFunctions_PyBindings.hpp"

                // {Scrap This}
                int myAdd(int a, int b){
                    return a + b;
                }

namespace py = pybind11;

PYBIND11_MODULE(py_mpsToolKit, m){

    // call the module functions
    TransferFunctions(m);

            // {Scrap This}
            m.def("myAdd", &myAdd, "a function that addes two numbers!", 
                py::arg("a"), // tell python what the argument's name is
                py::arg("b") // tell python what the argument's name is
                );
}

