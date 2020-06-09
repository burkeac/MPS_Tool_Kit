// programOptions.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// June 5, 2020
#include <stdexcept>
#include "utilities.hpp"
#include <iostream>
#include <map>
#include <string>

namespace MPS {
// Constructor
    ProgramOptions::ProgramOptions(int argc, char* argv[]){
        _argc = argc;
        _argv = argv;
    }
    
// Private Members

    // checks to see if flag exists in map, returns false if not in map.
    bool ProgramOptions::_flagDoesExist(std::string flag){
        std::map<std::string, bool>::iterator flagLocation = _optionFlags.find(flag);
        if (flagLocation == _optionFlags.end() )
            return(false);
        return(true);
    }

    bool ProgramOptions::_nameDoesExist(std::string name){
        auto nameLocation = _optionNames.find(name);
        if (nameLocation == _optionNames.end() )
            return(false);
        return(true);
    }

    bool ProgramOptions::_helpTextDoesExist(std::string flag){
        auto textLocation = _optionHelpText.find(flag);
        if (textLocation == _optionHelpText.end() )
            return(false);
        return(true);
    }

// Public Members

    void ProgramOptions::addOption(const std::string flag, 
                                   const std::string name, 
                                   const int numArgs,
                                   const bool flaggable){
        _optionFlags.insert( std::pair<std::string, bool>( "-" + flag, false));
        if(flaggable)
            _optionNames.insert( std::pair<std::string, std::string>("--"+ name, "-" + flag));
        else
            _optionNames.insert( std::pair<std::string, std::string>(name, "-" + flag));
        _numOptArgs.insert( std::pair<std::string, int>("-" + flag, numArgs));
    }

    // returns the state of the option (true or false)
    bool ProgramOptions::optionIsEnabled(const std::string flag){

        // if flag does not exist, error
        if (_flagDoesExist(flag) == false){
            throw std::invalid_argument( "flag: '" + flag + "' does not exist" );
            return(false);
        } 

        // otherwise: return the value of the flag state.
        return(_optionFlags[flag]);
    }
    
    void ProgramOptions::addOptionHelpText(std::string flag, std::string helpText){
        if (_flagDoesExist("-" + flag) == false)
            throw std::invalid_argument( "flag: '" + flag + "' does not exist. Help text not assigned" );
        else {
            _optionHelpText.insert(std::pair<std::string, std::string>("-" + flag, helpText));
        }
    }

    void ProgramOptions::parseInput(){

        for(int i=1; i<_argc; i++){

            std::string toParse;

            // Note: string comparison can be used here because: map<string, string> 
            // vs map<string, bool>, where the _flagDoesExist method has to be 
            // used because it returns a bool.
            if(_nameDoesExist(_argv[i]) && _argv[i][0] == '-' && _argv[i][1] == '-')
                toParse = _optionNames[_argv[i]];
            else toParse = _argv[i];

            if(_flagDoesExist(toParse)){
                _optionFlags[toParse] = true;
            } else {
                // ensure it is not a negative number eg: -3. Then error.
                // otherwise the value is a param
                if( toParse[0] == '-' && std::isdigit(toParse[1]) == false){
                    throw std::invalid_argument
                        ( "flag: '" + toParse + "' does not exist" );
                } else {

                    /* param handling */
                    arguments.push_back(toParse);
                    
                }
            }
        }
    }

    void ProgramOptions::showHelp(){
        if(overRideProgramName == ""){
            // Get name of program and list the usage
            std::size_t lastSlash = ((std::string)_argv[0]).find_last_of("/");
            std::cout << "\n usage: " << ((std::string)_argv[0]).substr(lastSlash+1) << " ";
        } else std::cout << "\n usage: " << overRideProgramName + " ";

        for(auto parm : paramNames) 
            std::cout << parm << " ";
            std::cout << " [options]" << std::endl;

        std::cout << "\n options are:" << std::endl;
        auto flagIt = _optionFlags.begin();
        auto nameIt = _optionNames.begin();
        while(flagIt != _optionFlags.end()){
            std::string helpText = "";
            if (_helpTextDoesExist(flagIt->first))
                helpText = _optionHelpText[flagIt->first];

            std::cout << "   " << flagIt->first << ", " << nameIt->first 
            <<  "   " << helpText << std::endl;
            flagIt++;
            nameIt++;
        } 
        std::cout << std::endl;
    }

}