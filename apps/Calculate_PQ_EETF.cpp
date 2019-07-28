// Calculate_PQ_EETF.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// July 27, 2019

#include <iostream>
#include "HDR_TranFunc.hpp"

using namespace std;

int main(int argv, char * argc[]){

    float i = 4000;
    float j = MPS::Linear_2_PQ(i);

    cout << j << endl;

    return EXIT_SUCCESS;
}