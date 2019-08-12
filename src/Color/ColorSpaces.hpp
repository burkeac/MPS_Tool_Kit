// ColorSpaces.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// July 28, 2019

#ifndef MPS_Tool_KIT_Color_ColorSpaces
#define MPS_Tool_KIT_Color_ColorSpaces

#include <vector>

using namespace std;

namespace MPS {
    enum ColorSpaces{Rec709, Rec2020, P3D65, P3DCI, ACES_P0, ACES_P1};

    class colorPrimaries {
        public:
            colorPrimaries();
            colorPrimaries(int colorSpace);
            vector<float> Primaries;
            vector<float> WhitePoint;
            void selectPrimary(int colorspace);
            void printPrimaries();
            
    };

    class phosphorMatrix {
        public:
            phosphorMatrix(MPS::colorPrimaries primaries);
            phosphorMatrix(MPS::colorPrimaries primarySet1, MPS::colorPrimaries primarySet2);
            void generatePM(MPS::colorPrimaries primaries);
            
            
    };

}
#endif