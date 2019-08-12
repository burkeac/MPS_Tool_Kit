// ColorSpaces.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// July 28, 2019

#include <iostream>
#include <vector>
#include "ColorSpaces.hpp"

using namespace std;

// Primaries in format: {Xr, Yr, Xg, Yg, Xb, Yb}
// White Point in format: {Xw, Yw}
const vector<float> Rec709_Primaries  =  {0.64, 0.33, 0.30, 0.60, 0.15, 0.06};
const vector<float> Rec709_WhitePoint =  {0.3127, 0.3290};
const vector<float> Rec2020_Primaries =  {0.708, 0.292, 0.17, 0.797, 0.131, 0.046};
const vector<float> Rec2020_WhitePoint = {0.3127, 0.3290};
const vector<float> P3D65_Primaries    = {0.680, 0.320, 0.265, 0.690, 0.150, 0.060};
const vector<float> P3D65_WhitePoint   = {0.3127, 0.3290};
const vector<float> P3DCI_Primaries    = {0.680, 0.320, 0.265, 0.690, 0.150, 0.060};
const vector<float> P3DCI_WhitePoint   = {0.314, 0.351};
const vector<float> ACES_P0_Primaries  = {0.7347, 0.2653, 0.0, 1.0, 0.0001, -0.0770};
const vector<float> ACES_P1_Primaries  = {0.713, 0.293, 0.165, 0.830, 0.128, 0.044};
const vector<float> ACES_WhitePoint    = {0.32168, 0.33767};

namespace MPS{
    //Member Functions of MPS colorPrimaries Class
    MPS::colorPrimaries::colorPrimaries(){
        Primaries = vector<float> {0,0,0,0,0,0}; 
        WhitePoint = vector<float> {0,0};
    }
    MPS::colorPrimaries::colorPrimaries(int colorSpace){
        MPS::colorPrimaries::selectPrimary(colorSpace);
    }
    void MPS::colorPrimaries::selectPrimary(int colorspace){
        switch (colorspace){
            case MPS::Rec709: 
                Primaries = Rec709_Primaries; 
                WhitePoint = Rec709_WhitePoint;
                break;
            case MPS::Rec2020: 
                Primaries = Rec2020_Primaries;
                WhitePoint = Rec2020_WhitePoint;
                break;
            case MPS::P3D65:
                Primaries = P3D65_Primaries;
                WhitePoint = P3D65_WhitePoint;
                break;
            case MPS::P3DCI:
                Primaries = P3DCI_Primaries;
                WhitePoint = P3DCI_WhitePoint;
                break;
            case MPS::ACES_P0:
                Primaries = ACES_P0_Primaries;
                WhitePoint = ACES_WhitePoint;
                break;
            case MPS::ACES_P1:
                Primaries = ACES_P0_Primaries;
                WhitePoint = ACES_WhitePoint;
            default: 
                Primaries = vector<float> {0,0,0,0,0,0}; 
                WhitePoint = vector<float> {0,0};
                break;
        }
    }

    void MPS::colorPrimaries::printPrimaries(){
        cout << "Red x: " << Primaries[0] << endl;
        cout << "Red y: " << Primaries[1] << endl;
        cout << "Green x: " << Primaries[2] << endl;
        cout << "Green y: " << Primaries[3] << endl;
        cout << "Blue x: " << Primaries[4] << endl;
        cout << "Blue y: " << Primaries[5] << endl;
    }

// Member Functions for phosphorMatrix Class
    MPS::phosphorMatrix::phosphorMatrix(MPS::colorPrimaries primaries){
        cout << "1 vars" << endl;
    };
    MPS::phosphorMatrix::phosphorMatrix(MPS::colorPrimaries primarySet1, MPS::colorPrimaries primarySet2){
        cout << "2 vars" << endl;
    };

    MPS::phosphorMatrix::phosphorMatrix(MPS::colorPrimaries primaries){
        
    }
}
