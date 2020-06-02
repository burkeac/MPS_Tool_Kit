// detlaE.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// June 1, 2020

#include <cmath>
#include "deltaE.hpp"

// constructors
namespace MPS {
    // CIEDeltaE Class -- Constructors
    CIEdeltaE::CIEdeltaE(){
        _L1 = 0.; _a1 = 0.; _b1 = 0.;
        _L2 = 0.; _a2 = 0.; _b2 = 0.;

    }
    CIEdeltaE::CIEdeltaE(double L1, double a1, double b1, 
                         double L2, double a2, double b2){
        setLab1(L1, a1, b2);
        setLab2(L2, a2, b2);
    }

    // CIEDeltaE Class -- Methods

    void CIEdeltaE::setLab1(double L1, double a1, double b1) {
        _L1 = L1; _a1 = a1; _b1 = b1; 
    }
    void CIEdeltaE::setLab2(double L2, double a2, double b2) {
        _L2 = L2; _a2 = a2; _b2 = b2;
    }

    double CIEdeltaE::cie76(){
        double deltaE_ab = std::sqrt( 
            std::pow( (_L2 - _L1), 2) + 
            std::pow( (_a2 - _a1), 2) + 
            std::pow( (_b2 - _b1), 2) );
        
        return(deltaE_ab);
    }

    double CIEdeltaE::cie94(bool graphicArts){
        double deltaL = _L1 - _L2;
        double C1 = sqrt( std::pow(_a1, 2) + std::pow(_b1, 2) );
        double C2 = sqrt( std::pow(_a2, 2) + std::pow(_b2, 2) );
        double DeltaC_ab = C1 - C2;
        
        double Delta_a = _a1 - _a2;
        double Delta_b = _b1 - _b2;

        double DeltaH_ab = sqrt( std::pow(Delta_a, 2) +
                                 std::pow(Delta_b, 2) -
                                 std::pow(DeltaC_ab, 2) );

        double kL = graphicArts ? 1.0 : 2;
        double K1 = graphicArts ? 0.045 : 0.048;
        double K2 = graphicArts ? 0.015 : 0.014;

        double S_c = 1 + K1 * C1;
        double S_h = 1 + K2 * C1;

        double deltaE_94 = std::pow( ( deltaL / kL ), 2) + 
                           std::pow( ( DeltaC_ab / S_c ), 2) + 
                           std::pow( ( DeltaH_ab / S_h ), 2);

        return(deltaE_94);
    }

}