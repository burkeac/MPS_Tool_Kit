// ColorSpaces.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// July 28, 2019

#include <iostream>
#include <cmath>
#include "ColorSpaces.hpp"
#include "HDR_TranFunc.hpp"

#ifndef _WIN32
    #include "../Eigen/Core"
    #include "../Eigen/Eigen"
#else
    #include "..\Eigen\Core"
    #include "..\Eigen\Eigen"
#endif

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
    // default constructor inititates with identity matrix
    MPS::phosphorMatrix::phosphorMatrix(){
        _PM << 1,0,0,0,1,0,0,0,1;
        _invPM << 1,0,0,0,1,0,0,0,1;
    }

    // PM Constructor for single primary input. Used for going from a primary set to XYZ and vice-versa
    MPS::phosphorMatrix::phosphorMatrix(MPS::colorPrimaries& primaries){
        generatePMs(primaries, 1, 1);
    };

    // PM Constructor for double primary input. Used for going from a primary to a second set and vice-versa
    MPS::phosphorMatrix::phosphorMatrix(MPS::colorPrimaries& primarySet1, MPS::colorPrimaries& primarySet2){
        Eigen::Matrix3f PM1 = generatePMs(primarySet1, 1, 1, false);
        Eigen::Matrix3f PM2 = generatePMs(primarySet2, 1, 1, false);
        _PM = PM2.inverse() * PM1;
        _invPM = _PM.inverse();
    };

    // Generate and return the phosphore matrix. 
    // Also sets the the _PM memember
    Eigen::Matrix3f MPS::phosphorMatrix::generatePMs(MPS::colorPrimaries& primaries, 
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

    // Return the phosphore matrix as an Eigen::Matrix3f
    Eigen::Matrix3f MPS::phosphorMatrix::getPM(){
        return _PM;
    };
    
    // Return the phosphore matrix as an Eigen::Matrix3f
    Eigen::Matrix3f MPS::phosphorMatrix::getInvPM(){
        return _invPM;
    };

// Linear RGB Rec 2020 CVs
    // Returns std::vector of floats in I Ct Cp format.
    std::vector<float> Rec2020_to_ICtCp(float R, float G, float B, bool PQ, bool scaleToJNDs){
     
        //Convert RGB to LMS
        Eigen::Vector3f RGB; RGB << R, G, B;
        Eigen::Matrix3f RGBtoLMS; 
        RGBtoLMS << 1688.0, 2146.0, 262.0, 683.0, 2951.0, 462.0, 99.0, 309.0, 3688.0;
        Eigen::Vector3f LMS = (1./4096.) * RGBtoLMS* RGB;

        // Make Non-linear LMS
        if(PQ){
            LMS(0) = Linear_2_PQ(LMS(0));
            LMS(1) = Linear_2_PQ(LMS(1));
            LMS(2) = Linear_2_PQ(LMS(2));
        }else{
            LMS(0) = Linear_2_HLG(LMS(0));
            LMS(1) = Linear_2_HLG(LMS(1));
            LMS(2) = Linear_2_HLG(LMS(2));
        }

        Eigen::Matrix3f LMStoICtCp; 
        LMStoICtCp << 2048.0, 2048.0, 0.0, 6610.0, -13613.0, 7003.0, 17933.0, -17390.0, -543.0;
        
        Eigen::Vector3f ICtCp = (1./4096.) * LMStoICtCp * LMS;

        vector<float> result;
        if (scaleToJNDs) result = { ICtCp(0) * (float)720.0, ICtCp(1) * (float)360.0, ICtCp(2) * (float)720.0};
        else result = { ICtCp(0), ICtCp(1), ICtCp(2)};

        return result;
        
    }

// XYZ to CIE LAB

    double f_of_t(double t){
        const double delta = 6.0/29.0;
        if( t > (delta) )
            return(std::cbrt(t));
        else
            return( t/(3.0 * (delta * delta) ) + (4/29) );
    }

    std::vector<double> XYZ_to_cieLAB(const float& X, 
                                      const float& Y, 
                                      const float& Z, 
                                      const WhitePoint& whitePt){
        
        double Xn, Yn, Zn;
        
        switch (whitePt){
            case D65:
                Xn = 95.0489;
                Yn = 100.0;
                Zn = 108.884;
                break;

            case D50:
                Xn = 96.4212;
                Yn = 100.0;
                Zn = 82.5188;
                break;
        
            default:
                throw "Not a supported white point!";
                break;
        }

        std::vector<double> LAB;
        LAB.push_back(116.0 * f_of_t(Y/Yn) - 16.0);
        LAB.push_back(500.0 * (f_of_t(X/Xn) - f_of_t(Y/Yn) ) );
        LAB.push_back(200.0 * (f_of_t(Y/Yn) - f_of_t(Z/Zn) ));

        return(LAB);
    }

    double inverse_f_of_t(double t){
        const double delta = 6.0/29.0;
        if(t > delta)
            return(std::pow(t, 3));
        else 
            return(3.0 * std::pow(delta, 2) * ( t - (4.0/29.0) ) );
    }

    std::vector<double> cieLAB_to_XYZ(const float& L, 
                                      const float& A, 
                                      const float& B, 
                                      const WhitePoint& whitePt){

        double Xn, Yn, Zn;

        switch (whitePt){
            case D65:
                Xn = 95.0489;
                Yn = 100.0;
                Zn = 108.884;
                break;

            case D50:
                Xn = 96.4212;
                Yn = 100.0;
                Zn = 82.5188;
                break;
        
            default:
                throw "Not a supported white point!";
                break;
        }

        std::vector<double> XYZ;
        XYZ.push_back(Xn * inverse_f_of_t( (L + 16)/116.0 + A/500.0 ));
        XYZ.push_back(Yn * inverse_f_of_t( (L + 16.0)/116 ));
        XYZ.push_back(Zn * inverse_f_of_t( (L + 16.0)/116 - B/200.0 ));

        return(XYZ);
    }
}
