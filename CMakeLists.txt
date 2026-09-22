cmake_minimum_required(VERSION 4.3)
project(std_reimplimentation)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(vector)

add_executable(std_reimplimentation
        vector/vector.hpp
        README.md
        main.cpp)
