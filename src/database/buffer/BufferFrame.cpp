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

bool BufferFrame::isFlagSet(uint8_t mask) {
	this->latchFlags();
	bool result = (this->state & mask) == mask;
	this->unlatchFlags();
	return result;
}

void BufferFrame::setFlagBool(bool flagSet, uint8_t flag) {
	if (flagSet) {
		this->setFlag(flag);
	} else {
		this->unsetFlag(flag);
	}
}

void BufferFrame::setFlag(uint8_t flag) {
	this->latchFlags();
	this->state |= flag;
	this->unlatchFlags();
}

void BufferFrame::unsetFlag(uint8_t flag) {
	this->latchFlags();
	this->state &= ~flag;
	this->unlatchFlags();
}

void BufferFrame::resetFlags() {
	this->latchFlags();
	this->state &= 0x0;
	this->unlatchFlags();
}

bool BufferFrame::isUsed() {
	return usageCount > 0;
}

void BufferFrame::latchFlags() {
	latch.lock();  //TODO might be unnecessary
}

void BufferFrame::unlatchFlags() {
	latch.unlock(); //TODO might be unnecessary
}

void BufferFrame::lock(bool exclusive) {
	if(exclusive) {
		pthread_rwlock_wrlock(&rwlock);
	} else {
		pthread_rwlock_rdlock(&rwlock);
	}
}

void BufferFrame::unlock() {
	pthread_rwlock_unlock(&rwlock);
}

void BufferFrame::increaseUsageCount() {
	usageCount++;
}

void BufferFrame::decreaseUsageCount() {
	usageCount--;
}