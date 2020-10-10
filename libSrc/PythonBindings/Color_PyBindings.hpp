// Adam C. Burke
// Color_PyBinding.hpp
// 10/6/2020

#pragma once

#ifndef _WIN32
      #include <pybind11/pybind11.h>
      #include <pybind11/stl.h>
      #include <pybind11/eigen.h>
      #include "../Color/ColorSpaces.hpp"
      #include "../Color/deltaE.hpp"
#else 
      #include <pybind11\pybind11.h>
      #include <pybind11\stl.h>
      #include <pybind11\eigen.h>
      #include "..\Color\ColorSpaces.hpp"
      #include "..\Color\deltaE.hpp"
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

    py::class_<MPS::CIEdeltaE>(m, "CIEdeltaE")
        .def(py::init<>())
        .def(py::init<const double&,const double&,const double&,const double&,const double&,const double&>())
                    // this type casting/punning is needed because of the overloaded (static) functions
                    // see: https://pybind11.readthedocs.io/en/stable/classes.html#overloaded-methods
        .def("cie76", (double (MPS::CIEdeltaE::*)() const) &MPS::CIEdeltaE::cie76)
        .def("cie94", (double (MPS::CIEdeltaE::*)(const bool&) const) &MPS::CIEdeltaE::cie94, "Calculates CIE Delta E 1994", py::arg("graphicArts"))
        .def("cie2000", (double (MPS::CIEdeltaE::*)() const) &MPS::CIEdeltaE::cie2000, "Calculates CIE Delta E 2000");


    py::class_<MPS::CIEdeltaE_frmXYZ, MPS::CIEdeltaE>(m, "CIEdeltaE_frmXYZ")
        .def(py::init<const double&,const double&,const double&,const double&,const double&,const double&, const MPS::WhitePoint&>());

    // Function bindings
    // Rec2020_to_ICtCp
    m.def("Rec2020_to_ICtCp", 
          &MPS::Rec2020_to_ICtCp, 
          "Converts code values between Rec2020 and ICtCp",
          py::arg("R"),
          py::arg("G"),
          py::arg("B"),
          py::arg("PQ"),
          py::arg("scaleToJNDs")
    );

    // XYZ_to_cieLAB
    m.def("XYZ_to_cieLAB",
          &MPS::XYZ_to_cieLAB,
          "converts CIE XYZ values to CIELAB. XYZ values should be normalized so that Y = 100. WhitePoint options include D65 and D50",
          py::arg("X"),
          py::arg("Y"),
          py::arg("Z"),
          py::arg("whitePt")
    );

    // cieLAB_to_XYZ
    m.def("cieLAB_to_XYZ",
          &MPS::cieLAB_to_XYZ,
          "LAB values are returned normalized so that Y = 100. WhitePoint options include D65 and D50",
          py::arg("L"),
          py::arg("A"),
          py::arg("B"),
          py::arg("whitePt")
    );
}