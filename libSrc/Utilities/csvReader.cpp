// csvReader.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// July 18, 2020

#include "utilities.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>

namespace MPS {

    CSVreader::CSVreader() 
        : filePath("") {}

    CSVreader::CSVreader(const std::string& filePath, bool strip)
        : filePath(filePath) { 
            read(strip);
        }
    
    std::vector<std::vector<std::string>>& CSVreader::read(bool strip){

        // Open file from path
        std::fstream file(filePath, std::fstream::in);
        if( !file.is_open() ){
            throw (std::string) "Can not open file: " + filePath;
            return data;
        }

        std::string line;

        while(getline(file, line)){

            std::stringstream lineStream(line);
            std::string element;
            std::vector<std::string> dataRow;

            while(getline(lineStream, element, ',')){
                if(strip) element = MPS::strip(element);
            
                dataRow.push_back(element);
            }

            data.push_back(dataRow);
        }
        file.close();
        return data;
    }

    std::vector<std::vector<std::string>>& CSVreader::getRef() {
        return data; // returns a reference to the data stored within the option!
    }

    std::string& CSVreader::at(size_t row, size_t col){
        return data[row][col];
    }

    std::vector<std::string>& CSVreader::getRow(size_t row) {
        return data[row];
    }
    
    std::vector<std::string>& CSVreader::operator[](size_t row){
        return data[row];
    }

}