// GetPrimaries.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// July 28, 2019

#include <iostream>
#include "ColorSpaces.hpp"
#include "deltaE.hpp"

using namespace std;

int main(){

    MPS::colorPrimaries Rec709_Primaries(MPS::Rec709);
    MPS::phosphorMatrix PM(Rec709_Primaries);

    Eigen::Matrix3f inverseMatrix = PM.getInvPM();

    cout << PM.getPM() << endl;
    cout << inverseMatrix << endl;
    
    return EXIT_SUCCESS;
}