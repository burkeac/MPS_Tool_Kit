// HDR_Transforms.hpp
// Adam C. Burke (adam.burke603@gmail.com)
// July 27, 2019

#include <cmath>
#include <iostream>
#include "HDR_TranFunc.hpp"

using namespace std;

// Constants from SMPTE ST 2084-2014
const float pq_m1 = 0.1593017578125; // ( 2610.0 / 4096.0 ) / 4.0;
const float pq_m2 = 78.84375; // ( 2523.0 / 4096.0 ) * 128.0;
const float pq_c1 = 0.8359375; // 3424.0 / 4096.0 or pq_c3 - pq_c2 + 1.0;
const float pq_c2 = 18.8515625; // ( 2413.0 / 4096.0 ) * 32.0;
const float pq_c3 = 18.6875; // ( 2392.0 / 4096.0 ) * 32.0;

const float pq_C = 10000.0;

// --> Addapted from CTL code from aces-dev library 
// --> (aces-dev/transforms/ctl/lib/ACESlib.Utilities_Color.ctl):
    // Converts from linear cd/m^2 to the non-linear perceptually quantized space
    // Note that this is in float, and assumes normalization from 0 - 1
    // (0 - pq_C for linear) and does not handle the integer coding in the Annex 
    // sections of SMPTE ST 2084-2014
    float Y_2_ST2084( float C ){
    // Note that this does NOT handle any of the signal range
    // considerations from 2084 - this returns full range (0 - 1)
    float L = C / pq_C;
    float Lm = pow( L, pq_m1 );
    float N = ( pq_c1 + pq_c2 * Lm ) / ( 1.0 + pq_c3 * Lm );
    N = pow( N, pq_m2 );
    return N;
    }

// --> Addapted from CTL code from aces-dev library 
// --> (aces-dev/transforms/ctl/lib/ACESlib.Utilities_Color.ctl):
    // Converts from the non-linear perceptually quantized space to linear cd/m^2
    // Note that this is in float, and assumes normalization from 0 - 1
    // (0 - pq_C for linear) and does not handle the integer coding in the Annex 
    // sections of SMPTE ST 2084-2014
    float ST2084_2_Y( float N )
    {
    // Note that this does NOT handle any of the signal range
    // considerations from 2084 - this assumes full range (0 - 1)
    float Np = pow( N, 1.0 / pq_m2 );
    float L = Np - pq_c1;
    if ( L < 0.0 )
        L = 0.0;
    L = L / ( pq_c2 - pq_c3 * Np );
    L = pow( L, 1.0 / pq_m1 );
    return L * pq_C; // returns cd/m^2
    }

// HLG constants
const float a = 0.17883277;
const float b = 1-4*a;
const float c = 0.5 - a * log(4*a);

namespace MPS{
    // Performs the SMPTE ST2084 transform from linear light to PQ (inverse EETF)
    // Note this is a wrapper from the function provided in the aces-dev library
    // [in]     linear      floating point 0-1
    // [return] PQ encoded  floating point 0-1
    float Linear_2_PQ(const float input){
        return(Y_2_ST2084(input));
    }

    // Performs the SMPTE ST2084 transform from PQ to linear light (EETF)
    // Note this is a wrapper from the function provided in the aces-dev library
    // [in]     PQ encoded  floating point 0-1
    // [return] linear      floating point 0-1
    float PQ_2_Linear(const float input){
        return(ST2084_2_Y(input));
    }

    // Apply the OETF of BT.2100 HLG to the input
    // [in]     scene linear light
    // [return] HLG encoded value 
    float Linear_2_HLG(const float input){
        if(input <= (1/12)){
            return sqrt(3*input);
        }else{
            return a*log(12*input - b) + c;
        }
    }

    // Apply the inverse OETF of BT.2100 HLG to the input
    // [in]     HLG encoded value 
    // [return] Scene linear light
    float HLG_2_Linear(const float input){
        if(input <= 1/2){
            return pow(input,2)/3;
        }else{
            return (exp((input-c)/a)+b)/12;
        }
    }
}