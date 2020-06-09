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
    options.addOption("j", "letterj", 0, false);
    options.addOption("k", "letterk", 0);
    options.addOption("l", "letterl", 0);

    options.addOptionHelpText("j", "this is the letter j");

    try{
        options.parseInput();
    }catch(...){
        std::cout << "an error occured" << std::endl;
    }
    std::cout << "-j " << options.optionIsEnabled("-j") << std::endl;
    std::cout << "-k " << options.optionIsEnabled("-k") << std::endl;
    std::cout << "-l " << options.optionIsEnabled("-l") << std::endl;

    options.showHelp();

    // MPS::colorPrimaries Rec709_Primaries(MPS::Rec709);
    // MPS::phosphorMatrix PM(Rec709_Primaries);

    // Eigen::Matrix3f inverseMatrix = PM.getInvPM();

    // cout << PM.getPM() << endl;
    // cout << inverseMatrix << endl;
    
    return EXIT_SUCCESS;
}