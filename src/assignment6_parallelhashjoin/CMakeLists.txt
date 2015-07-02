project(parallelhashjoin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") # TODO: -O3 -lto

set(PARALLELHASHJOIN_SOURCE_FILES
#    ChainingLockingHT.cpp
    ChainingHT.cpp
    LinearProbingHT.cpp
    hash.hpp)

add_executable(parallelhashjoin ${PARALLELHASHJOIN_SOURCE_FILES})
