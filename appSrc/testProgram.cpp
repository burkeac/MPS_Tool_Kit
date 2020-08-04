// testProgram.cpp


#include <iostream>
#include "utilities.hpp"

int main(){

    MPS::CSVreader file("testFilePath");
    std::cout << file.filePath << std::endl;
    return EXIT_SUCCESS;
}