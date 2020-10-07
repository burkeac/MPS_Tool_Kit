// ColorSpaces.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// July 28, 2019

#include <iostream>
#include <cmath>
#include "ColorSpaces.hpp"
#include "HDR_TranFunc.hpp"

#ifndef _WIN32
    #include "../../ThirdParty/Eigen/Core"
    #include "../../ThirdParty/Eigen/Eigen"
#else
    #include "..\..\ThirdParty\Eigen\Core"
    #include "..\..\ThirdParty\Eigen\Eigen"
#endif

using namespace std;

// Primaries in format: {Xr, Yr, Xg, Yg, Xb, Yb}
// White Point in format: {Xw, Yw}
const array<float, 6> Rec709_Primaries  =  {0.64f, 0.33f, 0.30f, 0.60f, 0.15f, 0.06f};
const array<float, 2> Rec709_WhitePoint =  {0.3127f, 0.3290f};
const array<float, 6> Rec2020_Primaries =  {0.708f, 0.292f, 0.17f, 0.797f, 0.131f, 0.046f};
const array<float, 2> Rec2020_WhitePoint = {0.3127f, 0.3290f};
const array<float, 6> P3D65_Primaries    = {0.680f, 0.320f, 0.265f, 0.690f, 0.150f, 0.060f};
const array<float, 2> P3D65_WhitePoint   = {0.3127f, 0.3290f};
const array<float, 6> P3DCI_Primaries    = {0.680f, 0.320f, 0.265f, 0.690f, 0.150f, 0.060f};
const array<float, 2> P3DCI_WhitePoint   = {0.314f, 0.351f};
const array<float, 6> ACES_P0_Primaries  = {0.7347f, 0.2653f, 0.0f, 1.0f, 0.0001f, -0.0770f};
const array<float, 6> ACES_P1_Primaries  = {0.713f, 0.293f, 0.165f, 0.830f, 0.128f, 0.044f};
const array<float, 2> ACES_WhitePoint    = {0.32168f, 0.33767f};

namespace MPS{
// Implementations for the colorPrimaries class
    // Constructors
    MPS::colorPrimaries::colorPrimaries(){
        Primaries = array<float, 6> {0,0,0,0,0,0}; 
        WhitePoint = array<float, 2> {0,0};
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
                Primaries = array<float, 6> {0,0,0,0,0,0}; 
                WhitePoint = array<float, 2> {0,0};
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
        generatePM(primaries, 1, 1);
    };

    MPS::phosphorMatrix::phosphorMatrix(const MPS::ColorSpaces& colorSpace){
        colorPrimaries primaries(colorSpace);
        generatePM(primaries);
    }

    // PM Constructor for double primary input. Used for going from a primary to a second set and vice-versa
    MPS::phosphorMatrix::phosphorMatrix(MPS::colorPrimaries& primarySet1, MPS::colorPrimaries& primarySet2){
        Eigen::Matrix3f PM1 = _generatePM(primarySet1, 1, 1);
        Eigen::Matrix3f PM2 = _generatePM(primarySet2, 1, 1);
        _PM = PM2.inverse() * PM1;
        _invPM = _PM.inverse();
    };

    MPS::phosphorMatrix::phosphorMatrix(const MPS::ColorSpaces& colorSpace1, 
                                        const MPS::ColorSpaces& colorSpace2){
        colorPrimaries primarySet1(colorSpace1);
        colorPrimaries primarySet2(colorSpace2);
        Eigen::Matrix3f PM1 = _generatePM(primarySet1, 1, 1);
        Eigen::Matrix3f PM2 = _generatePM(primarySet2, 1, 1);
        _PM = PM2.inverse() * PM1;
        _invPM = _PM.inverse();
    }

    // Generate and return the phosphore matrix. 
    // Also sets the the _PM memember
    Eigen::Matrix3f MPS::phosphorMatrix::_generatePM(MPS::colorPrimaries& primaries, 
                                                     const float& actualLum, 
                                                     const float& aimLum){

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

        return PM;
    };

    Eigen::Matrix3f& MPS::phosphorMatrix::generatePM(MPS::colorPrimaries& primaries, 
                                                     const float& actualLum, 
                                                     const float& aimLum){
        _PM = _generatePM(primaries, actualLum, aimLum);
        _invPM = _PM.inverse();
        return _PM;
    }


    // Return the phosphore matrix as an Eigen::Matrix3f
    const Eigen::Matrix3f& MPS::phosphorMatrix::getPM() const{
        return _PM;
    };
    
    // Return the phosphore matrix as an Eigen::Matrix3f
    const Eigen::Matrix3f& MPS::phosphorMatrix::getInvPM() const{
        return _invPM;
    };

// Linear RGB Rec 2020 CVs
    // Returns std::array<float, 3> AKA MPS::tripletF of floats in I Ct Cp format.
    MPS::tripletF Rec2020_to_ICtCp(const float& R, const float& G, const float& B, 
                                   const bool& PQ, const bool& scaleToJNDs){
     
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

        MPS::tripletF result;
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

    MPS::tripletD XYZ_to_cieLAB(const float& X, 
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

        MPS::tripletD LAB;
        LAB[0] = (116.0 * f_of_t(Y/Yn) - 16.0);
        LAB[1] = (500.0 * (f_of_t(X/Xn) - f_of_t(Y/Yn) ) );
        LAB[2] = (200.0 * (f_of_t(Y/Yn) - f_of_t(Z/Zn) ));

        return(LAB);
    }

    double inverse_f_of_t(double t){
        const double delta = 6.0/29.0;
        if(t > delta)
            return(std::pow(t, 3));
        else 
            return(3.0 * std::pow(delta, 2) * ( t - (4.0/29.0) ) );
    }

    MPS::tripletD cieLAB_to_XYZ(const float& L, 
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

        MPS::tripletD XYZ;
        XYZ[0] = (Xn * inverse_f_of_t( (L + 16)/116.0 + A/500.0 ));
        XYZ[1] = (Yn * inverse_f_of_t( (L + 16.0)/116 ));
        XYZ[2] = (Zn * inverse_f_of_t( (L + 16.0)/116 - B/200.0 ));

        return(XYZ);
    }
}
