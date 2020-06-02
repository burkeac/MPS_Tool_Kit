// deltaE.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// June 1, 2020

namespace MPS{

    class CIEdeltaE {
        private:
            // private memebers
            double _L1, _a1, _b1, 
                   _L2, _a2, _b2;

        public:

        // constructors
        /// Default constructor
        CIEdeltaE();
        /// CIE DeltaE constructor
        CIEdeltaE(double L1, double a1, double b1, 
                  double L2, double a2, double b2);

        // Methods
        void setLab1(double L1, double a1, double b1);
        void setLab2(double L2, double a2, double b2);
        double cie76();
        double cie94(bool graphicArts = true); //note: kc and kh are = 1

    };

}


