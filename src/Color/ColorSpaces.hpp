// ColorSpaces.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// July 28, 2019

#ifndef MPS_Tool_KIT_Color_ColorSpaces
#define MPS_Tool_KIT_Color_ColorSpaces

#include <vector>
#include <opencv2/core.hpp>
#include "../mpsVersion.hpp"

// using namespace std;

namespace MPS {
    enum ColorSpaces{Rec709, Rec2020, P3D65, P3DCI, ACES_P0, ACES_P1};
    enum ChromaPoint{xRed, yRed, zRed, xGreen, yGreen, zGreen, xBlue, yBlue, zBlue, xWhite, yWhite, zWhite};

    class colorPrimaries {
        public:
            colorPrimaries();
            colorPrimaries(int colorSpace);
            std::vector<float> Primaries;
            std::vector<float> WhitePoint;
            void selectPrimary(int colorspace);
            void printPrimaries();
            float getChromaPoint(int chromaPoint);
    };

    class phosphorMatrix {
        public:
        // Constructors
            phosphorMatrix(MPS::colorPrimaries primaries);
            phosphorMatrix(MPS::colorPrimaries primarySet1, MPS::colorPrimaries primarySet2);

        // Member Functions
            // Generates the PM and InvPM and saves them to the private members.
            // Returns the PM which can then have cv::Mat.inv() member used on it.
            cv::Mat generatePMs(MPS::colorPrimaries primaries, 
                                float actualLum = 100.0, 
                                float aimLum = 100.0, 
                                bool setMembers = true);
            cv::Mat getPM();
            cv::Mat getInvPM();
        private:
            cv::Mat _PM;
            cv::Mat _invPM;
    };

    std::vector<float> Rec2020_to_ICtCp(float R, float G, float B);
}
#endif