cmake_minimum_required(VERSION 3.1)

project(MPS_TOOLS VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/apps)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)

add_library(MPSToolKit SHARED
            ./src/TransferFunctions/HDR_TranFunc.cpp
            ./src/TransferFunctions/HDR_TranFunc.hpp
            ./src/Color/ColorSpaces.cpp
            ./src/Color/ColorSpaces.hpp)

add_executable(EETF ./apps/Calculate_PQ_EETF.cpp)
add_executable(GetPrimaries ./apps/GetPrimaries.cpp)

include_directories(./src/TransferFunctions
                    ./src/Color)

target_link_libraries(EETF MPSToolKit)
target_link_libraries(GetPrimaries MPSToolKit)