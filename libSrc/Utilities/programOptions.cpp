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
ProgramOptions::OptionObj* ProgramOptions::_findOption(const std::string& flag) {
        
        if( flag.substr(0,2) == "--"){

            for(int i=0; i < _options.size(); i++){
                if( flag.substr(2, flag.length()).compare(_options[i]._longFlag) == 0 ){
                    return(&_options[i]);
                } else if(i == (_options.size()-1)){
                    throw std::invalid_argument( "flag: '" + flag + "' does not exist" );
                    break;
                }
            }
        } else {
            for(int i=0; i < _options.size(); i++){
                if(flag.substr(1, flag.length()).compare(_options[i]._shortFlag) == 0){
                    return(&_options[i]);
                }else if(i == _options.size()-1){
                    throw std::invalid_argument( "flag: '" + flag + "' does not exist" );
                    break;
                }
            }
        }
        return nullptr;
    }

    void ProgramOptions::_enableOption(const std::string& flag){
        _findOption(flag)->selected = true;
    }

// Public Members
    void ProgramOptions::addOption(const std::string shortFlag,
                                   const std::string longFlag){
        if(shortFlag.length() > 1) 
            std::cerr << "Development warning: 'const std::string shortFlag' should not exceed 1 charecter" << std::endl;
        _options.push_back( ProgramOptions::OptionObj(shortFlag, longFlag) );
    }

    void ProgramOptions::addOption_SF(const std::string shortFlag){
        _options.push_back( ProgramOptions::OptionObj(shortFlag, true) );
    } 
    
    void ProgramOptions::addOption_LF(const std::string longFlag){
        _options.push_back( ProgramOptions::OptionObj(longFlag, false) );
    }

    // returns the state of the option (true or false)
    bool ProgramOptions::optionIsEnabled(const std::string flag){
        std::string pole = (flag.length() > 1) ? "--" : "-";
        return(_findOption(pole+flag)->selected);
    }
    
    void ProgramOptions::addOptionHelpText(std::string flag, std::string helpText){
       std::string pole = (flag.length() > 1) ? "--" : "-";
       _findOption(pole+flag)->_helpText = helpText;
    }

    void ProgramOptions::numOptionParams(std::string flag, int num){
        std::string pole = (flag.length() > 1) ? "--" : "-";
       _findOption(pole+flag)->numParams = num;
    }

    std::vector<std::string>& ProgramOptions::getOptionArgs(std::string flag){
        std::string pole = (flag.length() > 1) ? "--" : "-";
        return(_findOption(pole+flag)->optionArgs);
    }

    void ProgramOptions::parseInput(){

        for(int i=1; i<_argc; i++){

            if( std::isdigit(_argv[i][1]) == false && 
               (_argv[i][0] == '-' || std::string(_argv[i]).substr(0,1) == "--") ){
                    _enableOption(_argv[i]); 
                
                auto crntOptn = _findOption(_argv[i]);
                if(crntOptn->numParams != 0){

                    for(int j=i+1; j< (i + crntOptn->numParams + 1); j++){

                        if(j == _argc){ // ensure not out of bounds
                            throw std::invalid_argument("not enough params");
                            break;
                        }

                        crntOptn->optionArgs.push_back(_argv[j]);
                    }
                    i += crntOptn->numParams;
                }

            } else arguments.push_back(_argv[i]);
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
            std::cout << " [options] \n";

        std::cout << "\n options are:" << std::endl;
        for(auto option : _options){
            if(option._shortFlag == ""){
                std::cout << "         --" << option._longFlag << "   " 
                        << option._helpText << "\n";
            } else if(option._longFlag == ""){
                std::cout << "     -" << option._shortFlag << "   " 
                        << option._helpText << "\n";
            }else{
                std::cout << "     -" << option._shortFlag << ", " 
                        << "--" << option._longFlag << "   " 
                        << option._helpText << "\n";
            }
        }
        std::cout << std::endl;
    }

    bool ProgramOptions::operator[](const std::string& flag){
        std::string pole = (flag.length() > 1) ? "--" : "-";
        return(_findOption(pole+flag)->selected);
    }
}