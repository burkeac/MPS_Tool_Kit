// LUT.hpp
// Adam C. Burke (adam.burke603@gmail.com)
// October 13, 2020

#pragma once
#include <vector>
#include <functional>
#include "macro_typedef.hpp"

#ifndef _WIN32
    #include "../../ThirdParty/Eigen/Core"
    #include "../../ThirdParty/Eigen/Eigen"
#else
    #include "..\..\ThirdParty\Eigen\Core"
    #include "..\..\ThirdParty\Eigen\Eigen"
#endif

namespace MPS {

class LUT3D {
    protected:
    /// Internal Data structure for the LUT3D class.
    /*  
    Data should be stored in RGB index order. Access to the "output" code values
    should be accessed as such: LUT3D_Data[in_R][in_G][in_B]. This will return a 
    3 element array in the out_R out_G out_B index order.
    */
    typedef std::vector<std::vector<std::vector<tripletF>>> LUT3D_Data;

    // data is stored in a 3D array. data[R][G][B] returns the new codevalues of RGB 
    LUT3D_Data _data;

    // Used for interpolation normalization
    std::vector<float> _NodeValueLUT;

    /// Allocates the interal data structure
    void _Allocate3D_Data();
    inline float _getC(const tripletB node, const tripletF CV, const int chan);
    inline float _getScaleFactor(const float cord);
    uint8_t numNodes;

    public:

    struct CUBE_Params{
        // CUBE_Params(){}
        std::string TITLE = "";
        tripletF DOMAIN_MIN = {0.0f,0.0f,0.0f};
        tripletF DOMAIN_MAX = {1.0f,1.0f,1.0f};
    };

    LUT3D();

    /**
     * \note It is recomended that the number of nodes follows the sequence: 2^x+1. 
     * Example: nodes = {9, 17, 33, 65}. This is to ensure uniform spacing between code values.
     * */
    LUT3D(const uint8_t numNodes);

    /// Generates the inital code values
    /** 
     * Without manipulation, this would generate a "null" or "identity" LUT 
     * where input = output 
     */
    void Generate_3DLUT_Nodes();

    /// Returns the number of nodes the LUT contains
    int GetNumNodes();

    /// Writes the LUT to a CSV file.
    /**
     * Writes the data to a CSV file in the RGB format, where the blue channel
     * changes most frequently. 
     * @param [in] filePath         The path of the file to be written.
     * @param [in] writeHeaders     Writes "R,G,B" headers as the first row. Default = true, 
     * @param [in] scale_bitdepth   Scales the code values to the specified integer bitdepth.
     *                              Scaling is off when scale_bitdepth is 0
     * */
    void Write2CSV(const std::string& filePath, 
                   bool writeHeaders = true, 
                    uint16_t scale_bitdepth = 0);

    /// Writes to an Adobe(C)/IRIDAS(C) .cube file
    /**
     * Writes the data to an Adobe/IRIDAS cube file. 
     * \param [in] filePath  The path of the file to be written.
     * \param [in] Params    Pointer to an object containing the optional paramaters.
     *                       Default value = NULL.
     * 
     * Reference: \n
     * https://wwwimages2.adobe.com/content/dam/acom/en/products/speedgrade/cc/pdfs/cube-lut-specification-1.0.pdf
     * */
    void Write2CUBE_Adobe(const std::string& filePath, 
                          const CUBE_Params* Params = NULL);
    
    /// Writes to a Black Magic .cube file
    /**
     * Writes the data to a Black Magic cube file. 
     * \param [in] filePath  The path of the file to be written.
     * \param [in] Params    Pointer to an object containing the optional paramaters.
     *                       Default value = NULL.
     * */
    void Write2CUBE_BM(const std::string& filePath, 
                       const CUBE_Params* Params = NULL);

    /// Reads from a specified CSV file containing 3D LUT data.
    /** 
     * The data within the CSV file should be in RGB format, where the blue channel
     * changes most frequently. If the number of nodes has been specified via the 
     * constructor, that number will be used. Otherwise, the number of nodes will be 
     * determined by the number of rows in the CSV file.
     * 
     * \param [in] FilePath The path of the file to be read.
     * \param [in] scale_bitdepth The bitdepth the code values must be scaled down from.
     *             Scaling is ignored when scale_bitdepth is 0. In this case, values should 
     *             range between 0 and 1 (float).
     * */
    void ReadFromCSV(const std::string& filePath,
                     uint8_t scale_bitdepth = 0);

    /// Reads from a specified cube file (Adobe or BM) containting the 3D LUT data.
    /** 
     * Reads data from a 3D cube file. The LUT size is determined based on the keywords within
     * the file. This method will work for both Adobe cube files and Black Magic Cube Files.
     * @param [in] filePath The file that should be read.
     * @param [in] Params If provided, the optional keyword paramaters will be filled for later usage.
     * */
    void ReadFromCubeFile(const std::string& filePath,
                          CUBE_Params* Params = NULL);



    // wite to image

    // read from image

    /// Returns the interpolated values of the supplied 3D LUT using the trilinear interpolation method.
    /**
     * Uses the trilinear interpolation method to generate output cove values. All inputs should be normalized 
     * between 0-1. 
     * @param [in] R the input code value for the red channel.
     * @param [in] G the input code value for the green channel.
     * @param [in] B the input code value for the blue channel.
     * @return An array of floating point numbers in the order of [RGB]
     * 
     * */
    tripletF Interpolate_Trilin(const float R, const float G, const float B);

    /// Returns the interpolated values of the supplied 3D LUT using the tetrahedral interpolation method.
    /**
     * Uses the tetrahedral interpolation method to generate output cove values. All inputs should be normalized 
     * between 0-1. 
     * @param [in] R the input code value for the red channel.
     * @param [in] G the input code value for the green channel.
     * @param [in] B the input code value for the blue channel.
     * @return An array of floating point numbers in the order of [RGB]
     * 
     * </br></br>Reference: https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&ved=2ahUKEwi7_Ozzq5Lt
     *AhWPdN8KHY_RBJkQFjADegQIBBAC&url=https%3A%2F%2Fcommunity.acescentral.com%2Fuploads%2Fdefault%2Foriginal%2F
     *2X%2F5%2F518c5ede1ca11c4a7e13f9c7350e2228bb8762c7.pdf&usg=AOvVaw0V1HmuA841HaKUIuSEPWt4
     * */   
    tripletF Interpolate_Tetra(const float R, const float G, const float B);

    /// Generates a 3D LUT using a supplied 3 x n eigen matrix
    /** 
     * This method generates a matrix based on the supplied matrix as an Eigen::Marix3Xf. Standard values
     * should be between 0-1. Each row should correspond to RGB out. 
     * @param [in] inputMatrix The matrix to generate the LUT from 
     * 
     * */
    void GenerateFromMatrix(const Eigen::Matrix3Xf& inputMatrix);

    /// Generates a 3D LUT using a supplied function.
    /**
     * This function will generate a 3D LUT based on a supplied function, passed as a 
     * function pointer. The function signature should be:
     * 
     * \code{.cpp}
     *                           // Input CVs normalized 0-1.
     *  MPS::tripletF yourFunction(float r, float g, float b){
     *      // your code
     * 
     *      return{Rout, Gout, Bout};
     *  }
     * \endcode
     * 
     * This method allows more complex LUTs to be generated (including non-linear) than what is possible 
     * with a 3 x n matrix.
     * 
     * \param [in] function Pointer to the function that should be used to generate the LUT
     * */
    void GenerateFromFunction(tripletF (*function)(float, float, float));

    /// Generates a 3D LUT using a supplied function.
    /** 
     * \overload
     * 
     * Note: The Python API binds to this function
     * */
    void GenerateFromFunction(std::function<tripletF(float, float, float)> &function);
};
}
