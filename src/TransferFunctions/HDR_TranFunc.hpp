// HDR_Transforms.hpp
// Adam C. Burke (adam.burke603@gmail.com)
// July 27, 2019

#ifndef MPS_Tool_KIT_TranFunc_HDR_Transforms
#define MPS_Tool_KIT_TranFunc_HDR_Transforms

namespace MPS{
    // Performs the SMPTE ST2084 transform from linear light to PQ (inverse EETF)
    // Note this is a wrapper from the function provided in the aces-dev library
    // [in]     linear      floating point 0-1
    // [return] PQ encoded  floating point 0-1
    float Linear_2_PQ(const float input);

    // Performs the SMPTE ST2084 transform from PQ to linear light (EETF)
    // Note this is a wrapper from the function provided in the aces-dev library
    // [in]     PQ encoded  floating point 0-1
    // [return] linear      floating point 0-1
    float PQ_2_Linear(const float input);
}

#endif