#include <stdint.h>
#include "BufferFrame.hpp"

uint64_t pageId;
bool isDirty;
bool isExclusive; //TODO brauchen wir das in dem frame?

void *BufferFrame::getData() {
    return nullptr;
}


