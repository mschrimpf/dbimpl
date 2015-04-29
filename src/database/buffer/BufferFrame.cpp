#include <stdint.h>
#include "BufferFrame.hpp"

void *BufferFrame::getData() {
	return this->data;
}


uint64_t BufferFrame::getPageId() {

}

uint64_t BufferFrame::getSegmentId() {

}

bool BufferFrame::isDirty() {
	return this->isFlagSet(DIRTY_MASK);
}

bool BufferFrame::isExclusive() {
	return this->isFlagSet(EXCLUSIVE_MASK);
}

bool BufferFrame::isUnfixed() {
	return this->isFlagSet(UNFIXED_MASK);
}

bool BufferFrame::isFlagSet(uint8_t mask) {
	return (this->state & mask) == mask;
}

void BufferFrame::setFlag(uint8_t mask) {
	this->state |= mask;
}

void BufferFrame::unsetFlag(uint8_t mask) {
	this->state &= ~mask;
}