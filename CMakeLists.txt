cmake_minimum_required(VERSION 3.1)


################################
# GTest
################################
#ADD_SUBDIRECTORY (lib/gtest-1.7.0)
#enable_testing()
#include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


### Assignments
add_subdirectory(src/assignment1_externalsort)
#add_subdirectory(test/assignment1_externalsort)
add_subdirectory(src/database)
add_subdirectory(testing/buffer)
