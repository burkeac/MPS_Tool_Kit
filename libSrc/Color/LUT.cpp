// LUT.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// October 13, 2020

#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <string>

#ifndef _WIN32
    #include "../../ThirdParty/Eigen/Core"
    #include "../../ThirdParty/Eigen/Eigen"
#else
    #include "..\..\ThirdParty\Eigen\Core"
    #include "..\..\ThirdParty\Eigen\Eigen"
#endif

#include "LUT.hpp"
#include "utilities.hpp"
 
enum{ red, green, blue };

namespace MPS{

LUT3D::LUT3D(const uint8_t numNodes)
    : numNodes(numNodes){
    _Allocate3D_Data();
}
LUT3D::LUT3D(){};

void LUT3D::_Allocate3D_Data(){
    typedef std::vector<tripletF> row;
    typedef std::vector<row> plane;
    typedef std::vector<plane> cube;

    // incilize the data structure
    _data = LUT3D_Data(numNodes,plane(numNodes, row(numNodes)));

    _NodeValueLUT.reserve(numNodes);
    for(int i=0; i<numNodes; i++){
        _NodeValueLUT.push_back((float )i / (numNodes-1));
    }

}

void LUT3D::Generate_3DLUT_Nodes(){
    float spacing = 1.0 / ((float)numNodes-1.0);

    for(int R=0; R<numNodes; R++){
        for(int G=0; G<numNodes; G++){
            for(int B=0; B<numNodes; B++){
                _data[R][G][B][red] = R * spacing;
                _data[R][G][B][green] = G * spacing;
                _data[R][G][B][blue] = B * spacing;
            }
        }
    }
}

int LUT3D::GetNumNodes(){
    return numNodes;
}

void LUT3D::Write2CSV(const std::string& filePath, 
                      bool writeHeaders, 
                      uint16_t scale_bitdepth){
    
    std::fstream fileOut(filePath, std::ios::out);
    if(fileOut.good()){
        if(writeHeaders)
            fileOut << "R,G,B" << std::endl;

        if(!scale_bitdepth){
            for(int R=0; R<numNodes; R++){
                for(int G=0; G<numNodes; G++){
                    for(int B=0; B<numNodes; B++){
                        fileOut << _data[R][G][B][red] << "," 
                                << _data[R][G][B][green] << "," 
                                << _data[R][G][B][blue] << "\n";
                    }
                }
            }
        }else{
            int NumCVs = pow(2, scale_bitdepth);
            for(int R=0; R<numNodes; R++){
                for(int G=0; G<numNodes; G++){
                    for(int B=0; B<numNodes; B++){
                        fileOut << ((_data[R][G][B][red]   * NumCVs)   < (NumCVs-1) 
                            ? (_data[R][G][B][red]   * NumCVs) : (NumCVs-1)) 
                        << ","  << ((_data[R][G][B][green] * NumCVs)   < (NumCVs-1) 
                            ? (_data[R][G][B][green] * NumCVs) : (NumCVs-1)) 
                        << ","  << ((_data[R][G][B][blue]  * NumCVs)   < (NumCVs-1) 
                            ? (_data[R][G][B][blue]  * NumCVs) : (NumCVs-1)) 
                        << "\n";
                    }
                }
            }
        }

        fileOut.flush();
        fileOut.close();
    }
}

void LUT3D::Write2CUBE_Adobe(const std::string& filePath, 
                             const CUBE_Params* Params){

    LUT3D::CUBE_Params l_Params;

    // write any and all keywords to file
    std::fstream fileOut(filePath, std::ios::out);
    if(fileOut.good()){
        if(Params){ // optional keywords
            if(Params->TITLE != "")
                fileOut << "TITLE " << Params->TITLE << "\n";

            tripletF DefaultMin = {0,0,0};
            if(Params->DOMAIN_MIN != DefaultMin)
                fileOut << "DOMAIN_MIN " 
                        << Params->DOMAIN_MIN[0] << " " 
                        << Params->DOMAIN_MIN[1] << " "
                        << Params->DOMAIN_MIN[2] << "\n";

            tripletF DefaultMax = {1,1,1};
            if(Params->DOMAIN_MAX != DefaultMax)
                fileOut << "DOMAIN_MAX " 
                        << Params->DOMAIN_MAX[0] << " " 
                        << Params->DOMAIN_MAX[1] << " "
                        << Params->DOMAIN_MAX[2] << "\n";

            l_Params = *Params;
        }

        // required keywords
        fileOut << "LUT_3D_SIZE " << (int)numNodes << "\n";

        // write the code values out...
        for(int B=0; B<numNodes; B++){
            for(int G=0; G<numNodes; G++){
                for(int R=0; R<numNodes; R++){
                    fileOut << _data[R][G][B][red]   * (l_Params.DOMAIN_MAX[0] - l_Params.DOMAIN_MIN[0]) 
                                + l_Params.DOMAIN_MIN[0] << " " 
                            << _data[R][G][B][green] * (l_Params.DOMAIN_MAX[1] - l_Params.DOMAIN_MIN[1]) 
                                + l_Params.DOMAIN_MIN[1] << " " 
                            << _data[R][G][B][blue]  * (l_Params.DOMAIN_MAX[2] - l_Params.DOMAIN_MIN[2]) 
                                + l_Params.DOMAIN_MIN[2] << "\n"; 
                }
            }
        }

        //closet the file
        fileOut.flush();
        fileOut.close();
    }
}

void LUT3D::Write2CUBE_BM(const std::string& filePath, 
                          const CUBE_Params* Params){

    LUT3D::CUBE_Params l_Params;

    // write any and all keywords to file
    std::fstream fileOut(filePath, std::ios::out);
    if(fileOut.good()){
        if(Params){ // optional keywords
            if(Params->TITLE != "")
                fileOut << "TITLE " << Params->TITLE << "\n";

            if( (Params->DOMAIN_MIN[0] != 0) || (Params->DOMAIN_MAX[0] != 1) )
                fileOut << "LUT_3D_INPUT_RANGE " 
                        << Params->DOMAIN_MIN[0] << " "
                        << Params->DOMAIN_MAX[0] << "\n";

            l_Params = *Params;
        }

        // required keywords
        fileOut << "LUT_3D_SIZE " << (int)numNodes << "\n";

        // write the code values out...
        for(int B=0; B<numNodes; B++){
            for(int G=0; G<numNodes; G++){
                for(int R=0; R<numNodes; R++){
                    fileOut << _data[R][G][B][red]   * (l_Params.DOMAIN_MAX[0] - l_Params.DOMAIN_MIN[0]) 
                                + l_Params.DOMAIN_MIN[0] << " " 
                            << _data[R][G][B][green] * (l_Params.DOMAIN_MAX[1] - l_Params.DOMAIN_MIN[1]) 
                                + l_Params.DOMAIN_MIN[1] << " " 
                            << _data[R][G][B][blue]  * (l_Params.DOMAIN_MAX[2] - l_Params.DOMAIN_MIN[2]) 
                                + l_Params.DOMAIN_MIN[2] << "\n"; 
                }
            }
        }

        //closet the file
        fileOut.flush();
        fileOut.close();
    }
}

void LUT3D::ReadFromCSV(const std::string& filePath, 
                        uint8_t scale_bitdepth){
   
    CSVreader CSVFile(filePath, true);
    auto csvdata = CSVFile.getRef();
    bool hasHeaders = false;
    int line = 0;
    for(int i=0; i<3; i++){
        for(auto c : csvdata[0][0]){
            if( !isdigit(c) ){
                line = 1;
                hasHeaders = true;
                break;
            }
        }
    }
    
    if(numNodes == 0){
        if(hasHeaders)
            numNodes = std::cbrt(csvdata.size()-1);
        else
            numNodes = std::cbrt(csvdata.size());
        _Allocate3D_Data();
    }

    if(scale_bitdepth == 0){
        for(int R=0; R<numNodes; R++){
            for(int G=0; G<numNodes; G++){
                for(int B=0; B<numNodes; B++){
                    try{
                        _data[R][G][B][0] = std::stof(csvdata[line][0]);
                        _data[R][G][B][1] = std::stof(csvdata[line][1]);
                        _data[R][G][B][2] = std::stof(csvdata[line][2]);
                        line++;
                    }catch(...){
                        throw(std::invalid_argument("Cannot convert value on line: " + 
                            std::to_string(line) + " to numeric value. Aborting"));
                    }
                }
            }
        }
    }else{
    // if scaling is needed
        float scaleFactor = pow(2, scale_bitdepth) - 1;

        for(int R=0; R<numNodes; R++){
            for(int G=0; G<numNodes; G++){
                for(int B=0; B<numNodes; B++){
                    try{
                        _data[R][G][B][0] = std::stof(csvdata[line][0]) / scaleFactor;
                        _data[R][G][B][1] = std::stof(csvdata[line][1]) / scaleFactor;
                        _data[R][G][B][2] = std::stof(csvdata[line][2]) / scaleFactor;
                        line++;
                        
                    }catch(...){
                        throw(std::invalid_argument("Cannot convert value on line: " + 
                            std::to_string(line) + " to numeric value. Aborting"));
                    }
                }
            }
        }
    }
}

// read from cube file
void LUT3D::ReadFromCubeFile(const std::string& filePath,
                             CUBE_Params* Params){
    std::fstream fileIn(filePath, std::ios::in);
    if(!fileIn.is_open()){
        throw(std::invalid_argument("Can't open file specified."));
        return;
    }

    tripletF DOMAIN_MIN = {0,0,0};
    tripletF DOMAIN_MAX = {1,1,1};
    bool NormalizationNeeded = false;
    

    int R, G, B = 0;

    std::string line;
    std::string value;

    while(getline(fileIn, line)){
        std::stringstream lineS(line);

        lineS >> value;
        
        // if comment line: skip
        if(value[0] == '#') continue; 

        if(value == "LUT_3D_SIZE"){
            lineS >> value;
            numNodes = stof(value);
            _Allocate3D_Data();
        }
        
        else if(value == "TITLE"){
            std::string title;
            getline(lineS, value, '\n');
            if(Params)
                Params->TITLE = MPS::strip(value);
        }

        else if(value == "DOMAIN_MIN"){
            lineS >> value;
            DOMAIN_MIN[red] = stof(value);
            if(Params) Params->DOMAIN_MIN[red] = stof(value);

            lineS >> value;
            DOMAIN_MIN[green] = stof(value);
            if(Params) Params->DOMAIN_MIN[green] = stof(value);

            lineS >> value;
            DOMAIN_MIN[blue] = stof(value);
            if(Params) Params->DOMAIN_MIN[blue] = stof(value);
        }

        else if(value == "DOMAIN_MAX"){
            lineS >> value;
            DOMAIN_MAX[red] = stof(value);
            if(Params) Params->DOMAIN_MAX[red] = stof(value);

            lineS >> value;
            DOMAIN_MAX[green] = stof(value);
            if(Params) Params->DOMAIN_MAX[green] = stof(value);

            lineS >> value;
            DOMAIN_MAX[blue] = stof(value);
            if(Params) Params->DOMAIN_MAX[blue] = stof(value);
        }

        else if (value == "LUT_3D_INPUT_RANGE"){
            lineS >> value;
            DOMAIN_MIN[red] = stof(value);
            DOMAIN_MIN[green] = stof(value);
            DOMAIN_MIN[blue] = stof(value);
            if(Params) Params->DOMAIN_MIN[red] = stof(value);
            if(Params) Params->DOMAIN_MIN[green] = stof(value);
            if(Params) Params->DOMAIN_MIN[blue] = stof(value);

            lineS >> value;
            DOMAIN_MAX[red] = stof(value);
            DOMAIN_MAX[green] = stof(value);
            DOMAIN_MAX[blue] = stof(value);
            if(Params) Params->DOMAIN_MAX[red] = stof(value);
            if(Params) Params->DOMAIN_MAX[green] = stof(value);
            if(Params) Params->DOMAIN_MAX[blue] = stof(value);
        }

        // parse values
        else {
            _data[R][G][B][red] =   (stof(value) - DOMAIN_MIN[0]) / (DOMAIN_MAX[0] - DOMAIN_MIN[0]);
            lineS >> value;
            _data[R][G][B][green] = (stof(value) - DOMAIN_MIN[1]) / (DOMAIN_MAX[1] - DOMAIN_MIN[1]);
            lineS >> value;
            _data[R][G][B][blue] =  (stof(value) - DOMAIN_MIN[2]) / (DOMAIN_MAX[2] - DOMAIN_MIN[2]);
        
        
            // increment channel indexes appropriatly
            R++;
            if(R >= numNodes){ G++; R = 0; }
            if(G >= numNodes){ B++; G = 0; }
            if(B >= numNodes){ break; }
        }

    }

    fileIn.close();
}

inline float LUT3D::_getC(const tripletB node, const tripletF CV, const int chan){
    float multFactor = numNodes -1;
    
    int a = node[0] ? std::ceil(CV[0] * multFactor) : std::floor(CV[0] * multFactor);
    int b = node[1] ? std::ceil(CV[1] * multFactor) : std::floor(CV[1] * multFactor);
    int c = node[2] ? std::ceil(CV[2] * multFactor) : std::floor(CV[2] * multFactor);
    return _data[a][b][c][chan];
}

inline float LUT3D::_getScaleFactor(const float cord){
    float multFactor = numNodes - 1; 

    float din = _NodeValueLUT[std::ceil(cord * multFactor)] -
                 _NodeValueLUT[std::floor(cord * multFactor)];
    if(din)
        return ( (cord - _NodeValueLUT[std::floor(cord * multFactor)]) / din );
    else
        return 0;
}

tripletF LUT3D::Interpolate_Trilin(const float R, const float G, const float B){
    // interp red channel
    float rC1 = _getC({false,false,false}, {R,G,B}, 0);
    float rC2 = _getC({true,false,false},  {R,G,B}, red);
    float rC3 = _getC({false,true,false},  {R,G,B}, red);
    float rC4 = _getC({true,true,false},   {R,G,B}, red);
    float rC5 = _getC({false,false,true},  {R,G,B}, red);
    float rC6 = _getC({true, false, true}, {R,G,B}, red);
    float rC7 = _getC({false,true,true},   {R,G,B}, red);
    float rC8 = _getC({true,true,true},    {R,G,B}, red);

    
    float rC12 = _getScaleFactor(R) * (rC2-rC1) + rC1;
    float rC34 = _getScaleFactor(R) * (rC4-rC3) + rC3;
    float rC56 = _getScaleFactor(R) * (rC6-rC5) + rC5;
    float rC78 = _getScaleFactor(R) * (rC8-rC7) + rC7;

    float rC1234 = _getScaleFactor(G) * (rC34 - rC12) + rC12;
    float rC5678 = _getScaleFactor(G) * (rC78 - rC56) + rC56;

    float rC = _getScaleFactor(B) * (rC5678 - rC1234) + rC1234;
    
    // interp green channel
    float gC1 = _getC({false,false,false}, {R,G,B}, green);
    float gC2 = _getC({true,false,false},  {R,G,B}, green);
    float gC3 = _getC({false,true,false},  {R,G,B}, green);
    float gC4 = _getC({true,true,false},   {R,G,B}, green);
    float gC5 = _getC({false,false,true},  {R,G,B}, green);
    float gC6 = _getC({true, false, true}, {R,G,B}, green);
    float gC7 = _getC({false,true,true},   {R,G,B}, green);
    float gC8 = _getC({true,true,true},    {R,G,B}, green);

    float gC12 = _getScaleFactor(R) * (gC2-gC1) + gC1;
    float gC34 = _getScaleFactor(R) * (gC4-gC3) + gC3;
    float gC56 = _getScaleFactor(R) * (gC6-gC5) + gC5;
    float gC78 = _getScaleFactor(R) * (gC8-gC7) + gC7;

    float gC1234 = _getScaleFactor(G) * (gC34 - gC12) + gC12;
    float gC5678 = _getScaleFactor(G) * (gC78 - gC56) + gC56;

    float gC = _getScaleFactor(B) * (gC5678 - gC1234) + gC1234;

    // interp blue channel
    float bC1 = _getC({false,false,false}, {R,G,B}, blue);
    float bC2 = _getC({true,false,false},  {R,G,B}, blue);
    float bC3 = _getC({false,true,false},  {R,G,B}, blue);
    float bC4 = _getC({true,true,false},   {R,G,B}, blue);
    float bC5 = _getC({false,false,true},  {R,G,B}, blue);
    float bC6 = _getC({true, false, true}, {R,G,B}, blue);
    float bC7 = _getC({false,true,true},   {R,G,B}, blue);
    float bC8 = _getC({true,true,true},    {R,G,B}, blue);

    float bC12 = _getScaleFactor(R) * (bC2-bC1) + bC1;
    float bC34 = _getScaleFactor(R) * (bC4-bC3) + bC3;
    float bC56 = _getScaleFactor(R) * (bC6-bC5) + bC5;
    float bC78 = _getScaleFactor(R) * (bC8-bC7) + bC7;

    float bC1234 = _getScaleFactor(G) * (bC34 - bC12) + bC12;
    float bC5678 = _getScaleFactor(G) * (bC78 - bC56) + bC56;

    float bC = _getScaleFactor(B) * (bC5678 - bC1234) + bC1234;


    return {rC,gC,bC};
}

tripletF LUT3D::Interpolate_Tetra(const float R, const float G, const float B){
    float deltaR = _getScaleFactor(R);
    float deltaG = _getScaleFactor(G);
    float deltaB = _getScaleFactor(B);

    Eigen::MatrixXf T1(4,8);
    T1 << 1.f, 0.f, 0.f, 0.f, 0.f, 0.f,  0.f, 0.f,
         -1.f, 0.f, 0.f, 0.f, 1.f, 0.f,  0.f, 0.f,
          0.f, 0.f, 0.f, 0.f,-1.f, 0.f,  1.f, 0.f,
          0.f, 0.f, 0.f, 0.f, 0.f, 0.f, -1.f, 1.f;
    
    Eigen::MatrixXf T2(4,8);
    T2 << 1.f, 0.f, 0.f, 0.f,  0.f,  0.f, 0.f, 0.f,
         -1.f, 0.f, 0.f, 0.f,  1.f,  0.f, 0.f, 0.f,
          0.f, 0.f, 0.f, 0.f,  0.f, -1.f, 0.f, 1.f,
          0.f, 0.f, 0.f, 0.f, -1.f,  1.f, 0.f, 0.f;

    Eigen::MatrixXf T3(4,8);
    T3 << 1.f,  0.f, 0.f, 0.f, 0.f,  0.f, 0.f, 0.f,
          0.f, -1.f, 0.f, 0.f, 0.f,  1.f, 0.f, 0.f,
          0.f,  0.f, 0.f, 0.f, 0.f, -1.f, 0.f, 1.f,
         -1.f,  1.f, 0.f, 0.f, 0.f,  0.f, 0.f, 0.f;

    Eigen::MatrixXf T4(4,8);
    T4 <<  1.f, 0.f,  0.f, 0.f, 0.f, 0.f,  0.f, 0.f,
           0.f, 0.f, -1.f, 0.f, 0.f, 0.f,  1.f, 0.f,
          -1.f, 0.f,  1.f, 0.f, 0.f, 0.f,  0.f, 0.f,
           0.f, 0.f,  0.f, 0.f, 0.f, 0.f, -1.f, 1.f;

    Eigen::MatrixXf T5(4,8);
    T5 <<  1.f, 0.f,  0.f,  0.f, 0.f, 0.f, 0.f, 0.f,
           0.f, 0.f,  0.f, -1.f, 0.f, 0.f, 0.f, 1.f,
          -1.f, 0.f,  1.f,  0.f, 0.f, 0.f, 0.f, 0.f,
           0.f, 0.f, -1.f,  1.f, 0.f, 0.f, 0.f, 0.f;

    Eigen::MatrixXf T6(4,8);
    T6 <<  1.f,  0.f, 0.f,  0.f, 0.f, 0.f, 0.f, 0.f,
           0.f,  0.f, 0.f, -1.f, 0.f, 0.f, 0.f, 1.f,
           0.f, -1.f, 0.f,  1.f, 0.f, 0.f, 0.f, 0.f,
          -1.f,  1.f, 0.f,  0.f, 0.f, 0.f, 0.f, 0.f;

    Eigen::VectorXf deltaT(4);
    deltaT << 1, deltaB, deltaR, deltaG;

    tripletF result = {0,0,0};

    // For the red CV
    Eigen::VectorXf V_red(8);
    V_red << _getC({0,0,0}, {R,G,B}, red),
            _getC({0,1,0}, {R,G,B}, red),
            _getC({1,0,0}, {R,G,B}, red),
            _getC({1,1,0}, {R,G,B}, red),
            _getC({0,0,1}, {R,G,B}, red),
            _getC({0,1,1}, {R,G,B}, red),
            _getC({1,0,1}, {R,G,B}, red),
            _getC({1,1,1}, {R,G,B}, red);

    Eigen::VectorXf V_green(8);
    V_green << _getC({0,0,0}, {R,G,B}, green),
               _getC({0,1,0}, {R,G,B}, green),
               _getC({1,0,0}, {R,G,B}, green),
               _getC({1,1,0}, {R,G,B}, green),
               _getC({0,0,1}, {R,G,B}, green),
               _getC({0,1,1}, {R,G,B}, green),
               _getC({1,0,1}, {R,G,B}, green),
               _getC({1,1,1}, {R,G,B}, green);

    Eigen::VectorXf V_blue(8);
    V_blue << _getC({0,0,0}, {R,G,B}, blue),
              _getC({0,1,0}, {R,G,B}, blue),
              _getC({1,0,0}, {R,G,B}, blue),
              _getC({1,1,0}, {R,G,B}, blue),
              _getC({0,0,1}, {R,G,B}, blue),
              _getC({0,1,1}, {R,G,B}, blue),
              _getC({1,0,1}, {R,G,B}, blue),
              _getC({1,1,1}, {R,G,B}, blue);

    if((deltaB > deltaR) && (deltaR > deltaG)){
        result[red]   = (deltaT.transpose() * T1 * V_red  ).value();
        result[green] = (deltaT.transpose() * T1 * V_green).value();
        result[blue]  = (deltaT.transpose() * T1 * V_blue ).value();
    }
    else if((deltaB > deltaG) && (deltaG > deltaR)){
        result[red]   = (deltaT.transpose() * T2 * V_red  ).value();
        result[green] = (deltaT.transpose() * T2 * V_green).value();
        result[blue]  = (deltaT.transpose() * T2 * V_blue ).value();
    }
    else if((deltaG > deltaB) && (deltaB > deltaR)){
        result[red]   = (deltaT.transpose() * T3 * V_red  ).value();
        result[green] = (deltaT.transpose() * T3 * V_green).value();
        result[blue]  = (deltaT.transpose() * T3 * V_blue ).value();
    }
    else if((deltaR > deltaB) && (deltaB > deltaG)){
        result[red]   = (deltaT.transpose() * T4* V_red  ).value();
        result[green] = (deltaT.transpose() * T4* V_green).value();
        result[blue]  = (deltaT.transpose() * T4* V_blue ).value();
    }
    else if((deltaR > deltaG) && (deltaG > deltaB)){
        result[red]   = (deltaT.transpose() * T5 * V_red  ).value();
        result[green] = (deltaT.transpose() * T5 * V_green).value();
        result[blue]  = (deltaT.transpose() * T5 * V_blue ).value();
    }
    else{
        result[red]   = (deltaT.transpose() * T6 * V_red  ).value();
        result[green] = (deltaT.transpose() * T6 * V_green).value();
        result[blue]  = (deltaT.transpose() * T6 * V_blue ).value();
    }

    return result;
}

void LUT3D::GenerateFromMatrix(const Eigen::Matrix3Xf& inputMatrix){
    
    Eigen::Vector3f LUTcvs;
    float divFactor = numNodes - 1.0;

    for(int R=0; R<numNodes; R++){
        for(int G=0; G<numNodes; G++){
            for(int B=0; B<numNodes; B++){
                LUTcvs << (float) R / divFactor, (float) G / divFactor, (float) B / divFactor;

                auto result = inputMatrix * LUTcvs;
                _data[R][G][B][0] = result[0];
                _data[R][G][B][1] = result[1];
                _data[R][G][B][2] = result[2];

            }
        }
    }
}

void LUT3D::GenerateFromFunction(tripletF (*function)(float, float, float)){
    
    float divFactor = numNodes - 1.0;

    for(int R=0; R<numNodes; R++){
        for(int G=0; G<numNodes; G++){
            for(int B=0; B<numNodes; B++){
                tripletF result = function(R/divFactor, G/divFactor, B/divFactor);
                _data[R][G][B] = result;
            }
        }
    }
}

void LUT3D::GenerateFromFunction(std::function<tripletF(float, float, float)> &function){
    float divFactor = numNodes - 1.0;

    for(int R=0; R<numNodes; R++){
        for(int G=0; G<numNodes; G++){
            for(int B=0; B<numNodes; B++){
                tripletF result = function(R/divFactor, G/divFactor, B/divFactor);
                _data[R][G][B] = result;
            }
        }
    }
}

} // end namespace
