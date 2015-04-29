#include <stdint.h>
#include "BufferFrame.hpp"

BufferFrame::BufferFrame(uint64_t pageId, uint64_t segmentId, void *data) {
	this->pageId = pageId;
	this->segmentId = segmentId;
	this->data = data;
}

void *BufferFrame::getData() {
	return this->data;
}

uint64_t BufferFrame::getPageId() {
	return this->pageId;
}

uint64_t BufferFrame::getSegmentId() {
	return this->segmentId;
}

void BufferFrame::setDirty(bool dirty) {
	this->setFlagBool(dirty, DIRTY_FLAG);
}

bool BufferFrame::isDirty() {
	return this->isFlagSet(DIRTY_FLAG);
}

void BufferFrame::setExclusive(bool exclusive) {
	this->setFlagBool(exclusive, EXCLUSIVE_FLAG);
}

bool BufferFrame::isExclusive() {
	return this->isFlagSet(EXCLUSIVE_FLAG);
}

void BufferFrame::setUnfixed(bool unfixed) {
	this->setFlagBool(unfixed, UNFIXED_FLAG);
}

bool BufferFrame::isUnfixed() {
	return this->isFlagSet(UNFIXED_FLAG);
}

bool BufferFrame::isFlagSet(uint8_t mask) {
	return (this->state & mask) == mask;
}

void BufferFrame::setFlagBool(bool flagSet, uint8_t flag) {
	if(flagSet) {
		this->setFlag(flag);
	} else {
		this->unsetFlag(flag);
	}
}

void BufferFrame::setFlag(uint8_t flag) {
	this->state |= flag;
}

void BufferFrame::unsetFlag(uint8_t flag) {
	this->state &= ~flag;
}

void BufferFrame::resetFlags() {
	this->state &= 0x0;
}

unsigned int BufferFrame::getReaderCount() {
	return readerCount;
}

void BufferFrame::unlock() {
	latch.unlock();
}

void BufferFrame::lock(bool exclusive) {
	if (exclusive){
		boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(latch);
	}else{
		boost::shared_lock<boost::shared_mutex> lock(latch);
	}
}

