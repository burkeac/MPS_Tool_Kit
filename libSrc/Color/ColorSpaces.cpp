// ColorSpaces.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// July 28, 2019

#include <iostream>
#include "ColorSpaces.hpp"
#include "HDR_TranFunc.hpp"
#include "../Eigen/Core"
#include "../Eigen/Eigen"

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
    // Implementations for the colorPrimaries class
        // Constructors
    MPS::colorPrimaries::colorPrimaries(){
        Primaries = vector<float> {0,0,0,0,0,0}; 
        WhitePoint = vector<float> {0,0};
    }
    MPS::colorPrimaries::colorPrimaries(MPS::ColorSpaces colorSpace){
        MPS::colorPrimaries::selectPrimary(colorSpace);
    }
        // Methods of MPS colorPrimaries Class
    void MPS::colorPrimaries::selectPrimary(MPS::ColorSpaces colorspace){
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
        cout << "Red x: " << Primaries[0] << "\n"
             << "Red y: " << Primaries[1] << "\n"
             << "Green x: " << Primaries[2] << "\n"
             << "Green y: " << Primaries[3] << "\n"
             << "Blue x: " << Primaries[4] << "\n"
             << "Blue y: " << Primaries[5] << endl;
    }

    float MPS::colorPrimaries::getChromaPoint(ChromaPoint chromaPoint){
        switch(chromaPoint){
            case MPS::xRed: return(Primaries[0]); break;
            case MPS::yRed: return(Primaries[1]); break;
            case MPS::zRed: return(1. - Primaries[0] - Primaries[1]); break;

            case MPS::xGreen: return(Primaries[2]); break;
            case MPS::yGreen: return(Primaries[3]); break;
            case MPS::zGreen: return(1. - Primaries[2] - Primaries[3]); break;

            case MPS::xBlue: return(Primaries[4]); break;
            case MPS::yBlue: return(Primaries[5]); break;
            case MPS::zBlue: return(1. - Primaries[4] - Primaries[5]); break;

            case MPS::xWhite: return(WhitePoint[0]); break;
            case MPS::yWhite: return(WhitePoint[1]); break;
            case MPS::zWhite: return(1. - WhitePoint[0] - WhitePoint[1]); break;
            
            default: return(0); break;

        }
    };

    // Member Functions for phosphorMatrix Class
    // PM Constructor for single primary input. Used for going from a primary set to XYZ and vice-versa
    MPS::phosphorMatrix::phosphorMatrix(MPS::colorPrimaries primaries){
        generatePMs(primaries, 1, 1);
    };

    // PM Constructor for double primary input. Used for going from a primary to a second set and vice-versa
    MPS::phosphorMatrix::phosphorMatrix(MPS::colorPrimaries primarySet1, MPS::colorPrimaries primarySet2){
        Eigen::Matrix3f PM1 = generatePMs(primarySet1, 1, 1, false);
        Eigen::Matrix3f PM2 = generatePMs(primarySet2, 1, 1, false);
        _PM = PM2.inverse() * PM1;
        _invPM = _PM.inverse();
    };

    // Generate and return the phosphore matrix. 
    // Also sets the the _PM memember
    Eigen::Matrix3f MPS::phosphorMatrix::generatePMs(MPS::colorPrimaries primaries, 
                                                    float actualLum, 
                                                    float aimLum, 
                                                    bool setMembers){

        // Fill the (C) Matrix
        Eigen::Matrix3f C_matrix(3,3);
        C_matrix << primaries.getChromaPoint(xRed), 
                    primaries.getChromaPoint(xGreen),
                    primaries.getChromaPoint(xBlue),
                    primaries.getChromaPoint(yRed),
                    primaries.getChromaPoint(yGreen),
                    primaries.getChromaPoint(yBlue),
                    primaries.getChromaPoint(zRed),
                    primaries.getChromaPoint(zGreen),
                    primaries.getChromaPoint(zBlue);

        //load Whitepoint vector
        Eigen::Vector3f W_vector;
        W_vector << primaries.getChromaPoint(xWhite),
                    primaries.getChromaPoint(yWhite),
                    primaries.getChromaPoint(zWhite);

        // Luminance scalar
        float lumScalar = (actualLum/aimLum)/primaries.getChromaPoint(yWhite);

        // multiply the white point vector times the luma scalars to normalize.
        W_vector = W_vector * lumScalar;

        // Multiply the C^-1 matrix by the normalized whitepoint vector 
        // creating the diagonalized J-matrix
        Eigen::Vector3f J_vector = C_matrix.inverse() * W_vector;

        //Calculate the PM
        Eigen::Matrix3f PM = C_matrix * J_vector.asDiagonal();

        // Set the private member and return the matrix.
        if(setMembers){
            _PM = PM;
            _invPM = PM.inverse();
        }
        return PM;
    };

    Eigen::Matrix3f MPS::phosphorMatrix::getPM(){
        return _PM;
    };

    Eigen::Matrix3f MPS::phosphorMatrix::getInvPM(){
        return _invPM;
    };

    // Linear RGB Rec 2020 CVs normalized 0 to 1.
    // Returns std::vector of floats in I Ct Cp format.
    std::vector<float> Rec2020_to_ICtCp(float R, float G, float B){
        float L = (1688.0 * R + 2146.0 * G + 262.0  * B) / 4096.0;
        float M = (683.0  * R + 2951.0 * G + 462.0  * B) / 4096.0;
        float S = (99.0   * R + 309.0  * G + 3688.0 * B) / 4096.0;

        float L_prime = MPS::Linear_2_PQ(L);
        float M_prime = MPS::Linear_2_PQ(M);
        float S_prime = MPS::Linear_2_PQ(S);

        float I  = 0.5 * L_prime + 0.5 * M_prime;
        float Ct = (6610.0 * L_prime - 13613.0 * M_prime + 7003.0 * S_prime) / 4096.0;
        float Cp = (17933.0 * L_prime - 17390.0 * M_prime - 543.0 * S_prime) / 4096.0;

        vector<float> ICtCp;
        ICtCp.push_back(I);
        ICtCp.push_back(Ct);
        ICtCp.push_back(Cp);
        return ICtCp;
    }
}
