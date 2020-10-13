// testProgram.cpp


#include <iostream>
#include "utilities.hpp"
#include "deltaE.hpp"
#include "ColorSpaces.hpp"

#include <thread>
#include <chrono>

using namespace std;

int main(int argc, char* argv[]){

    MPS::CSVreader data(argv[1]);

    cout << data[1][1] << endl;

    // std::cout << data[0][1] << std::endl;

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