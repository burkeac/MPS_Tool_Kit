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
        CIEdeltaE(const double& L1, const double& a1, const double& b1, 
                  const double& L2, const double& a2, const double& b2);

        // Methods

        /// Returns the DeltaE 76 (1976) value
        /** 
         * Variables L a b correspond to CIE L*a*b* respectivly.\n
         * [return] double - DeltaE 76 \n
         * \n Note: this non-static function wraps its sister-static function,
         * but can be used for holding state when an object is constructed.
        */
        double cie76() const;

        /// static method returning DeltaE 76.
        /**
         * This method is provided for convinience / potential performance benefit.
         * */
        static double cie76(const double& L1, const double& a1, const double& b1, 
                            const double& L2, const double& a2, const double& b2);

        /// Returns the DeltaE 76 (1976) value as a double
        /** 
         * [in] bool - determins the values for graphic arts or textiles. \n
         * true: Graphic Arts. false: textiles. Default is true. \n
         * note: kc and kh are = 1 \n
         * */
        double cie94(const bool& graphicArts = true) const; //note: kc and kh are = 1

        /// static method returning DeltaE 94.
        /**
         * This method is provided for convinience / potential performance benefit.
         * */
        static double cie94(const double& L1, const double& a1, const double& b1,
                            const double& L2, const double& a2, const double& b2,
                            const bool& graphicArts = true);

        /// Returns the DeltaE 2000 value as a double
        /** 
         * [return] the DeltaE 2000 value as a double \n\n
         * 
         * Addapted from source code by Greg Fiumara (C) 2015 under the MIT license. 
         * for more info, see the implmentation in deltaE.cpp. 
         * Origional src: https://github.com/gfiumara/CIEDE2000
         * */
        double cie2000() const; 

        /// static method returning DeltaE 2000.
        /**
         * This method is provided for convinience / potential performance benefit.
         * */
        static double cie2000(const double& L1, const double& a1, const double& b1,
                              const double& L2, const double& a2, const double& b2);
    };

    class CIEdeltaE_frmXYZ : public CIEdeltaE{
        private: 
        
        public:
        /// calculates the delta E between 2 CIE XYZ values.
        /**
         * XYZ values are converted to CIELAB upon construction. The delta E value can 
         * then be calculated using the methods inherated from CIEdeltaE. \n
         * XYZ values should be normalized so that Y = 100. \n
         * NOTE: inherited static-members have been deleted as insantiat
         * */
        CIEdeltaE_frmXYZ(const double& X1, const double& Y1, const double& Z1,
                         const double& X2, const double& Y2, const double& Z2, 
                         const WhitePoint& wtpt);
        
        /// Static members can't be used, as class instaniation is reuqired for color transform.
        static double cie2000(const double& L1, const double& a1, const double& b1,
                              const double& L2, const double& a2, const double& b2) = delete;
        /// Static members can't be used, as class instaniation is reuqired for color transform.
        static double cie94(const double& L1, const double& a1, const double& b1,
                            const double& L2, const double& a2, const double& b2,
                            const bool& graphicArts = true) = delete;
        /// Static members can't be used, as class instaniation is reuqired for color transform.
        static double cie76(const double& L1, const double& a1, const double& b1, 
                            const double& L2, const double& a2, const double& b2) = delete;
        // redclair dynamic classes
        double cie2000();
        double cie94(const bool& graphicArts = true);
        double cie76();
    };

}


