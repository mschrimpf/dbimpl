cmake_minimum_required(VERSION 3.1)
project(dbimpl)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    lib/data_generator/gen.cpp
    src/assignment1/DataGenerator.cpp
    src/assignment1/external_sort.cpp
    src/assignment1/external_sort.h
    src/assignment1/main.cpp)

add_executable(dbimpl ${SOURCE_FILES})