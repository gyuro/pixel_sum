cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(
    PixelSum
    VERSION 1.0
    LANGUAGES CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(PixelSumLib PixelSum.cpp PixelSum.h TestUtility.h TimeUtility.h Common.h)

add_executable(PixelSumTest PixelSumTest.cpp)

target_link_libraries(PixelSumTest PRIVATE PixelSumLib)

add_test(NAME PixelSumTest COMMAND PixelSumTest)
