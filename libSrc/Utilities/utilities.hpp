// utilities.hpp
// Adam C. Burke (adam.burke603@gmail.com)
// June 2, 2020

#pragma once
#include <chrono>

namespace MPS{


    /// Can be used to time a scope's execution time. Object should be constructed at the begining of the scope.
    /** 
     * Uppon destruction, of the class (at end of scope) the ellapsed time is output using std::cout. \n
     * example: \n \n
     * 
     * int main(){ \n
     *      { // start of scope \n
     *          MPS::ScopeTimer Timer; \n
     *          //(your code here) \n
     *      } // end of scope. Time printed. \n
     * return(1); \n
     * }
     * */
    class ScopeTimer {
        private:
            std::chrono::time_point<std::chrono::steady_clock> start, end;
            std::chrono::duration<float> duration;
        public:
            ScopeTimer();
            ~ScopeTimer();
        };
}