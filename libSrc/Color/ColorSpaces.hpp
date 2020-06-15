// ColorSpaces.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// July 28, 2019

#pragma once

#include <vector>
//#include <opencv2/core.hpp> <-- put into function
#ifndef _WIN32
    #include "../Eigen/Core"
    #include "../mpsVersion.hpp"
#else
    #include "..\Eigen\Core"
    #include "..\mpsVersion.hpp"
#endif

namespace MPS {

    /// Pre-defined color spaces that can be used within the colorPrimaries class
    enum ColorSpaces{Rec709, Rec2020, P3D65, P3DCI, ACES_P0, ACES_P1};
    
    /// Enums to be used in calling the corresponding primary's x,y or z value.
    enum ChromaPoint{xRed, yRed, zRed, xGreen, yGreen, zGreen, xBlue, yBlue, zBlue, xWhite, yWhite, zWhite};
    
    /// Defines the primaries of a pre-defined or custom colorspace i.e. Rec709 or Rec2020
    /**
     * Provides easy access and storage to predefined and custom primary sets
     * Methods and members allow for easy modification of selected primarie sets.
     * Data stored in this class should be in CIE xy as outlined in the CIE 1931 Color Space
     * */
    class colorPrimaries {
        public:
        // Constructors 
            /// Default constructor
            colorPrimaries();
            
            /// Construct the object using a predefined colorspace
            /** [In]    ColorSpace enum*/ 
            colorPrimaries(ColorSpaces colorSpace);

        // Members 
            std::vector<float> Primaries; ///< Stores the RGB primaires within the object as a std::vector
                                          /**< Primaries are store in Red x, Red y, Green x, Green y, Blue x, Blue y*/

            std::vector<float> WhitePoint; ///< Stores the white point within the object as a std::vector

        // Methods
            /// Construct the object using a predefined colorspace
            /** [In]    ColorSpace enum*/
            void selectPrimary(ColorSpaces colorspace);
            
            void printPrimaries(); ///< Prints the primaries to std out

            /// Construct the object using a predefined colorspace
            /** [In]    ChromaPoint enum \n
             * [Return] Returns the specified color corrdinate
            */
            float getChromaPoint(ChromaPoint chromaPoint); 
    };

    class phosphorMatrix {
        public:
        // Constructors
            /// Default constructor initiates PM and PM^1 matrices with idenity matricies.
            phosphorMatrix();
            
            // PM Constructor for single primary input. 
            /**Used for going from a primary set to XYZ and vice-versa. \n
             * [In] colorPrimaries object containing specified primaries. Passed by Reference.
            */
            phosphorMatrix(MPS::colorPrimaries& primaries);

            /// PM Constructor for double primary input.
            /**  Used for translating from one primary set to a second primary set and vice-versa \n
             * [In] colorPrimaries set 1 \n
             * [In] colorPrimaries set 2
            */
            phosphorMatrix(MPS::colorPrimaries& primarySet1, MPS::colorPrimaries& primarySet2);

        // Member Functions
            /// Generates the PM and Inverse PM from the set of primaries
            /** Calculates the forward and inverse PMs of the given primary sets \n
             * [In] colorPrimaries the primaries the phosphore matrix should be calcualted for. 
             *      Passed by reference \n
             * [In] The acutal luminance the PM should be calculated for. Default = 100 nits \n
             * [In] The aim luminance the PM should be calculated for. Default = 100 nits \n
             * [In] Set the private PM members of the object. Default = True. \n
             * [Return] Forward PM as an Eigen::Matrix3f.
             */
            Eigen::Matrix3f generatePMs(MPS::colorPrimaries& primaries, 
                                float actualLum = 100.0, 
                                float aimLum = 100.0, 
                                bool setMembers = true);
            
            /// Returns the phosphore matrix stored in the private member as an Eigen::Matrix3f.
            Eigen::Matrix3f getPM();

            /// Returns the inverse phosphore matrix stored in the private member as an Eigen::Matrix3f.
            Eigen::Matrix3f getInvPM();

        private:
            Eigen::Matrix3f _PM; ///< Private meember function that stores the PM
            Eigen::Matrix3f _invPM; ///< Private meember function that stores the inverse PM
    };

    // Converts code values between Rec2020 and the ICtCp Color Space
    /**
     * Input code values should be linear Rec2020 values ie. PQ_RGB = 0-10,000 \n
     * [In] Rec2020 R CV \n
     * [In] Rec2020 G CV \n
     * [In] Rec2020 B CV \n
     * [In] PQ = true, HLG = false \n
     * [In] Scale the values to JND \n 
     * [Return] std::vector<float> in I, Ct, Cp format \n
     * 
     *  Implementation based on Dolby White Paper "Perceptual Color Volume" and WHAT IS ICTCP?
     *  https://www.dolby.com/us/en/technologies/dolby-vision/measuring-perceptual-color-volume-v7.1.pdf
     *  https://www.dolby.com/us/en/technologies/dolby-vision/ictcp-white-paper.pdf
     * */
    std::vector<float> Rec2020_to_ICtCp(float R, float G, float B, 
                                        bool PQ = true, bool scaleToJNDs = false);
}