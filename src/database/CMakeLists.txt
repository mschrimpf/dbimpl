project(database)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES buffer/BufferFrame.cpp buffer/BufferFrame.hpp buffer/BufferManager.cpp buffer/BufferManager.hpp testing/BufferManagerTest.cpp util/FileUtil.cpp util/FileUtil.h util/Math.cpp util/Math.h)
add_executable(database ${SOURCE_FILES} util/LruCache.h testing/LruCacheTest.cpp util/ThreeQList.cpp util/ThreeQList.h)