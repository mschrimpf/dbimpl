cmake_minimum_required(VERSION 3.1)
project(assignment1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES assignment1/main.cpp)
add_executable(assignment1 ${SOURCE_FILES})