// ToICtCp.cpp | App
// Adam C. Burke (adam.burke603@gmail.com)
// September 6, 2019

#include <iostream>
#include "ColorSpaces.hpp"
#include <vector>

using namespace std;

int main(int argv, char * argc[]){

    if(argv != 4) return EXIT_FAILURE;
    
    float R = stof(argc[1]);
    float G = stof(argc[2]);
    float B = stof(argc[3]);

    cout << "[in] Rec2020 R: " << R << endl;
    cout << "[in] Rec2020 G: " << G << endl;
    cout << "[in] Rec2020 B: " << B << endl;

    vector<float> ICtCp = MPS::Rec2020_to_ICtCp(R, G, B);

    cout << "[out] I: "  << ICtCp[0] << endl;
    cout << "[out] Ct: " << ICtCp[1] << endl;
    cout << "[out] Cp: " << ICtCp[2] << endl;

    return EXIT_SUCCESS;
}