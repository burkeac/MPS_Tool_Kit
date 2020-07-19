cmake_minimum_required(VERSION 3.1)

project(MPS_TOOLS VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set where binaries are created
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/apps)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)

# Find OpenCV
find_package(opencv REQUIRED)

# Create the library
add_library(MPSToolKit SHARED
            ./libSrc/Core/macro_typedef.hpp
            ./libSrc/TransferFunctions/HDR_TranFunc.cpp
            ./libSrc/TransferFunctions/HDR_TranFunc.hpp
            ./libSrc/Color/ColorSpaces.cpp
            ./libSrc/Color/ColorSpaces.hpp
            ./libSrc/Color/deltaE.cpp
            ./libSrc/Color/deltaE.hpp
            ./libSrc/Utilities/utilities.cpp
            ./libSrc/Utilities/utilities.hpp
            ./libSrc/Utilities/programOptions.cpp
            ./libSrc/Utilities/csvReader.cpp
)

# Find OpenCV package and Link it to the MPSToolKit Libary
target_link_libraries(MPSToolKit ${OpenCV_LIBRARIES})

# Compile the executables
add_executable(EOTF ./appSrc/Calculate_PQ_EOTF.cpp)
add_executable(GetPrimaries ./appSrc/GetPrimaries.cpp)
add_executable(ToICtCp ./appSrc/ToICtCp.cpp)
add_executable(TEST ./appSrc/testProgram.cpp)

# Show Cmake where to find the header files
include_directories(./libSrc/TransferFunctions
                    ./libSrc/Color
                    ./libSrc/Utilities
                    ./libSrc/Core)

# Links the MPS toolkit library to Apps
target_link_libraries(EOTF MPSToolKit)
target_link_libraries(GetPrimaries MPSToolKit)
target_link_libraries(ToICtCp MPSToolKit)
target_link_libraries(TEST MPSToolKit)