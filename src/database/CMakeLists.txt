project(database)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES buffer/BufferFrame.cpp buffer/BufferFrame.hpp buffer/BufferManager.cpp buffer/BufferManager.hpp)
add_executable(sort ${SOURCE_FILES})