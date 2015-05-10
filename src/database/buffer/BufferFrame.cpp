
#include "BufferFrame.hpp"

BufferFrame::BufferFrame(uint64_t pageId, uint64_t segmentId, void *data) {
	this->pageId = pageId;
	this->segmentId = segmentId;
	this->data = data;
	pthread_rwlock_init(&rwlock, nullptr);
}

BufferFrame::~BufferFrame(){
	pthread_rwlock_destroy(&rwlock);
}

void *BufferFrame::getData() {
	return this->data;
}

void BufferFrame::setPageId(uint64_t pageId) {
	this->pageId = pageId;
}

uint64_t BufferFrame::getPageId() {
	return this->pageId;
}

void BufferFrame::setSegmentId(uint64_t segmentId) {
	this->segmentId = segmentId;
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
	bool result = (this->state & mask) == mask;
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
	this->state |= flag;
}

void BufferFrame::unsetFlag(uint8_t flag) {
	this->state &= ~flag;
}

void BufferFrame::resetFlags() {
	this->state &= 0x0;
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


bool BufferFrame::usedBefore() {
	return this->isFlagSet(USED_FLAG);
}

void BufferFrame::setUsedBefore() {
	this->setFlag(USED_FLAG);
}

void BufferFrame::setUnusedBefore(){
	this->unsetFlag(USED_FLAG);
}

bool BufferFrame::tryLock(bool exclusive) {
	int result;
	if (exclusive){
		result = pthread_rwlock_trywrlock(&rwlock);
	}else{
		result = pthread_rwlock_tryrdlock(&rwlock);
	}
	return result == 0;
}
