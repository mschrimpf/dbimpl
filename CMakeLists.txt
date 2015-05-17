cmake_minimum_required(VERSION 3.1)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -g")

################################
# GTest
################################
ADD_SUBDIRECTORY (lib/gtest-1.7.0)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


###
#add_subdirectory(src/assignment1_externalsort)
#add_subdirectory(test/assignment1_externalsort)
add_subdirectory(src/database)
add_subdirectory(testing/buffer)
add_subdirectory(testing/slotted_pages)


