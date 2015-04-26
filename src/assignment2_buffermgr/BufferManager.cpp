//
// Created by Martin on 26.04.2015.
//

#include "BufferManager.h"

const PAGE_SIZE_BYTE = 4096;

BufferManager::~BufferManager() {

}

BufferManager::BufferManager(unsigned pageCount) {

}

BufferFrame& BufferManager::fixPage(uint64_t pageId, bool exclusive) {

}

void BufferManager::unfixPage(BufferFrame& frame, bool isDirty) {

}