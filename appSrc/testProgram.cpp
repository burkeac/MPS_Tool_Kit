// testProgram.cpp


#include <iostream>
#include "utilities.hpp"
#include "deltaE.hpp"
#include "ColorSpaces.hpp"
#include "LUT.hpp"

    #include "../../ThirdParty/Eigen/Core"
    #include "../../ThirdParty/Eigen/Eigen"

#include <thread>
#include <chrono>

using namespace std;


MPS::tripletF myFunc(float r, float g, float b){

    float R = pow(r-.5,3) / .4 + pow(r-.5,2) / .4 + .1;
    float G = pow(g-.5,3) / .4 + pow(g-.5,2) / .4 + .1;
    float B = pow(b-.5,3) / .4 + pow(b-.5,2) / .4 + .1;


    return {
        // r*.9f + b*.1f, 
        // .1f*r + .8f*g + .1f*b, 
        // .2f*r + .8f*b
        R,G,B
        };
}

int main(int argc, char* argv[]){

    MPS::LUT3D LUT;
    // LUT.ReadFromCSV("/Users/adamburke/Documents/2019/MPS_Toolkit/build/roundTrip.csv", 10);

    MPS::LUT3D::CUBE_Params params;
    LUT.ReadFromCubeFile("/Users/adamburke/Documents/2019/MPS_Toolkit/build/CubeTest.cube", &params);

    cout << params.TITLE << endl;

    float scaleFactor = 1023.0;

    float Rin = stof(argv[1]) / scaleFactor;
    float Gin = stof(argv[2]) / scaleFactor;
    float Bin = stof(argv[3]) / scaleFactor;

    auto InterpResult = LUT.Interpolate_Trilin(Rin, Gin, Bin);
    cout << InterpResult[0] * scaleFactor << " " << InterpResult[1] * scaleFactor << " " << InterpResult[2] * scaleFactor << std::endl;


/*
    MPS::LUT3D LUT(17);
    // Eigen::Matrix3f matrix;
    // matrix << .9,0,.1,
    //           .1,.8,.1,
    //           .2,0,.8;
    // LUT.GenerateFromMatrix(matrix);


    LUT.GenerateFromFunction(myFunc);

    float scaleFactor = 1023.0;

    float Rin = stof(argv[1]) / scaleFactor;
    float Gin = stof(argv[2]) / scaleFactor;
    float Bin = stof(argv[3]) / scaleFactor;

    Eigen::Vector3f CVin;
    CVin << Rin, Gin, Bin;
    // auto MatrixResult = matrix * CVin;
    
    auto MatrixResult = myFunc(Rin, Gin, Bin);

    auto InterpResult = LUT.Interpolate_Trilin(Rin, Gin, Bin);
    auto InterpResult2 = LUT.Interpolate_Tetra(Rin, Gin, Bin);


    cout << "Result:   " << MatrixResult[0] * scaleFactor << " " << MatrixResult[1] * scaleFactor << " " << MatrixResult[2] * scaleFactor << std::endl;
    cout << "Interp 1: " << InterpResult[0] * scaleFactor << " " << InterpResult[1] * scaleFactor << " " << InterpResult[2] * scaleFactor << std::endl;
    cout << "Interp 2: " << InterpResult2[0] * scaleFactor << " " << InterpResult2[1] * scaleFactor << " " << InterpResult2[2] * scaleFactor << std::endl;
*/



    // LUT.ReadFromCSV(argv[1], 10);


    // auto interpTet = LUT.Interpolate_Tetra(stof(argv[2])/scaleFactor, stof(argv[3])/scaleFactor, stof(argv[4])/scaleFactor);
    // auto interpTri = LUT.Interpolate_Trilin(stof(argv[2])/scaleFactor, stof(argv[3])/scaleFactor, stof(argv[4])/scaleFactor);

    // cout << "\n\n";
    // cout << "Tri " << interpTri[0]*scaleFactor << " " << interpTri[1]*scaleFactor <<  " " << interpTri[2]*scaleFactor << endl;
    // cout << "Tet " << interpTet[0]*scaleFactor << " " << interpTet[1]*scaleFactor <<  " " << interpTet[2]*scaleFactor << endl;

    return EXIT_SUCCESS;
}


// int main() {
    // MPS::CIEdeltaE colorDif(50.0000, 2.5000, 0.0000, 58.0000, 24.0000, 15.0000);

    // MPS::CIEdeltaE colorSet1(60.2574, -34.0099, 36.2677, 60.4626, -34.1751, 39.4387);
    // cout << colorSet1.cie2000() <<  " 1.26442" << endl;
    // cout << colorSet1.cie76() <<    " 3.18192" << endl;
    // cout << colorSet1.cie94() <<    " 1.39099" << endl <<endl;

    // MPS::CIEdeltaE colorSet2(63.0109, -31.0961, -5.8663, 62.8187, -29.7946, -4.0864);
    // cout << colorSet2.cie2000() << " 1.26296" << endl;
    // cout << colorSet2.cie76()   << " 2.21334" << endl;
    // cout << colorSet2.cie94()   << " 1.24809" << endl <<endl;

    // MPS::CIEdeltaE colorSet3(61.2901, 3.7196, -5.3901, 61.4292, 2.2480, -4.9620);
    // cout << colorSet3.cie2000() << " 1.87307" << endl;
    // cout << colorSet3.cie76()   << "  1.5389" << endl;
    // cout << colorSet3.cie94()   << " 1.29796" << endl <<endl;


    // MPS::CIEdeltaE colorSet4(35.0831,-44.1164,3.7933, 35.0232,-40.0716,1.5901);
    // cout << colorSet4.cie2000() << " 1.8645" << endl;
    // cout << colorSet4.cie76()   << " 4.60631" << endl;
    // cout << colorSet4.cie94()   << " 1.82045" << endl <<endl;

    // MPS::CIEdeltaE colorSet5(2.0776,0.0795,-1.1350,0.9033,-0.0636,-0.5514);
    // cout << colorSet5.cie2000() << " 0.908233" << endl;
    // cout << colorSet5.cie76()   << " 1.31911" << endl;
    // cout << colorSet5.cie94()   << " 1.30654" << endl <<endl;


    // MPS::CIEdeltaE::cie76(2.0776,0.0795,-1.1350,0.9033,-0.0636,-0.5514);
    // MPS::CIEdeltaE_frmXYZ deltaE(1,1,1,1,1,1, MPS::D65);
    // std::cout << deltaE.cie2000() << std::endl;

    // {
    //     MPS::ScopeTimer timer;
    //     std::this_thread::sleep_for(chrono::duration(chrono::seconds(10)));
    // }

// }
