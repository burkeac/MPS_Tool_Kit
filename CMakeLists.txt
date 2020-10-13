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
add_library(MPSToolkit SHARED
            ./libSrc/TransferFunctions/HDR_TranFunc.cpp
            ./libSrc/Color/ColorSpaces.cpp
            ./libSrc/Color/deltaE.cpp
            ./ThirdParty/CIEDE2000/CIEDE2000.cpp
            ./libSrc/Utilities/utilities.cpp
            ./libSrc/Utilities/programOptions.cpp
            ./libSrc/Utilities/csvReader.cpp
)

# Find OpenCV package and Link it to the MPSToolKit Libary
target_link_libraries(MPSToolkit ${OpenCV_LIBRARIES})

# Add Pybind11 and create MPS python module 
add_subdirectory(ThirdParty/pybind11)
pybind11_add_module(py_mpsToolkit  ./libSrc/PythonBindings/PythonBindings.cpp)
target_link_libraries(py_mpsToolkit PUBLIC MPSToolkit)

# Compile the apps executables
add_executable(EOTF ./appSrc/Calculate_PQ_EOTF.cpp)
add_executable(GetPrimaries ./appSrc/GetPrimaries.cpp)
add_executable(ToICtCp ./appSrc/ToICtCp.cpp)
add_executable(TEST ./appSrc/testProgram.cpp)

# Show Cmake where to find the header files
include_directories(./libSrc/TransferFunctions
                    ./libSrc/Color
                    ./libSrc/Utilities
                    ./libSrc/Core
                    ./libSrc/
                    ./ThirdParty)

# Links the MPS toolkit library to Apps
target_link_libraries(EOTF MPSToolkit)
target_link_libraries(GetPrimaries MPSToolkit)
target_link_libraries(ToICtCp MPSToolkit)
target_link_libraries(TEST MPSToolkit)