project(database)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -O3 -g") # TODO: remove -g flag

set(BUFFER_SOURCE_FILES
    buffer/BufferFrame.cpp buffer/BufferFrame.hpp
    buffer/BufferManager.cpp buffer/BufferManager.hpp
    buffer/IPageIO.h
    buffer/IReplacementStrategy.h
    util/Math.cpp util/Math.h
    util/TwoQList.h util/TwoQList.cpp
    util/debug.h
    util/debug.cpp
    util/PageIOUtil.h util/PageIOUtil.cpp)
set(SLOTTED_PAGES_SOURCE_FILES
    slotted_pages/TID.hpp
    slotted_pages/SlottedPage.hpp
    slotted_pages/SlottedPage.cpp
    slotted_pages/SPSegment.hpp
    slotted_pages/SPSegment.cpp
    slotted_pages/Record.hpp)
set(RELATIONS_SOURCE_FILES
    relations/Schema.hpp
    relations/Types.hpp
    relations/parser/Parser.cpp
    relations/parser/Schema.cpp)
add_library(database ${BUFFER_SOURCE_FILES} ${SLOTTED_PAGES_SOURCE_FILES} ${RELATIONS_SOURCE_FILES} relations/SchemaSerializer.cpp relations/SchemaSerializer.h)
