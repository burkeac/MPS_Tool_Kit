// GetPrimaries.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// July 28, 2019

#include <iostream>
#include <stdexcept>
#include "ColorSpaces.hpp"
#include "deltaE.hpp"
#include "utilities.hpp"

using namespace std;


int main(int argc, char* argv[]){

    MPS::ProgramOptions options(argc, argv);
    options.addOption_SF("j");
    options.addOption_LF("letterk");
    options.addOption("l", "letterl");
    options.addOption("h", "help");

    options.addOptionHelpText("j", "this is some help text");
    options.numOptionParams("j", 2);

    try{
        options.parseInput();
    }catch(std::invalid_argument err){
        std::cout << "an error occured: "<< err.what() << std::endl;
    }
    std::cout << "-j " << options.optionIsEnabled("j") << std::endl;
    std::cout << "-k " << options.optionIsEnabled("letterk") << std::endl;
    std::cout << "-l " << options.optionIsEnabled("l") << std::endl;
    
    if(options.optionIsEnabled("h"))
        options.showHelp();

    for(auto arg : options.getOptionArgs("j")){
        std::cout << arg << std::endl;
    }


    // MPS::colorPrimaries Rec709_Primaries(MPS::Rec709);
    // MPS::phosphorMatrix PM(Rec709_Primaries);

    // Eigen::Matrix3f inverseMatrix = PM.getInvPM();

    // cout << PM.getPM() << endl;
    // cout << inverseMatrix << endl;
    
    return EXIT_SUCCESS;
}
