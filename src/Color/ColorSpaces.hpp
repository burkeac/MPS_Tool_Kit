// ColorSpaces.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// July 28, 2019

#ifndef MPS_Tool_KIT_Color_ColorSpaces
#define MPS_Tool_KIT_Color_ColorSpaces

#include <vector>

using namespace std;

namespace MPS {
    enum ColorSpaces{Rec709, Rec2020};

    class ColorPrimaries {
        public:
            vector<float> Primaries;
            vector<float> WhitePoint;
            void SelectPrimary(int colorspace);
    };

}
#endif