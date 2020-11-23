// LUT_App.cpp
// Adam C. Burke (adam.burke603@gmail.com)
// October 13, 2020

#include <iostream>
#include <cmath>

#include "utilities.hpp"
#include "LUT.hpp"


using namespace std;

bool SetupOptions(MPS::ProgramOptions& Options){

    Options.paramNames = {"<Output File path> [options]", "\n        LUT <Rin> <Gin> <Bin>"};

    Options.addOption("h", "help");
    Options.addOptionHelpText("h", "| Show's this prompt.");

    Options.addOption("L", "input_LUT")
           .addOptionHelpText("Specified the input LUT")
           .numOptionParams(1);

    Options.addOption("n", "generate_null")
           .addOptionHelpText("<num nodes> | Generates a 'null' LUT where nodes_in == nodes_out")
           .numOptionParams(1);

    Options.addOption("s", "scale")
            .addOptionHelpText("| Sets the bitdepth of the codvalues. Default is float 0-1")
            .numOptionParams(1);

    Options.addOption_LF("CSV_headers")
           .addOptionHelpText("| Applies 'headers' to columns in output file.");
    
    Options.addOption_LF("CSV_bitdepth_in")
           .addOptionHelpText("| Specifies the bitdepth to scale to.")
           .numOptionParams(1);
    
    Options.addOption_LF("CSV_bitdepth_out")
           .addOptionHelpText("| Specifies the bitdepth to scale from.")
           .numOptionParams(1);

    Options.addOption("L", "input_LUT")
           .addOptionHelpText("<LUT Path> | LUT file to be read.")
           .numOptionParams(1);
    
    Options.addOption_LF("Trilinear_Interp")
           .addOptionHelpText("Sets the interploation method to Trilinear. (default method)");

    Options.addOption_LF("Tetrahedral_Interp")
           .addOptionHelpText("Sets the interploation method to Tetrahedral.");

    try{
        Options.parseInput();
    }catch(const std::exception& e){
        std::cerr << e.what() << '\n';
        return true;
    }

    return false;
}

int main(int argc, char* argv[]){

    //Setup and parse CLI
    MPS::ProgramOptions Options(argc, argv);
    if(SetupOptions(Options))
        return EXIT_FAILURE;

    if(Options.optionIsEnabled("h") || argc<2){
        Options.showHelp();
        return EXIT_SUCCESS;
    }

    if(Options.arguments.size() < 1){
        Options.showHelp();
        std::cerr << "At least one positional argument must be supplied" << std::endl;
        return EXIT_FAILURE;
    }


    // Either create or read LUT from file
    MPS::LUT3D* LUT = NULL;

    try{
    // Generate NULL lut
    if(Options["generate_null"] && Options["input_LUT"]){
        cerr << "--generate_null (-n) and --input_LUT (-L) cannot be specified at the same time! Aborting" << std::endl;
        return EXIT_FAILURE;
    }

    else if(Options["generate_null"]){
        int nodes = stoi(Options.getOptionArgs("n")[0]);
        LUT = new MPS::LUT3D(nodes);
        LUT->Generate_3DLUT_Nodes();
    }
    
    // else read lut
    else if(Options["input_LUT"]){
        std::string extension = MPS::ExtensionFromPath(Options.getOptionArgs("L")[0]);
        int bitdepth = Options["CSV_bitdepth_in"] ? stoi(Options.getOptionArgs("CSV_bitdepth_in")[0]) : 0;

        LUT = new MPS::LUT3D;
        if(extension == "csv"){
            LUT->ReadFromCSV(Options.getOptionArgs("L")[0], bitdepth);
        } else if(extension == "cube"){
            LUT->ReadFromCubeFile(Options.getOptionArgs("L")[0]);
        }else{
            cerr << "File type not supported: " << extension << endl;
        }
    }
    
    // else generate default null LUT
    else{
        LUT = new MPS::LUT3D(17);
        LUT->Generate_3DLUT_Nodes();
    }

    }catch(...){
        cerr << "Can't read input LUT file or generate LUT." << endl;
        delete LUT;
        return EXIT_FAILURE;
    }


    // is the argument a file path?
    bool isAllNum = true;
    for(char C : Options.arguments[0]){
        if( !isdigit(C) )
            isAllNum = false;
    }

    // output LUT if not interpolating
    if( !isAllNum ){

        try{
            
            string extension = MPS::ExtensionFromPath(Options.arguments[0]);
            
            if(extension == "csv"){
                LUT->Write2CSV(Options.arguments[0], Options["CSV_headers"]);
            }

            else if(extension == "cube"){
                LUT->Write2CUBE_Adobe(Options.arguments[0]);
            }

        }catch(...){
            cerr << "Can't write LUT file." << endl;
            delete LUT;
            return EXIT_FAILURE;
        }
    }
    else{
        
        if(Options.arguments.size() < 3){
            std::cout << "Interpolaton requries 3 input code values! Only " << 
            Options.arguments.size() << " were provided."  << std::endl;
            return EXIT_FAILURE;
        }

        float Rin, Gin, Bin;

        try{
            Rin = stof(Options.arguments[0]);
            Gin = stof(Options.arguments[1]);
            Bin = stof(Options.arguments[2]);
        } catch(...){
            std::cerr << "Cannot convert " << Rin << " " << Gin << " " << Bin << " " << "  to RGB code values!" << std::endl;
            delete LUT;
            return EXIT_FAILURE;
        }

        float scale = 1;
        if(Options["scale"]){
            try{
                scale = std::pow(2, stof(Options.getOptionArgs("scale")[0])) - 1;
            }catch(...){
                std::cerr << "Cannot convert scale factor to numric value!" << std::endl;
                delete LUT;
                return EXIT_FAILURE;
            }
        }


        if(Options["Tetrahedral_Interp"] && Options["Trilinear_Interp"]){
            std::cerr << "flags --Tetrahedral_Interp and --Trilinear_Interp cannot be used simultaneously!" << std::endl;
        }

        MPS::tripletF interpResult;
        if(Options["Tetrahedral_Interp"]){
            interpResult = LUT->Interpolate_Tetra (Rin / scale, Gin / scale, Bin / scale); 
        }else{
            interpResult = LUT->Interpolate_Trilin(Rin / scale, Gin / scale, Bin / scale);
        }  

        cout << interpResult[0] * scale << " " << interpResult[1] * scale << " " << interpResult[2] * scale << std::endl;
    }

    delete LUT;
    return EXIT_SUCCESS;
}