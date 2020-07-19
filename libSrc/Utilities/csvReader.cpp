// csvReader.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// July 18, 2020

#include "utilities.hpp"

#include <iostream>

namespace MPS {
    CSVreader::CSVreader() 
        : filePath("") {}


    CSVreader::CSVreader(const std::string& filePath)
        : filePath(filePath) {
            std::cout << this->filePath << std::endl;
        }
    
}