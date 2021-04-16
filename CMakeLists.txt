cmake_minimum_required(VERSION 3.1)

project(MPS_TOOLS VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# use -DUSEOPENCV=ON to use OpenCV
option(USE_OPENCV "Build components that depend on OpenCV" OFF)
option(BUILD_PYTHONBINDINGS "Build Python Bindings" ON)

# Set where binaries are created
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/apps)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)

# Show Cmake where to find the header files
include_directories(./libSrc/TransferFunctions
                    ./libSrc/Color
                    ./libSrc/Utilities
                    ./libSrc/Core
                    ./libSrc/
                    ./ThirdParty)

# SRC file list
set( SRC_Files ./libSrc/TransferFunctions/HDR_TranFunc.cpp
    ./libSrc/Color/ColorSpaces.cpp
    ./libSrc/Color/deltaE.cpp
    ./libSrc/Color/LUT.cpp
    ./ThirdParty/CIEDE2000/CIEDE2000.cpp
    ./libSrc/Utilities/utilities.cpp
    ./libSrc/Utilities/programOptions.cpp
    ./libSrc/Utilities/csvReader.cpp
)

# Create the C++ library
add_library(MPSToolkit SHARED ${SRC_Files})

#LIink CMake if option is set
if(USE_OPENCV)
    # Find OpenCV
    find_package(opencv REQUIRED)

    # Find OpenCV package and Link it to the MPSToolKit Libary
    target_link_libraries(MPSToolkit ${OpenCV_LIBRARIES})
endif()

# Add Pybind11 and create MPS python module 
add_subdirectory(ThirdParty/pybind11)
pybind11_add_module(py_mpsToolkit  ./libSrc/PythonBindings/PythonBindings.cpp ${SRC_Files}) 
# target_link_libraries(py_mpsToolkit PUBLIC MPSToolkit) Would statically link the files

# Compile the apps executables
add_executable(EOTF ./appSrc/Calculate_PQ_EOTF.cpp)
add_executable(GetPrimaries ./appSrc/GetPrimaries.cpp)
add_executable(ToICtCp ./appSrc/ToICtCp.cpp)
add_executable(LUT ./appSrc/LUT_App.cpp)
add_executable(TEST ./appSrc/testProgram.cpp)

# Links the MPS toolkit library to Apps
target_link_libraries(EOTF MPSToolkit)
target_link_libraries(GetPrimaries MPSToolkit)
target_link_libraries(ToICtCp MPSToolkit)
target_link_libraries(LUT MPSToolkit)
target_link_libraries(TEST MPSToolkit)

# copy header files into include directory
file(COPY 
    ./libSrc/mpsVersion.hpp
    DESTINATION include
)

file(COPY 
    ./libSrc/Color/ColorModule.hpp
    ./libSrc/Color/ColorSpaces.hpp
    ./libSrc/Color/deltaE.hpp
    ./libSrc/Color/LUT.hpp
    DESTINATION include/Color
)
    
file(COPY 
    ./libSrc/TransferFunctions/HDR_TranFunc.hpp
    DESTINATION include/TransferFunctions
)
    
file(COPY
    ./libSrc/Utilities/utilities.hpp
    DESTINATION include/Utilities
)

file(COPY 
    ./libSrc/Core/macro_typedef.hpp
    DESTINATION include/Core
)
