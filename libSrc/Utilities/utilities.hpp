// utilities.hpp
// Adam C. Burke (adam.burke603@gmail.com)
// June 2, 2020

#pragma once
#include <chrono>
#include <map>
#include <vector>
#include <string>

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
            std::map<std::string, bool> _optionFlags;
            std::map<std::string, std::string> _optionNames;
            std::map<std::string, int> _numOptArgs;
            std::map<std::string, std::string> _optionHelpText;
            std::map<std::string, std::string> _optionArguments;
            bool _flagDoesExist(std::string flag);
            bool _nameDoesExist(std::string name);
            bool _helpTextDoesExist(std::string flag);
            int _argc;
            char** _argv;

        public:
        // Constructor
            ProgramOptions(int argc, char* argv[]);

        // Public Members
            std::vector<std::string> arguments;
            std::vector<std::string> paramNames = {"<arguments>c"};
            std::string overRideProgramName = "";

        // Methdos
            void addArgument(const std::string argumentName);

            /// Adds and option flag
            /** 
             * 
             * */
            void addOption(const std::string flag, 
                           const std::string name,  
                           const int numArgs = 0, // make -1 assign the rest of the params to this option
                           const bool flaggable = true);

            /// Returns the state of the object (true or false)
            /** 
             * If the flag does not exist, the method returns false 
             * and throws a std::invalid_argument exception. This should 
             * be caught and handled by the apps implementation.
             * */
            bool optionIsEnabled(const std::string flagOrName);

            void addOptionHelpText(std::string flag, std::string helpText);

            void parseInput();

            void showHelp();

    };
}

// to do 
//      make option names optional
//      allow option arguments (to follow flags)