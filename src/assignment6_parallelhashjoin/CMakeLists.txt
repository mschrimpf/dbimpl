project(parallelhashjoin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") # TODO: -O3 -lto

set(PARALLELHASHJOIN_SOURCE_FILES
    hashjoinskeleton.cpp
    ChainingLockingHT.hpp
    ChainingHT.hpp
    LinearProbingHT.hpp)

add_executable(parallelhashjoin ${SOURCE_FILES})
