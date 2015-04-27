#include <stdint.h>
#include "BufferManager.hpp"

uint64_t maxFramesInMemory;

/* Keeps up to size frames in main memory*/
BufferManager::BufferManager(uint64_t size) {
    maxFramesInMemory = size;
}

/* Writes all dirty frames to disk and free all resources */
BufferManager::~BufferManager() {

}

/* Retrieve frames given a page Id and indication whether or not it is exclusive for the thread
 * Can fail, if no free frame is available and no used frame can be freed*/
BufferFrame &BufferManager::fixPage(uint64_t pageId, bool exclusive) {
    BufferFrame frame;
    return frame;
}

/* Return a frame to the buffer manager indicating whether it is dirty or not.
 * If so, the page mut be written back to disk.
 * If not, it must be written back before unfixPage is called */
void BufferManager::unfixPage(BufferFrame &frame, bool isDirty) {

}
