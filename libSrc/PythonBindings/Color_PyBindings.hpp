// Adam C. Burke
// Color_PyBinding.hpp
// 10/6/2020

#pragma once

#ifndef _WIN32
      #include <pybind11/pybind11.h>
      #include <pybind11/stl.h>
      #include <pybind11/eigen.h>
      #include <pybind11/functional.h>
      #include "../Color/ColorSpaces.hpp"
      #include "../Color/deltaE.hpp"
      #include "../Color/LUT.hpp"
#else 
      #include <pybind11\pybind11.h>
      #include <pybind11\stl.h>
      #include <pybind11\eigen.h>
      #include <pybind11\functional.h>
      #include "..\Color\ColorSpaces.hpp"
      #include "..\Color\deltaE.hpp"
      #include "..\Color\LUT.hpp"
#endif

namespace py = pybind11;

namespace MPS::PyBindings{

    /* 
    this class is to work around the return by reference C++ API since pass 
    by ref/pointer is not supported in Python
    */
    class _py_LUT3D : public MPS::LUT3D{
        public:
        CUBE_Params _py_LUT3DReadFromCubeFile(std::string &filePath){
            
            CUBE_Params params;
            ReadFromCubeFile("/Users/adamburke/Documents/2019/MPS_Toolkit/build/CubeTest.cube", &params);
            
            return params;
        }
    };

}

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


    // 3D LUT class
    py::class_<MPS::PyBindings::_py_LUT3D> LUT3D(m, "LUT3D");
    LUT3D.def(py::init<>())
        .def(py::init<const uint8_t>())
        .def("numNodes", &MPS::PyBindings::_py_LUT3D::GetNumNodes)
        .def("Generate_3DLUT_Nodes", &MPS::LUT3D::Generate_3DLUT_Nodes,
            "Generates the inital code values so that in = out. aka a NULL LUT")
        .def("Write2CSV", &MPS::PyBindings::_py_LUT3D::Write2CSV, 
            "Writes the data to a CSV file in the RGB format, where the blue channel changes most frequently.",
             py::arg("filePath"), 
             py::arg("writeHeaders") = true, 
             py::arg("scale_bitdepth") = 0
        )
        .def("Write2CUBE_Adobe", &MPS::PyBindings::_py_LUT3D::Write2CUBE_Adobe,
            "Writes to an Adobe(C)/IRIDAS(C) .cube file",
            py::arg("filePath"),
            py::arg("Params") = nullptr // Note here nullptr must be used, opposed to NULL
        )
        .def("Write2CUBE_BM", &MPS::PyBindings::_py_LUT3D::Write2CUBE_BM,
            "Writes to a Black Magic .cube file",
            py::arg("filePath"),
            py::arg("Params") = nullptr
        )
        .def("ReadFromCSV", &MPS::PyBindings::_py_LUT3D::ReadFromCSV,
            "Reads from a specified CSV file containing 3D LUT data.",
            py::arg("filePath"),
            py::arg("scale_bitdepth") = 0
        )
        .def("ReadFromCubeFile", &MPS::PyBindings::_py_LUT3D::_py_LUT3DReadFromCubeFile,
            "Reads from a specified CSV file containing 3D LUT data.",
            py::arg("filePath")
        )
        .def("Interpolate_Trilin", &MPS::PyBindings::_py_LUT3D::Interpolate_Trilin,
            "Returns the interpolated values of the supplied 3D LUT using the trilinear interpolation method.",
            py::arg("Red CV"),
            py::arg("Green CV"),
            py::arg("Blue CV")
        )
        .def("Interpolate_Tetra", &MPS::PyBindings::_py_LUT3D::Interpolate_Tetra,
            "Returns the interpolated values of the supplied 3D LUT using the tetrahedral interpolation method.",
            py::arg("Red CV"),
            py::arg("Green CV"),
            py::arg("Blue CV")
        )
        .def("GenerateFromMatrix", &MPS::PyBindings::_py_LUT3D::GenerateFromMatrix,
            "Generates a 3D LUT using a supplied 3 x N NumPy matrix",
             py::arg("NumPy Matrix (3 x N)" )
        )
        .def("GenerateFromFunction", 
            py::overload_cast<std::function<MPS::tripletF(float, float, float)> &>
                (&MPS::PyBindings::_py_LUT3D::GenerateFromFunction),
            "Generates a 3D LUT using a supplied function. The supplied function should return a 3 element list of \
            RGB (0-1). 3 arguments should be supplied (in RGB order) values should be considered to be normalized \
            0-1",
            py::arg("function")
        );


    // LUT3D Subclass
    py::class_<MPS::LUT3D::CUBE_Params>(LUT3D, "CUBE_Params")
        .def(py::init<>())
        .def_readwrite("TITLE", &MPS::LUT3D::CUBE_Params::TITLE)
        .def_readwrite("DOMAIN_MIN", &MPS::LUT3D::CUBE_Params::DOMAIN_MIN)
        .def_readwrite("DOMAIN_MAX", &MPS::LUT3D::CUBE_Params::DOMAIN_MAX);


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