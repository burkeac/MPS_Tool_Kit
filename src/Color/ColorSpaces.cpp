// ColorSpaces.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// July 28, 2019

#include <iostream>
#include <vector>
#include "ColorSpaces.hpp"
#include "HDR_TranFunc.hpp"

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

    float MPS::colorPrimaries::getChromaPoint(int chromaPoint){
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
        cv::Mat PM1 = generatePMs(primarySet1, 1, 1, false);
        cv::Mat PM2 = generatePMs(primarySet2, 1, 1, false);
        _PM = PM2.inv() * PM1;
        _invPM = _PM.inv();
    };

    // Generate and return the phosphore matrix. 
    // Also sets the the _PM memember
    cv::Mat MPS::phosphorMatrix::generatePMs(MPS::colorPrimaries primaries, float actualLum, float aimLum, bool setMembers){
        cv::Mat PM, C_matrix, C_inverse, W_vector, J_Matrix;
        PM.create(3,3,CV_32F);
        C_matrix.create(3,3,CV_32F);
        C_inverse.create(3,3,CV_32F);
        W_vector.create(3,3,CV_32F);

        // Fill the (C) Matrix
        C_matrix.at<float>(0,0) = primaries.getChromaPoint(MPS::ChromaPoint::xRed);
        C_matrix.at<float>(0,1) = primaries.getChromaPoint(MPS::ChromaPoint::xGreen);
        C_matrix.at<float>(0,2) = primaries.getChromaPoint(MPS::ChromaPoint::xBlue);
        C_matrix.at<float>(1,0) = primaries.getChromaPoint(MPS::ChromaPoint::yRed);
        C_matrix.at<float>(1,1) = primaries.getChromaPoint(MPS::ChromaPoint::yGreen);
        C_matrix.at<float>(1,2) = primaries.getChromaPoint(MPS::ChromaPoint::yBlue);
        C_matrix.at<float>(2,0) = primaries.getChromaPoint(MPS::ChromaPoint::zRed);
        C_matrix.at<float>(2,1) = primaries.getChromaPoint(MPS::ChromaPoint::zGreen);
        C_matrix.at<float>(2,2) = primaries.getChromaPoint(MPS::ChromaPoint::zBlue);

        //Get inverse C matrix 
        C_inverse = C_matrix.inv();

        //load Whitepoint vector
        W_vector.at<float>(0,0) = primaries.getChromaPoint(MPS::ChromaPoint::xWhite);
        W_vector.at<float>(1,0) = primaries.getChromaPoint(MPS::ChromaPoint::yWhite);
        W_vector.at<float>(2,0) = primaries.getChromaPoint(MPS::ChromaPoint::zWhite);

        // Luminance scalar
        float lumScalar = (actualLum/aimLum)/primaries.getChromaPoint(MPS::ChromaPoint::yWhite);

        // multiply the white point vector times the luma scalars to normalize.
        W_vector = W_vector * lumScalar;

        // Multiply the C-1 matrix by the normalized whitepoint vector creating the diagonalized J-matrix
        J_Matrix.create(3,3,CV_32F);
        J_Matrix = J_Matrix * 0;

        J_Matrix.at<float>(0,0) = 
            C_inverse.at<float>(0,0) * W_vector.at<float>(0,0) 
            + C_inverse.at<float>(0,1) * W_vector.at<float>(1,0) 
            + C_inverse.at<float>(0,2) * W_vector.at<float>(2,0);
        J_Matrix.at<float>(1,1) = 
            C_inverse.at<float>(1,0) * W_vector.at<float>(0,0) 
            + C_inverse.at<float>(1,1) * W_vector.at<float>(1,0) 
            + C_inverse.at<float>(1,2) * W_vector.at<float>(2,0);
        J_Matrix.at<float>(2,2) = 
            C_inverse.at<float>(2,0) * W_vector.at<float>(0,0) 
            + C_inverse.at<float>(2,1) * W_vector.at<float>(1,0) 
            + C_inverse.at<float>(2,2) * W_vector.at<float>(2,0);

        //Calculate the PM
        PM = C_matrix * J_Matrix;

        // Set the private member and return the matrix.
        if(setMembers){
            _PM = PM;
            _invPM = PM.inv();
        }
        return PM;
    };

    cv::Mat MPS::phosphorMatrix::getPM(){
        return _PM;
    };

    cv::Mat MPS::phosphorMatrix::getInvPM(){
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
