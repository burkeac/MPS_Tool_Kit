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

            //sub object used to contain the options
            struct OptionObj {
                std::string _shortFlag, _longFlag, _helpText;
                int numParams;
                std::vector<std::string> optionArgs;
                bool selected;

                OptionObj(const std::string shortFlag, 
                          const std::string longFlag)
                          : _helpText(""), numParams(0), selected(false){
                    _shortFlag = shortFlag;
                    _longFlag = longFlag;
                }
            };

            // private members
            std::vector<OptionObj> _options;
            int _argc;
            char** _argv;
            
            // methods
            OptionObj* _findOption(const std::string& flag);
            void _enableOption(const std::string& flag);

        public:
        // Constructor
            ProgramOptions(int argc, char* argv[]);

        // Public Members
            std::vector<std::string> arguments;
            std::vector<std::string> paramNames = {"<arguments>"};
            std::string overRideProgramName = "";

        // Methdos

            /// Adds and option flag
            /** 
             * 
             * */
            void addOption(const std::string flag, 
                           const std::string longflag);

            /// Returns the state of the object (true or false)
            /** 
             * If the flag does not exist, the method returns false 
             * and throws a std::invalid_argument exception. This should 
             * be caught and handled by the apps implementation.
             * */
            bool optionIsEnabled(const std::string flagOrName);

            void addOptionHelpText(std::string flag, std::string helpText);

            void numOptionParams(std::string flag, int num);
            
            std::vector<std::string>& getOptionParams(std::string flag);

            void parseInput();

            void showHelp();


    };
}

// to do 
//      make option names optional
//      allow option arguments (to follow flags)
// / make -1 assign the rest of the params to this option