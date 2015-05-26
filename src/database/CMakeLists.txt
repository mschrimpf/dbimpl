project(database)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread") # TODO: O3

set(BUFFER_SOURCE_FILES
    buffer/BufferFrame.cpp buffer/BufferFrame.hpp
    buffer/BufferManager.cpp buffer/BufferManager.hpp
    buffer/IReplacementStrategy.h
    buffer/TwoQList.h buffer/TwoQList.cpp
    buffer/IPageIO.h
    buffer/PageIOUtil.h buffer/PageIOUtil.cpp
    util/Math.cpp util/Math.h
    util/debug.h
    util/debug.cpp)
set(SLOTTED_PAGES_SOURCE_FILES
    slotted_pages/TID.hpp
    slotted_pages/Slot.hpp
    slotted_pages/Slot.cpp
    slotted_pages/SlottedPage.hpp
    slotted_pages/SlottedPage.cpp
    slotted_pages/SPSegment.hpp
    slotted_pages/SPSegment.cpp
    slotted_pages/Record.hpp)
set(RELATIONS_SOURCE_FILES
    relations/Schema.hpp
    relations/SchemaSerializer.h
    relations/SchemaSerializer.cpp
    relations/Types.hpp
    relations/parser/Parser.cpp
    relations/parser/Schema.cpp)
set(BTREE_SOURCE_FILES
    btree/BTree.hpp
    btree/BTree.cpp
    btree/BTreeLeaf.cpp
    btree/BTreeVisualize.cpp)

add_library(database
    ${BUFFER_SOURCE_FILES}
    ${SLOTTED_PAGES_SOURCE_FILES}
    ${RELATIONS_SOURCE_FILES}
    ${BTREE_SOURCE_FILES}
    )
