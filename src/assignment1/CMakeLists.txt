project(assignment1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp external_sort.h external_sort.cpp sorted_check.h sorted_check.cpp)
add_executable(assignment1 ${SOURCE_FILES})