#include <stdint.h>
#include "BufferManager.hpp"

BufferManager::BufferManager(uint64_t size) {

}

BufferManager::~BufferManager() {

}

BufferFrame &BufferManager::fixPage(uint64_t pageId, bool exclusive) {
    return <#initializer#>;
}

void BufferManager::unfixPage(BufferFrame &frame, bool isDirty) {

}
