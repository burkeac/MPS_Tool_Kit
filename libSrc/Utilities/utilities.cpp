// utilities.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// June 2, 2020

#include <chrono>
#include <iostream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include "utilities.hpp"

namespace MPS {
    ScopeTimer::ScopeTimer(){
        std::cout << "construced" << std::endl;
        start = std::chrono::high_resolution_clock::now();
    }

    ScopeTimer::~ScopeTimer(){
        std::cout << "destructed" << std::endl;

        end = std::chrono::high_resolution_clock::now();

        duration = end-start;

        std::cout
            << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() 
            << " ms \n" 
            << std::chrono::duration_cast<std::chrono::seconds>(duration).count()
            << " s"
            << std::endl;
    }

    std::string strip(const std::string& input){
        size_t first = input.find_first_not_of(" \t");
        size_t last = input.find_last_not_of(" \t");

        return input.substr(first, last-first+1);
    }

    std::string ExtensionFromPath(const std::string& path, const bool ToLower){
        size_t i = path.find_last_of(".");
        if(i == std::string::npos)
            throw std::invalid_argument( "'" + path + "' does not contain a file extension.");
        std::string extension = path.substr(i+1, path.size()-1);
        if(ToLower) 
            std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
        return extension;
    }

    

}