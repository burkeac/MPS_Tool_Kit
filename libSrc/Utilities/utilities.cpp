// utilities.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// June 2, 2020

#include <chrono>
#include <iostream>
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

    

}