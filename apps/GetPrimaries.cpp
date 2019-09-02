// GetPrimaries.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// July 28, 2019

#include <iostream>
#include "ColorSpaces.hpp"

using namespace std;

int main(){

    const vector<float> Rec709_Primaries  =  {0.64, 0.33, 0.30, 0.60, 0.15, 0.06};

    // cout << "Gahhhh" << Rec709_Primaries[0]<< endl;


    MPS::colorPrimaries Rec709;
    Rec709.selectPrimary(MPS::Rec709);
    
    MPS::phosphorMatrix PM(Rec709);

    cout << PM.generatePMs(Rec709) << endl;
    

    return EXIT_SUCCESS;
}