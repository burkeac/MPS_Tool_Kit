// detlaE.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// June 1, 2020

#include <cmath>
#include <iostream>
#include "deltaE.hpp"
#include <vector>

// if M_PI is not defined on system
#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

// constructors
namespace MPS {
    // CIEDeltaE Class -- Constructors
    CIEdeltaE::CIEdeltaE()
          : _L1(0), _a1(0), _b1(0),
            _L2(0), _a2(0), _b2(0)
        {}

    CIEdeltaE::CIEdeltaE(double L1, double a1, double b1, 
                         double L2, double a2, double b2)
                         : _L1(L1), _a1(a1), _b1(b1),
                           _L2(L2), _a2(a2), _b2(b2)
                         {}

    // CIEDeltaE Class -- Methods
    void CIEdeltaE::setLab1(double L1, double a1, double b1) {
        _L1 = L1; _a1 = a1; _b1 = b1; 
    }

    void CIEdeltaE::setLab2(double L2, double a2, double b2) {
        _L2 = L2; _a2 = a2; _b2 = b2;
    }

    double CIEdeltaE::cie76(){
        double deltaE_ab = std::sqrt( 
                              std::pow( (_L2 - _L1), 2.0) 
                            + std::pow( (_a2 - _a1), 2.0) 
                            + std::pow( (_b2 - _b1), 2.0) );
        return(deltaE_ab);
    }

    double CIEdeltaE::cie94(bool graphicArts){


        double deltaL = _L1 - _L2;
        double C1 = sqrt( std::pow(_a1, 2.0) + std::pow(_b1, 2.0) );
        double C2 = sqrt( std::pow(_a2, 2.0) + std::pow(_b2, 2.0) );
        double DeltaC_ab = C1 - C2;
        
        double Delta_a = _a1 - _a2;
        double Delta_b = _b1 - _b2;

        double DeltaH_ab = sqrt( std::pow(Delta_a, 2.0) +
                                 std::pow(Delta_b, 2.0) -
                                 std::pow(DeltaC_ab, 2.0) );

        double kL = graphicArts ? 1.0 : 2;
        double K1 = graphicArts ? 0.045 : 0.048;
        double K2 = graphicArts ? 0.015 : 0.014;

        double S_c = 1.0 + K1 * C1;
        double S_h = 1.0 + K2 * C1;

        double deltaE_94 = std::sqrt(
                            std::pow( ( deltaL / kL ), 2.0) + 
                            std::pow( ( DeltaC_ab / S_c ), 2.0) + 
                            std::pow( ( DeltaH_ab / S_h ), 2.0)
                           );

        return(deltaE_94);
    }

    constexpr double deg2Rad(const double deg)
        {
            return (deg * (M_PI / 180.0));
        }

    constexpr double rad2Deg(const double rad)
        {
            return ((180.0 / M_PI) * rad);
        }

    double CIEdeltaE::cie2000(){

        /*
        This memebrs implementation has been addapted from the origional 
        program by Greg Fiumara (C) 2015 under the MIT license.
        Origonal Repository: https://github.com/gfiumara/CIEDE2000
        
        -Lincense-
        The MIT License (MIT)

        Copyright (c) 2015 Greg Fiumara 

        Permission is hereby granted, free of charge, to any person obtaining a copy
        of this software and associated documentation files (the "Software"), to deal
        in the Software without restriction, including without limitation the rights
        to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
        copies of the Software, and to permit persons to whom the Software is
        furnished to do so, subject to the following conditions:

        The above copyright notice and this permission notice shall be included in all
        copies or substantial portions of the Software.

        THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
        IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
        FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
        AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
        LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
        OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
        SOFTWARE.
        */
        /* 
        * "For these and all other numerical/graphical 􏰀delta E00 values
        * reported in this article, we set the parametric weighting factors
        * to unity(i.e., k_L = k_C = k_H = 1.0)." (Page 27).
        */
        const double k_L = 1.0, k_C = 1.0, k_H = 1.0;
        const double deg360InRad = deg2Rad(360.0);
        const double deg180InRad = deg2Rad(180.0);
        const double pow25To7 = 6103515625.0; /* pow(25, 7) */
        
        /*
        * Step 1 
        */
        /* Equation 2 */
        double C1 = sqrt((_a1 * _a2) + (_b1 * _b1));
        double C2 = sqrt((_a2 * _a2) + (_b2 * _b2));
        /* Equation 3 */
        double barC = (C1 + C2) / 2.0;
        /* Equation 4 */
        double G = 0.5 * (1 - sqrt(pow(barC, 7) / (pow(barC, 7) + pow25To7)));
        /* Equation 5 */
        double a1Prime = (1.0 + G) * _a1;
        double a2Prime = (1.0 + G) * _a2;
        /* Equation 6 */
        double CPrime1 = sqrt((a1Prime * a1Prime) + (_b1 * _b1));
        double CPrime2 = sqrt((a2Prime * a2Prime) + (_b2 * _b2));
        /* Equation 7 */
        double hPrime1;
        if (_b1 == 0 && a1Prime == 0)
            hPrime1 = 0.0;
        else {
            hPrime1 = atan2(_b1, a1Prime);
            /* 
            * This must be converted to a hue angle in degrees between 0 
            * and 360 by addition of 2􏰏 to negative hue angles.
            */
            if (hPrime1 < 0)
                hPrime1 += deg360InRad;
        }
        double hPrime2;
        if (_b2 == 0 && a2Prime == 0)
            hPrime2 = 0.0;
        else {
            hPrime2 = atan2(_b2, a2Prime);
            /* 
            * This must be converted to a hue angle in degrees between 0 
            * and 360 by addition of 2􏰏 to negative hue angles.
            */
            if (hPrime2 < 0)
                hPrime2 += deg360InRad;
        }
        
        /*
        * Step 2
        */
        /* Equation 8 */
        double deltaLPrime = _L2 - _L1;
        /* Equation 9 */
        double deltaCPrime = CPrime2 - CPrime1;
        /* Equation 10 */
        double deltahPrime;
        double CPrimeProduct = CPrime1 * CPrime2;
        if (CPrimeProduct == 0)
            deltahPrime = 0;
        else {
            /* Avoid the fabs() call */
            deltahPrime = hPrime2 - hPrime1;
            if (deltahPrime < -deg180InRad)
                deltahPrime += deg360InRad;
            else if (deltahPrime > deg180InRad)
                deltahPrime -= deg360InRad;
        }
        /* Equation 11 */
        double deltaHPrime = 2.0 * sqrt(CPrimeProduct) *
            sin(deltahPrime / 2.0);
        
        /*
        * Step 3
        */
        /* Equation 12 */
        double barLPrime = (_L1 + _L2) / 2.0;
        /* Equation 13 */
        double barCPrime = (CPrime1 + CPrime2) / 2.0;
        /* Equation 14 */
        double barhPrime, hPrimeSum = hPrime1 + hPrime2;
        if (CPrime1 * CPrime2 == 0) {
            barhPrime = hPrimeSum;
        } else {
            if (fabs(hPrime1 - hPrime2) <= deg180InRad)
                barhPrime = hPrimeSum / 2.0;
            else {
                if (hPrimeSum < deg360InRad)
                    barhPrime = (hPrimeSum + deg360InRad) / 2.0;
                else
                    barhPrime = (hPrimeSum - deg360InRad) / 2.0;
            }
        }
        /* Equation 15 */
        double T = 1.0 - (0.17 * cos(barhPrime - deg2Rad(30.0))) +
            (0.24 * cos(2.0 * barhPrime)) +
            (0.32 * cos((3.0 * barhPrime) + deg2Rad(6.0))) - 
            (0.20 * cos((4.0 * barhPrime) - deg2Rad(63.0)));
        /* Equation 16 */
        double deltaTheta = deg2Rad(30.0) *
            exp(-pow((barhPrime - deg2Rad(275.0)) / deg2Rad(25.0), 2.0));
        /* Equation 17 */
        double R_C = 2.0 * sqrt(pow(barCPrime, 7.0) /
            (pow(barCPrime, 7.0) + pow25To7));
        /* Equation 18 */
        double S_L = 1 + ((0.015 * pow(barLPrime - 50.0, 2.0)) /
            sqrt(20 + pow(barLPrime - 50.0, 2.0)));
        /* Equation 19 */
        double S_C = 1 + (0.045 * barCPrime);
        /* Equation 20 */
        double S_H = 1 + (0.015 * barCPrime * T);
        /* Equation 21 */
        double R_T = (-sin(2.0 * deltaTheta)) * R_C;
        
        /* Equation 22 */
        double deltaE = sqrt(
            pow(deltaLPrime / (k_L * S_L), 2.0) +
            pow(deltaCPrime / (k_C * S_C), 2.0) +
            pow(deltaHPrime / (k_H * S_H), 2.0) + 
            (R_T * (deltaCPrime / (k_C * S_C)) * (deltaHPrime / (k_H * S_H))));
        
        return (deltaE);
    }

    // DeltaE from XYZ constructor
    CIEdeltaE_frmXYZ::CIEdeltaE_frmXYZ(const double& X1, const double& Y1, const double& Z1,
                                       const double& X2, const double& Y2, const double& Z2, 
                                       const WhitePoint& wtpt){
 
        std::vector<double> LAB1 = XYZ_to_cieLAB(X1, Y1, Z1, wtpt);
        std::vector<double> LAB2 = XYZ_to_cieLAB(X2, Y2, Z2, wtpt);

        _L1 = LAB1[0]; _a1 = LAB1[1]; _b1 = LAB1[2];

        _L2 = LAB2[0]; _a2 = LAB2[1]; _b2 = LAB2[2];
    };
}