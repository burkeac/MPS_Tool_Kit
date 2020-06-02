// utilities.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// June 2, 2020

#include <chrono>
#include <iostream>
#include "utilities.hpp"

namespace MPS {
    ScopeTimer::ScopeTimer(){
        start = std::chrono::steady_clock::now();
    }
    ScopeTimer::~ScopeTimer(){
        end = std::chrono::steady_clock::now();
        std::cout
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
            << " ms \n" 
            << std::chrono::duration_cast<std::chrono::seconds>(end - start).count()
            << " s"
            << std::endl;
    }
}