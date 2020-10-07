// ColorSpaces.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// July 28, 2019

#pragma once

// #include <vector>
#include "macro_typedef.hpp"

//#include <opencv2/core.hpp> <-- put into function
#ifndef _WIN32
    #include "../../ThirdParty/Eigen/Core"
#else
    #include "..\..\ThirdParty\Eigen\Core"
#endif

namespace MPS {

    /// Pre-defined color spaces that can be used within the colorPrimaries class
    enum ColorSpaces{Rec709, Rec2020, P3D65, P3DCI, ACES_P0, ACES_P1};
    
    /// Enums to be used in calling the corresponding primary's x,y or z value.
    enum ChromaPoint{xRed, yRed, zRed, xGreen, yGreen, zGreen, xBlue, yBlue, zBlue, xWhite, yWhite, zWhite};
    
    enum WhitePoint{D65, D50};

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
            std::array<float, 6> Primaries; ///< Stores the RGB primaires within the object as a std::array<float, 6>
                                          /**< Primaries are store in Red x, Red y, Green x, Green y, Blue x, Blue y*/

            std::array<float, 2> WhitePoint; ///< Stores the white point within the object as a std::array<float, 2>

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
        private:
            // internal method used for calculating the forward PM.
            static Eigen::Matrix3f _generatePM(MPS::colorPrimaries& primaries, 
                                               const float& actualLum = 100.0, 
                                               const float& aimLum = 100.0);
        
        public:
        // Constructors
            /// Default constructor initiates PM and PM^1 matrices with idenity matricies.
            phosphorMatrix();
            
            /// PM Constructor for single primary input. 
            /**Used for going from a primary set to XYZ and vice-versa. \n
             * [In] colorPrimaries object containing specified primaries. Passed by Reference.
            */
            phosphorMatrix(MPS::colorPrimaries& primaries);
            
            /// Overloaded constructor for convenience. Constructs internal primary set.
            phosphorMatrix(const MPS::ColorSpaces& colorSpace);

            /// PM Constructor for double primary input.
            /**  Used for translating from one primary set to a second primary set and vice-versa \n
             * [In] colorPrimaries set 1 \n
             * [In] colorPrimaries set 2
            */
            phosphorMatrix(MPS::colorPrimaries& primarySet1, MPS::colorPrimaries& primarySet2);
            
            /// Overloaded constructor for convenience. Constructs internal primary sets.
            phosphorMatrix(const MPS::ColorSpaces& colorSpace1, const MPS::ColorSpaces& colorSpace2);

        // Member Functions
            /// Generates the PM and Inverse PM from the set of primaries
            /** Calculates the forward and inverse PMs of the given primary sets \n
             * @param [in] primaries  the primaries the phosphore matrix should be calcualted for. Passed by reference \n
             * @param [in] actualLUm  The acutal luminance the PM should be calculated for. Default = 100 nits \n
             * @param [in] aimLum     The aim luminance the PM should be calculated for. Default = 100 nits \n
             * @param [in] setMembers Set the private PM members of the object. Default = True. \n
             * @return Forward PM as an Eigen::Matrix3f.
             */
            Eigen::Matrix3f& generatePM(MPS::colorPrimaries& primaries, 
                                        const float& actualLum = 100.0, 
                                        const float& aimLum = 100.0);


                    
            /// Returns the phosphore matrix stored in the private member as an Eigen::Matrix3f.
            const Eigen::Matrix3f& getPM() const;

            /// Returns the inverse phosphore matrix stored in the private member as an Eigen::Matrix3f.
            const Eigen::Matrix3f& getInvPM() const;

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
     * [Return] MPS::tripletF in I, Ct, Cp format \n
     * 
     *  Implementation based on Dolby White Paper "Perceptual Color Volume" and WHAT IS ICTCP?
     *  https://www.dolby.com/us/en/technologies/dolby-vision/measuring-perceptual-color-volume-v7.1.pdf
     *  https://www.dolby.com/us/en/technologies/dolby-vision/ictcp-white-paper.pdf
     * */
    MPS::tripletF Rec2020_to_ICtCp(const float& R, const float& G, const float& B, 
                                   const bool& PQ = true, const bool& scaleToJNDs = false);


    /// Converts CIE XYZ values to CIELAB.
    /** 
     * XYZ values should be normalized so that Y = 100.\n
     * WhitePoint options include D65 and D50 
     * */
    MPS::tripletD XYZ_to_cieLAB(const float& X, 
                                const float& Y, 
                                const float& Z, 
                                const WhitePoint& whitePt = D65);

    /// Converts CIE XYZ values to CIELAB.
    /** 
     * LAB values are returned normalized so that Y = 100. \n 
     * WhitePoint options include D65 and D50 
     * */
    MPS::tripletD cieLAB_to_XYZ(const float& L, 
                                const float& A, 
                                const float& B, 
                                const WhitePoint& whitePt = D65);
        
}