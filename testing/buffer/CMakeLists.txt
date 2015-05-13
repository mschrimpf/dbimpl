project(buffer_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")

add_executable(buffertest ${BUFFER_SOURCE_FILES} buffertest.cpp)
add_executable(test_buffermanager ${BUFFER_SOURCE_FILES} test_buffermanager.cpp)