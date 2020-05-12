// ColorSpaces.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// July 28, 2019

#pragma once

#include <vector>
//#include <opencv2/core.hpp> <-- put into function
#include "../Eigen/Core"
#include "../mpsVersion.hpp"

namespace MPS {
    enum ColorSpaces{Rec709, Rec2020, P3D65, P3DCI, ACES_P0, ACES_P1};
    enum ChromaPoint{xRed, yRed, zRed, xGreen, yGreen, zGreen, xBlue, yBlue, zBlue, xWhite, yWhite, zWhite};

    class colorPrimaries {
        public:
            // Constructors 
            colorPrimaries();
            colorPrimaries(ColorSpaces colorSpace);

            // Members 
            std::vector<float> Primaries;
            std::vector<float> WhitePoint;

            // Methods
            void selectPrimary(ColorSpaces colorspace);
            void printPrimaries();
            float getChromaPoint(ChromaPoint chromaPoint);
    };

    class phosphorMatrix {
        public:
        // Constructors
            phosphorMatrix();
            phosphorMatrix(MPS::colorPrimaries primaries);
            phosphorMatrix(MPS::colorPrimaries primarySet1, MPS::colorPrimaries primarySet2);

        // Member Functions
            // Generates the PM and InvPM and saves them to the private members.
            // Returns the PM which can then have cv::Mat.inv() member used on it.
            Eigen::Matrix3f generatePMs(MPS::colorPrimaries primaries, 
                                float actualLum = 100.0, 
                                float aimLum = 100.0, 
                                bool setMembers = true);
            Eigen::Matrix3f getPM();
            Eigen::Matrix3f getInvPM();
        private:
            Eigen::Matrix3f _PM;
            Eigen::Matrix3f _invPM;
    };

    std::vector<float> Rec2020_to_ICtCp(float R, float G, float B);
}