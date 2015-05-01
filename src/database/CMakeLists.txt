project(database)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -lto")

set(SOURCE_FILES
    buffer/BufferFrame.cpp buffer/BufferFrame.hpp
    buffer/BufferManager.cpp buffer/BufferManager.hpp
    buffer/IPageIO.h
    buffer/IReplacementStrategy.h
    util/Math.cpp util/Math.h
    util/TwoQList.h util/TwoQList.cpp
    util/PageIOUtil.h util/PageIOUtil.cpp)
add_executable(database ${SOURCE_FILES} util/spinlock.h)