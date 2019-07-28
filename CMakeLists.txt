cmake_minimum_required(VERSION 3.1)

project(MPS_TOOLS VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/apps)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)

add_library(MPSToolKit SHARED
            ./src/TransferFunctions/HDR_TranFunc.cpp
            ./src/TransferFunctions/HDR_TranFunc.hpp)

add_executable(EETF
                ./apps/Calculate_PQ_EETF.cpp)

include_directories(./src/TransferFunctions)

target_link_libraries(EETF MPSToolKit)