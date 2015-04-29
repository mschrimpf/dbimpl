project(database)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -lto")

set(SOURCE_FILES util/TwoQList.cpp util/TwoQList.h testing/TwoQTest.cpp buffer/BufferFrame.cpp buffer/BufferFrame.hpp buffer/BufferManager.cpp buffer/BufferManager.hpp testing/BufferManagerTest.cpp buffer/IPageIO.h util/Math.cpp util/Math.h)
add_executable(database ${SOURCE_FILES} buffer/IReplacementStrategy.h util/FileIOUtil.cpp util/FileIOUtil.h)