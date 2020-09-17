// detlaE.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// June 1, 2020

#include <cmath>
#include <iostream>
#include "deltaE.hpp"

#ifndef _WIN32
    #include "../../ThirdParty/CIEDE2000/CIEDE2000.h"
#else
    #include "..\..\ThirdParty\CIEDE2000\CIEDE2000.h"
#endif

// if M_PI is not defined on system
#ifndef M_PI
    #define M_PI 3.14159265358979323846264338327950288
#endif

// constructors
namespace MPS {
    // CIEDeltaE Class -- Constructors

    CIEdeltaE::CIEdeltaE()
        :_L1(0), _a1(0), _b1(0),
         _L2(0), _a2(0), _b2(0)
        {}

    CIEdeltaE::CIEdeltaE(const double& L1, const double& a1, const double& b1, 
                         const double& L2, const double& a2, const double& b2)
                         : _L1(L1), _a1(a1), _b1(b1),
                           _L2(L2), _a2(a2), _b2(b2)
                         {}

    // CIEDeltaE Class -- Methods

    double CIEdeltaE::cie76() const{
        // call static method using assigned params
        return CIEdeltaE::cie76(_L1, _a1, _b1, _L2, _a2, _b2);
    }

    double CIEdeltaE::cie76(const double& L1, const double& a1, const double& b1, 
                            const double& L2, const double& a2, const double& b2){
        double deltaE_ab = std::sqrt( 
                              std::pow( (L2 - L1), 2.0) 
                            + std::pow( (a2 - a1), 2.0) 
                            + std::pow( (b2 - b1), 2.0) );
        return(deltaE_ab);
    }

    double CIEdeltaE::cie94(const bool& graphicArts) const{
        return CIEdeltaE::cie94(_L1, _a1, _b1, _L2, _a2, _b2, graphicArts);
    }

    double CIEdeltaE::cie94(const double& L1, const double& a1, const double& b1,
                            const double& L2, const double& a2, const double& b2,
                            const bool& graphicArts){

        double deltaL = L1 - L2;
        double C1 = sqrt( std::pow(a1, 2.0) + std::pow(b1, 2.0) );
        double C2 = sqrt( std::pow(a2, 2.0) + std::pow(b2, 2.0) );
        double DeltaC_ab = C1 - C2;
        
        double Delta_a = a1 - a2;
        double Delta_b = b1 - b2;

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


    double CIEdeltaE::cie2000() const{

        /*
        This memebr's implementation utilizes wraps thesource code from the 
        "CIEDE2000" program by Greg Fiumara (C) 2015 under the MIT license.
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

        CIEDE2000::LAB lab1 = {_L1, _a1, _b1};
        CIEDE2000::LAB lab2 = {_L2, _a2, _b2};
        return CIEDE2000::CIEDE2000(lab1, lab2);
    }

    double CIEdeltaE::cie2000(const double& L1, const double& a1, const double& b1,
                              const double& L2, const double& a2, const double& b2){
        CIEDE2000::LAB lab1 = {L1, a1, b1};
        CIEDE2000::LAB lab2 = {L2, a2, b2};
        return CIEDE2000::CIEDE2000(lab1, lab2);
    }

    // DeltaE from XYZ constructor
    CIEdeltaE_frmXYZ::CIEdeltaE_frmXYZ(const double& X1, const double& Y1, const double& Z1,
                                       const double& X2, const double& Y2, const double& Z2, 
                                       const WhitePoint& wtpt){
 
        MPS::tripletD LAB1 = XYZ_to_cieLAB(X1, Y1, Z1, wtpt);
        MPS::tripletD LAB2 = XYZ_to_cieLAB(X2, Y2, Z2, wtpt);

        _L1 = LAB1[0]; _a1 = LAB1[1]; _b1 = LAB1[2];

        _L2 = LAB2[0]; _a2 = LAB2[1]; _b2 = LAB2[2];
    };

    double CIEdeltaE_frmXYZ::cie2000(){
        return CIEdeltaE::cie2000(_L1, _a1, _b1, _L2, _a2, _b2);
    }
    
    double CIEdeltaE_frmXYZ::cie94(const bool& graphicArts){
        return CIEdeltaE::cie94(_L1, _a1, _b1, _L2, _a2, _b2, graphicArts);
    }
    
    double CIEdeltaE_frmXYZ::cie76(){
        return CIEdeltaE::cie76(_L1, _a1, _b1, _L2, _a2, _b2);
    }

}