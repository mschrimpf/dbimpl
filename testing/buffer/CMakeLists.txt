project(buffer_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")

add_executable(buffertest buffertest.cpp)
set_target_properties(buffertest PROPERTIES RUNTIME_OUTPUT_DIRECTORY .)
target_link_libraries(buffertest database)

add_executable(test_buffermanager test_buffermanager.cpp)
set_target_properties(test_buffermanager PROPERTIES RUNTIME_OUTPUT_DIRECTORY .)
target_link_libraries(test_buffermanager database)
