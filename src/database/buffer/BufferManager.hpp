#ifndef PROJECT_BUFFER_MANAGER_H
#define PROJECT_BUFFER_MANAGER_H

#include <stdint.h>
#include "BufferFrame.hpp"

class BufferManager{
public:
    BufferManager::BufferManager(uint64_t size);
    BufferFrame& BufferManager::fixPage(uint64_t pageId, bool exclusive);
    void BufferManager::unfixPage(BufferFrame& frame, bool isDirty);
    void* BufferFrame::getData();
    BufferManager::~BufferManager();
};



#endif //PROJECT_BUFFER_MANAGER_H
