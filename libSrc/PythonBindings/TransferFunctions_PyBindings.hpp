// Adam C. Burke
// TransferFunctions_PyBinding.hpp
// 8/23/2020

#pragma once

#include <pybind11/pybind11.h>
#include <../TransferFunctions/HDR_TranFunc.hpp> 

// if win32

namespace py = pybind11;

void TransferFunctions(py::module &m){
    m.def("Linear_2_PQ", 
          &MPS::Linear_2_PQ, 
          "Converts code values between a linear encoding and the 'PQ' encoding",
          py::arg("input")
        );
    m.def("PQ_2_Linear",
          &MPS::PQ_2_Linear,
          "Converts code values between the 'PQ' encoding and a linear encoding",
          py::arg("input")
        );
    m.def("Linear_2_HLG",
          &MPS::Linear_2_HLG,
          "Converts code values between linear light and HLG encoded value",
          py::arg("input")
          );
    m.def("HLG_2_Linear",
          &MPS::HLG_2_Linear,
          "Converts code values betweeen HLG encoded values and linear light",
          py::arg("input")
          );
}