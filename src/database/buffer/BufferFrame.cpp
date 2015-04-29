#include <stdint.h>
#include "BufferFrame.hpp"

void *BufferFrame::getData() {
    return this->data;
}


uint64_t BufferFrame::getPageId() {

}

uint64_t BufferFrame::getSegmentId() {

}

bool BufferFrame::isUnfixed() {
    return false;
}
