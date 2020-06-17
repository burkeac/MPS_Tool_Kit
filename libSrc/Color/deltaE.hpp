// deltaE.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// June 1, 2020


#pragma once
#include "ColorSpaces.hpp"

namespace MPS{

    class CIEdeltaE {
        protected:
            // private memebers
            double _L1, _a1, _b1, 
                   _L2, _a2, _b2;

        public:

        // constructors
        /// Default DeltaE constructor
        CIEdeltaE();

        /// DeltaE constructor
        /** 
         * Accepts the CIE L*a*b* 1976
         * [in] double - L* value of color 1. Should be in the range of 0 & 100 \n
         * [in] double - a* value of color 1. Should be in the range of -128 & 128 \n
         * [in] double - b* value of color 1. Should be in the range of -128 & 128 \n
         * [in] double - L* value of color 2. Should be in the range of 0 & 100 \n
         * [in] double - a* value of color 2. Should be in the range of -128 & 128 \n
         * [in] double - b* value of color 2. Should be in the range of -128 & 128
         * */
        CIEdeltaE(double L1, double a1, double b1, 
                  double L2, double a2, double b2);

        // Methods

        /// Returns the DeltaE 76 (1976) value
        /** [return] double - DeltaE 76 */
        double cie76();

        /// Returns the DeltaE 76 (1976) value as a double
        /** 
         * [in] bool - determins the values for graphic arts or textiles. \n
         * true: Graphic Arts. false: textiles. Default is true.
         * */
        double cie94(bool graphicArts = true); //note: kc and kh are = 1

        /// Returns the DeltaE 2000 value as a double
        /** 
         * [return] the DeltaE 2000 value as a double \n\n
         * 
         * Addapted from source code by Greg Fiumara (C) 2015 under the MIT license. 
         * for more info, see the implmentation in deltaE.cpp. 
         * Origional src: https://github.com/gfiumara/CIEDE2000
         * */
        double cie2000(); 
    };

    class CIEdeltaE_frmXYZ : public CIEdeltaE{
        private: 
        void setLab1(double L1, double a1, double b1);
        void setLab2(double L2, double a2, double b2);
        
        public:
        /// calculates the delta E between 2 CIE XYZ values.
        /**
         * XYZ values are converted to CIELAB upon construction. The delta E value can 
         * then be calculated using the methods inherated from CIEdeltaE. \n
         * XYZ values should be normalized so that Y = 100.
         * */
        CIEdeltaE_frmXYZ(const double& X1, const double& Y1, const double& Z1,
                         const double& X2, const double& Y2, const double& Z2, 
                         const WhitePoint& wtpt);
    };

}


