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
            std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
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
                        : _helpText(""), 
                        numParams(0), 
                        selected(false),
                        _shortFlag(shortFlag),
                        _longFlag(longFlag)
            {}

            OptionObj(const std::string flag, const bool isShort)
                : _helpText(""), 
                numParams(0), 
                selected(false), 
                _shortFlag(""),
                _longFlag("")
            {
                if(isShort)
                    _shortFlag = flag;
                else 
                    _longFlag = flag;
            }
        };

        std::vector<OptionObj> _options;
        int _argc;
        char** _argv;
        
        // methods
        OptionObj* _findOption(const std::string& flag);
        void _enableOption(const std::string& flag);

        public:

        /// Constructor for program options.
        /** 
         * accepts int argc and char* argv[] from int main(int argc, char* argv[])
         * */
        ProgramOptions(int argc, char* argv[]);
        
        /// vector containing the arguments after parsing.
        std::vector<std::string> arguments;

        /// Paramater names to be shown in the help page's "usage"
        std::vector<std::string> paramNames = {"<arguments>"};

        /// The name of the program to be shown in the help pages "usage"
        /**
         *  A blank string signals the help method to use executables name, parsed using char* argv[] 
         * */
        std::string overRideProgramName = "";

        /// creates options and generates associated flags
        /** 
         * Short flags should be called on the CLI with a preceeding "-" \n
         * Long flgas should be called on the CLI with a preceeding "--" \n
         * these prefixes are added automatically! \n\n
         * 
         * Example code: options.addOption("h", "help"); \n
         * On Commandline: -h or --help
         * */
        void addOption(const std::string shortFlag, 
                        const std::string longflag);
        
        void addOption_SF(const std::string shortFlag);

        void addOption_LF(const std::string longflag);

        /// Returns the state of the object (true or false)
        /** 
         * If the flag does not exist, the method returns false 
         * and throws a std::invalid_argument exception. This should 
         * be caught and handled by the app implementation.
         * */
        bool optionIsEnabled(const std::string flagOrName);

        /// Adds help text for options.
        /** 
         * The flag paramater can be either the short or long flag. Without the prefix. \n
         * For best formatting, help text should be short and avoid new lines.
         * */
        void addOptionHelpText(std::string flag, std::string helpText);

        /// Sets the number of optional arguments for an option flag.
        /**
         * Option flags can have associated paramaters. These are stored as part of the option object
         * and differ from the "arguments" member in the ProgramOptions object.\n\n
         * 
         * Example: ./program X -o Y Z \n
         * Assuming -o accepts 2 paramaters, Y and Z are stored within the -o options argument method.
         * X on the otherhand, belongs to the globabl ProgramOptions member.
         * \n option-arguments can be accessed using the getOptionParams(std::string flag) method.
         * */
        void numOptionParams(std::string flag, int num);

        /// Acess params associated with an option flag
        /** 
         * returns a vector reference containg the associated arguments 
         * with that flag similar to the global argument flag.
         * */
        std::vector<std::string>& getOptionArgs(std::string flag);

        /// Parses CLI argument stream.
        /**
         * This method should be called at the end of the decliration of all options.
         * */
        void parseInput();

        /// Outputs help screen through stdout (std::cout).
        /** outlines usage and options */
        void showHelp();
    };

    class CSVreader{
        private:
        std::vector<std::vector<std::string>> data;
        
        public: 
        CSVreader();
        CSVreader(const std::string& filePath, bool strip = false);
        
        std::string filePath;

        /// Reads data to internal data structure
        /** 
         * If file can not be read, the method throws a std::string exception.
         * */
        std::vector<std::vector<std::string>>& read(bool strip = true);
        
        /// returns a reference to the data.
        std::vector<std::vector<std::string>>& getRef();

        // Access data by row and col
        std::string& at(size_t row, size_t col);

        /// Get a specified row of data.
        std::vector<std::string>& getRow(size_t row);

        /// Get a specified row of data
        /** 
         * Can access specific col element in a row by appending an additional index [n]. \n 
         * such as: CSVreader[row][col]
         * */ 
        std::vector<std::string>& operator[](size_t row);

    };

    /// Removes leading and trailing spaces from string 
    std::string strip(const std::string& input);


}