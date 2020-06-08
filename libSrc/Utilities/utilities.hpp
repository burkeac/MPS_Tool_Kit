// utilities.hpp
// Adam C. Burke (adam.burke603@gmail.com)
// June 2, 2020

#pragma once
#include <chrono>
#include <map>

namespace MPS {

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

    class ProgramOptions {
        private:
            // std::map<std::string, std::string>
            std::map<std::string, bool> _optionFlags;
            std::map<std::string, std::string> _optionNames;
            std::map<std::string, int> _numOptArgs;
            // std::map<std::string, std::string> _helpText;
            bool _flagDoesExist(std::string flag);
            int _argc;
            char** _argv;

        public:
        // Constructor
            ProgramOptions(int argc, char* argv[]);
            
        // Methdos
            /// Adds and option flag
            /** 
             * 
             * */
            void addOption(const std::string flag, 
                           const std::string name,  
                           const int numArgs = 0);

            /// Returns the state of the object (true or false)
            /** 
             * If the flag does not exist, the method returns false 
             * and throws a std::invalid_argument exception. This should 
             * be caught and handled by the apps implementation.
             * */
            bool optionIsEnabled(const std::string flagOrName);

            void addArgument(const std::string argumentName);

            void parseInput();
    };
}