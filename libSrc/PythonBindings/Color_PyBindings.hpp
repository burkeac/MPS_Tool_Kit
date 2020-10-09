// Adam C. Burke
// Color_PyBinding.hpp
// 10/6/2020

#pragma once

#ifndef _WIN32
      #include <pybind11/pybind11.h>
      #include <pybind11/stl.h>
      #include <pybind11/eigen.h>
      #include <../Color/ColorSpaces.hpp> 
#else 
      #include <pybind11\pybind11.h>
      #include <pybind11\stl.h>
      #include <pybind11\eigen.h>
      #include <..\Color\ColorSpaces.hpp> 
#endif

namespace py = pybind11;

void Color(py::module &m){
    // Emum Bindings
    py::enum_<MPS::WhitePoint>(m, "WhitePoint")
        .value("D65", MPS::WhitePoint::D65)
        .value("D50", MPS::WhitePoint::D50);

    py::enum_<MPS::ColorSpaces>(m, "ColorSpaces")
        .value("Rec709" ,  MPS::ColorSpaces::Rec709)
        .value("Rec2020",  MPS::ColorSpaces::Rec2020)
        .value("P3D65"  ,  MPS::ColorSpaces::P3D65)
        .value("P3DCI"  ,  MPS::ColorSpaces::P3DCI)
        .value("ACES_P0",  MPS::ColorSpaces::ACES_P0)
        .value("ACES_P1",  MPS::ColorSpaces::ACES_P1);

    py::enum_<MPS::ChromaPoint>(m, "ChromaPoint")
        .value("xRed"    , MPS::ChromaPoint::xRed)
        .value("yRed"    , MPS::ChromaPoint::yRed)
        .value("zRed"    , MPS::ChromaPoint::zRed)
        .value("xGreen"  , MPS::ChromaPoint::xGreen)
        .value("yGreen"  , MPS::ChromaPoint::yGreen) 
        .value("zGreen"  , MPS::ChromaPoint::zGreen) 
        .value("xBlue"   , MPS::ChromaPoint::xBlue) 
        .value("yBlue"   , MPS::ChromaPoint::yBlue) 
        .value("zBlue"   , MPS::ChromaPoint::zBlue) 
        .value("xWhite"  , MPS::ChromaPoint::xWhite) 
        .value("yWhite"  , MPS::ChromaPoint::yWhite) 
        .value("zWhite"  , MPS::ChromaPoint::zWhite);

    // Class Bindings
    py::class_<MPS::colorPrimaries>(m, "colorPrimaries")
        .def(py::init<>())
        .def(py::init<MPS::ColorSpaces>())
        .def_readwrite("Primaries", &MPS::colorPrimaries::Primaries)
        .def_readwrite("WhitePoint", &MPS::colorPrimaries::WhitePoint)
        .def("selectPrimaries", &MPS::colorPrimaries::selectPrimaries, 
            "selects a predefined primary set", py::arg("PrimarySet"))
        .def("printPrimaries", &MPS::colorPrimaries::printPrimaries,
            "prints the primaries to std::out")
        .def("getChromaPoint", &MPS::colorPrimaries::getChromaPoint,
            "returns the x or y value of the specified primary", py::arg("corrdinate"));

    py::class_<MPS::phosphorMatrix>(m, "phosphorMatrix")
        .def(py::init<>())
        .def(py::init<MPS::colorPrimaries&>())
        .def(py::init<MPS::colorPrimaries&, MPS::colorPrimaries&>())
        .def(py::init<const MPS::ColorSpaces&, const MPS::ColorSpaces&>())
        .def("generatePM", &MPS::phosphorMatrix::generatePM,
            "Generates the forward and inverse PM")
        .def("getPM", &MPS::phosphorMatrix::getPM, 
            "Returns the PM as a numpy array")
        .def("getInvPM", &MPS::phosphorMatrix::getInvPM,
            "Returns the inverse PM as a numpy array");

    // Function bindings
    m.def("Rec2020_to_ICtCp", 
          &MPS::Rec2020_to_ICtCp, 
          "Converts code values between Rec2020 and ICtCp",
          py::arg("R"),
          py::arg("G"),
          py::arg("B"),
          py::arg("PQ"),
          py::arg("scaleToJNDs")
    );
    m.def("XYZ_to_cieLAB",
          &MPS::XYZ_to_cieLAB,
          "converts CIE XYZ values to CIELAB. XYZ values should be normalized so that Y = 100. WhitePoint options include D65 and D50",
          py::arg("X"),
          py::arg("Y"),
          py::arg("Z"),
          py::arg("whitePt")
    );
    m.def("cieLAB_to_XYZ",
          &MPS::cieLAB_to_XYZ,
          "LAB values are returned normalized so that Y = 100. WhitePoint options include D65 and D50",
          py::arg("L"),
          py::arg("A"),
          py::arg("B"),
          py::arg("whitePt")
    );
}