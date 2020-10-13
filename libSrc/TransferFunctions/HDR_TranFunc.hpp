// HDR_Transforms.hpp
// Adam C. Burke (adam.burke603@gmail.com)
// July 27, 2019

#pragma once

namespace MPS{
    // Performs the SMPTE ST2084 transform from linear light to PQ (inverse EOTF)
    // Note this is a wrapper from the function provided in the aces-dev library
    // [in]     linear      floating point 0-10000
    // [return] PQ encoded  floating point 0-1
    float Linear_2_PQ(const float input);

    // Performs the SMPTE ST2084 transform from PQ to linear light (EOTF)
    // Note this is a wrapper from the function provided in the aces-dev library
    // [in]     PQ encoded  floating point 0-1
    // [return] linear      floating point 0-10000
    float PQ_2_Linear(const float input);

    // Apply the OETF of BT.2100 HLG to the input
    // [in]     linear light
    // [return] HLG encoded value 
    float Linear_2_HLG(const float input);

    // Apply the inverse OETF of BT.2100 HLG to the input
    // [in]     HLG encoded value 
    // [return] linear light
    float HLG_2_Linear(const float input);
}