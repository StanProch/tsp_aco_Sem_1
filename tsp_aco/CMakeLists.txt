cmake_minimum_required(VERSION 3.29)
project(tsp_aco)

set(CMAKE_CXX_STANDARD 20)

add_executable(tsp_aco main.cpp
        functions.h
        structures.h
        functions.cpp)
