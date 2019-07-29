// ColorSpaces.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// July 28, 2019

#include <iostream>
#include <vector>
#include "ColorSpaces.hpp"

using namespace std;

const vector<float> Rec709_Primaries =  {0.64, 0.33, 0.30, 0.60, 0.15, 0.06};
const vector<float> Rec709_WhitePoint = {0.3127, 0.3290};
const vector<float> Rec2020_Primaries = {0.708, 0.292, 0.17, 0.797, 0.131, 0.046};
const vector<float> Rec2020_WhitePoint = {0.3127, 0.3290};

namespace MPS{
    
    void MPS::ColorPrimaries::SelectPrimary(int colorspace){
        switch (colorspace){
            case MPS::Rec709: 
                ColorPrimaries::Primaries = Rec709_Primaries; 
                ColorPrimaries::WhitePoint = Rec709_WhitePoint;
                break;
            case MPS::Rec2020: 
                ColorPrimaries::Primaries = Rec2020_Primaries;
                ColorPrimaries::WhitePoint = Rec2020_WhitePoint;
                break;
            default: ColorPrimaries::Primaries = vector<float> {0,0,0,0,0,0}; break;
        }
    }

}
