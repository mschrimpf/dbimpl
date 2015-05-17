
#include "BufferFrame.hpp"

BufferFrame::BufferFrame(uint64_t pageId, uint64_t segmentId, void *data) {
	this->header.pageId = pageId;
	this->header.segmentId = segmentId;
	this->header.data = data;
	pthread_rwlock_init(&header.rwlock, nullptr);
}

BufferFrame::~BufferFrame(){
	pthread_rwlock_destroy(&header.rwlock);
}

void *BufferFrame::getData() {
	return this->header.data;
}

/* Page and Segment */

void BufferFrame::setPageId(uint64_t pageId) {
	this->header.pageId = pageId;
}

uint64_t BufferFrame::getPageId() {
	return this->header.pageId;
}

void BufferFrame::setSegmentId(uint64_t segmentId) {
	this->header.segmentId = segmentId;
}

uint64_t BufferFrame::getSegmentId() {
	return this->header.segmentId;
}

/* Latching */

void BufferFrame::lock(bool exclusive) {
	if(exclusive) {
		pthread_rwlock_wrlock(&header.rwlock);
	} else {
		pthread_rwlock_rdlock(&header.rwlock);
	}
}

void BufferFrame::unlock() {
	pthread_rwlock_unlock(&header.rwlock);
}

bool BufferFrame::tryLock(bool exclusive) {
	int result;
	if (exclusive){
		result = pthread_rwlock_trywrlock(&header.rwlock);
	}else{
		result = pthread_rwlock_tryrdlock(&header.rwlock);
	}
	return result == 0;
}

/* States */

void BufferFrame::setDirty(bool dirty) {
	this->setFlagBool(dirty, DIRTY_FLAG);
}

bool BufferFrame::isDirty() {
	return this->isFlagSet(DIRTY_FLAG);
}

bool BufferFrame::usedBefore() {
	return this->isFlagSet(USED_FLAG);
}

void BufferFrame::setUsedBefore() {
	this->setFlag(USED_FLAG);
}

void BufferFrame::setUnusedBefore(){
	this->unsetFlag(USED_FLAG);
}

bool BufferFrame::isFlagSet(uint8_t mask) {
	bool result = (this->header.state & mask) == mask;
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
	this->header.state |= flag;
}

void BufferFrame::unsetFlag(uint8_t flag) {
	this->header.state &= ~flag;
}

void BufferFrame::resetFlags() {
	this->header.state &= 0x0;
}
