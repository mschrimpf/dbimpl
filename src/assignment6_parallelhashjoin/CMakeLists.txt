project(parallelhashjoin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -flto")

set(PARALLELHASHJOIN_SOURCE_FILES
    hash.hpp
    ChainingLockingHT.cpp
    ChainingLockingCachelineHT.cpp
    LinearProbingHT.cpp
    ChainingHT.cpp
    ChainingBloomHT.cpp
    MatrixHT.cpp)

add_executable(parallelhashjoin ${PARALLELHASHJOIN_SOURCE_FILES})
