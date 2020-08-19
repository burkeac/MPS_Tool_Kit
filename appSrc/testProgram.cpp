// testProgram.cpp


#include <iostream>
#include "utilities.hpp"

using namespace std;

int main(int argc, char* argv[]){

    MPS::CSVreader data(argv[1]);

    try{
    data.read(false);
    } catch (string err) {
        cout << err << endl;
        return EXIT_FAILURE;
    }


    auto myData = data.getRef();

    myData[0][1] = "test";

    std::cout << data.at(0,1) << std::endl;

    std::cout << data[0][1] << std::endl;

    return EXIT_SUCCESS;
}