#ifndef PROJECT_BUFFER_MANAGER_H
#define PROJECT_BUFFER_MANAGER_H

#include <stdint.h>
#include "BufferFrame.hpp"

class BufferManager{
public:
    BufferManager(uint64_t size);
    BufferFrame& fixPage(uint64_t pageId, bool exclusive);
    void unfixPage(BufferFrame& frame, bool isDirty);
    ~BufferManager();
};



#endif //PROJECT_BUFFER_MANAGER_H
