// deltaE.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// June 1, 2020


#pragma once

namespace MPS{

    class CIEdeltaE {
        private:
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
        /** 
         * Sets the CIE L*a*b* (1976) of the first color 
         * [in] double - L* value of color 1. Should be in the range of 0 & 100 \n
         * [in] double - a* value of color 1. Should be in the range of -128 & 128 \n
         * [in] double - b* value of color 1. Should be in the range of -128 & 128
         * */
        void setLab1(double L1, double a1, double b1);

        /** 
         * Sets the CIE L*a*b* (1976) of the second color 
         * [in] double - L* value of color 2. Should be in the range of 0 & 100 \n
         * [in] double - a* value of color 2. Should be in the range of -128 & 128 \n
         * [in] double - b* value of color 2. Should be in the range of -128 & 128
         * */
        void setLab2(double L2, double a2, double b2);

        /// Returns the DeltaE 76 (1976) value
        /** [return] double - DeltaE 76 */
        double cie76();

        /// Returns the DeltaE 76 (1976) value as a double
        /** 
         * [in] bool - determins the values for graphic arts or textiles. \n
         * true: Graphic Arts. false: textiles. Default is true.
         * */
        double cie94(bool graphicArts = true); //note: kc and kh are = 1

        double cie2000(); 

    };

}


