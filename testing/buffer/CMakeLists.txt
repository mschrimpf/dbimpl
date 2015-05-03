project(buffer_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -lto")

set(SOURCE_FILES
    buffertest.cpp
    ../../src/database/buffer/BufferFrame.cpp ../../src/database/buffer/BufferFrame.hpp
    ../../src/database/buffer/BufferManager.cpp ../../src/database/buffer/BufferManager.hpp
    ../../src/database/buffer/IPageIO.h
    ../../src/database/buffer/IReplacementStrategy.h
    ../../src/database/util/Math.cpp ../../src/database/util/Math.h
    ../../src/database/util/TwoQList.h ../../src/database/util/TwoQList.cpp
    ../../src/database/util/spinlock.h
    ../../src/database/util/PageIOUtil.h ../../src/database/util/PageIOUtil.cpp)
add_executable(buffer_test ${SOURCE_FILES})