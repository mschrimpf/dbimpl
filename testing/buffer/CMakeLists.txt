project(buffer_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0 -pthread") #TODO O3

set(SOURCE_FILES
    ../../src/database/buffer/BufferFrame.cpp ../../src/database/buffer/BufferFrame.hpp
    ../../src/database/buffer/BufferManager.cpp ../../src/database/buffer/BufferManager.hpp
    ../../src/database/buffer/IPageIO.h
    ../../src/database/buffer/IReplacementStrategy.h
    ../../src/database/util/Math.cpp ../../src/database/util/Math.h
    ../../src/database/util/TwoQList.h ../../src/database/util/TwoQList.cpp
    ../../src/database/util/debug.h
    ../../src/database/util/debug.cpp
    ../../src/database/util/PageIOUtil.h ../../src/database/util/PageIOUtil.cpp)
add_executable(buffertest buffertest.cpp ${SOURCE_FILES})
add_executable(test_buffermanager test_buffermanager.cpp ${SOURCE_FILES})