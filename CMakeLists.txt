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
            ./src/TransferFunctions/HDR_TranFunc.cpp
            ./src/TransferFunctions/HDR_TranFunc.hpp
            ./src/Color/ColorSpaces.cpp
            ./src/Color/ColorSpaces.hpp)

# Find OpenCV package and Link it to the MPSToolKit Libary
target_link_libraries(MPSToolKit ${OpenCV_LIBRARIES})

# Compile the executables
add_executable(EOTF ./apps/Calculate_PQ_EOTF.cpp)
add_executable(GetPrimaries ./apps/GetPrimaries.cpp)
add_executable(ToICtCp ./apps/ToICtCp.cpp)

# Show Cmake where to find the header files
include_directories(./src/TransferFunctions
                    ./src/Color)

# Links the MPS toolkit library to Apps
target_link_libraries(EOTF MPSToolKit)
target_link_libraries(GetPrimaries MPSToolKit)
target_link_libraries(ToICtCp MPSToolKit)