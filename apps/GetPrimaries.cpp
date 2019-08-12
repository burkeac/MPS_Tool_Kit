// GetPrimaries.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// July 28, 2019

#include <iostream>
#include "ColorSpaces.hpp"

using namespace std;

int main(){
    MPS::colorPrimaries Rec709;
    Rec709.selectPrimary(MPS::Rec2020);

    Rec709.printPrimaries();
    
    MPS::phosphorMatrix PM(Rec709, Rec709);
    

    return EXIT_SUCCESS;
}