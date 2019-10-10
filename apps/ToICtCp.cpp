// ToICtCp.cpp | App
// Adam C. Burke (adam.burke603@gmail.com)
// September 6, 2019

#include <iostream>
#include "ColorSpaces.hpp"
#include <vector>
#include <fstream>
#include <string>

using namespace std;

int main(int argv, char * argc[]){

    if(argv == 4){
    
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
    }else if(argv == 2){
        //file
        string line, R, G, B;
        fstream input(argc[1]);
        if(input.is_open()){

            vector<float> ICtCp;
            while(!input.eof()){

                input >> R; input >> G; input >> B;
  
                ICtCp = MPS::Rec2020_to_ICtCp( stof(R), stof(G), stof(B) );
                cout << ICtCp[0] << "," << ICtCp[1] << "," << ICtCp[2] << endl;
            }
            input.close();
        }
    }else{ 
        return EXIT_FAILURE;
    }
}