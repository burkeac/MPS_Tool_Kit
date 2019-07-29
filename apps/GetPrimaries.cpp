// GetPrimaries.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// July 28, 2019

#include <iostream>
#include "ColorSpaces.hpp"

using namespace std;

int main(){
    MPS::colorPrimaries Rec709;
    Rec709.SelectPrimary(MPS::Rec2020);

    for(int i=0; i<Rec709.Primaries.size(); i++){
        cout << Rec709.Primaries[i] << endl;
    }

    return EXIT_SUCCESS;
}