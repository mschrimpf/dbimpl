#include <stdlib.h>
#include "BufferManager.hpp"
#include "../util/TwoQList.h"

const int PAGE_SIZE_BYTE = 4096;

/* Keeps up to size frames in main memory*/
BufferManager::BufferManager(uint64_t pagesInMemory) {
	this->maxFramesInMemory = pagesInMemory;
	this->pageIO = new PageIOUtil();
	this->replacementStrategy = new TwoQList();
	this->initCache(this->maxFramesInMemory);
}

/* Writes all dirty frames to disk and free all resources */
BufferManager::~BufferManager() {
	this->freeCache();
	delete this->pageIO;
	delete this->replacementStrategy;
}

/* Retrieve frames given a page Id and indication whether or not it is exclusive for the thread
 * Can fail, if no free frame is available and no used frame can be freed*/
BufferFrame &BufferManager::fixPage(uint64_t pageAndSegmentId, bool exclusive) {
	mutex.lock();

	uint64_t pageId, segmentId;
	this->extractPageAndSegmentId(pageAndSegmentId, pageId, segmentId);

	BufferFrame *frame = this->getPageInMemoryOrNull(pageId);
	if (frame != nullptr) {
		if (exclusive || frame->isExclusive()) {
			throw "Fixing the same page with an exclusive flag is not allowed";
		}
		this->replacementStrategy->onUse(frame);
	}
		// frame does not exist
	else {
		if (this->isSpaceAvailable()) { // don't have to replace anything
			frame = this->createFrame(pageId, segmentId);
			this->replacementStrategy->push(frame);
		} else {
			frame = this->replacementStrategy->pop();
			if (frame == nullptr) {
				throw "Frame is not swapped in, no space is available and no pages are poppable";
			}
			this->writeOutIfNecessary(frame);
			this->reinitialize(frame, pageId);
			this->replacementStrategy->push(frame);
		}
		this->loadFromDiskIfExists(frame);
	}

	mutex.unlock();
	frame->lock(exclusive);
	frame->increaseReaderCount();
	return *frame;
}

/**
 * Return a frame to the buer manager indicating whether it is dirty or not.
 * If dirty, the page manager must write it back to disk.
 * It does not have to write it back immediately,
 * but must not write it back before unfixPage is called.
 */
void BufferManager::unfixPage(BufferFrame &frame, bool isDirty) {
	mutex.lock();
	frame.decreaseReaderCount();

	/*
	 * Only set the dirty flag.
	 * The frame will be swapped out when it is replaced in memory
	 * or when the BufferManager dies.
	 * Hence, the data pointer of the frame is not added to the free pages
	 * because the page data is still occupied although it can be replaced.
	 */
	if (frame.getReaderCount() == 0) {
		/* if no one is using the frame anymore, we can add it to the queue again */
		this->replacementStrategy->push(&frame);
	}
	frame.setDirty(isDirty);
	frame.setUnfixed(true);
	// TODO: delete from map?
	frame.unlock();
	mutex.unlock();
}

void BufferManager::extractPageAndSegmentId(uint64_t pageAndSegmentId, uint64_t &pageId, uint64_t &segmentId) {

}

void BufferManager::initCache(uint64_t pages) {
	this->cache = malloc((size_t) (pages * PAGE_SIZE_BYTE));
	for (int i = 0; i < pages; ++i) {
		this->freePages.push_back(this->cache + i * PAGE_SIZE_BYTE);
	}
}

void BufferManager::freeCache() {
	free(this->cache);
}

BufferFrame *BufferManager::getPageInMemoryOrNull(uint64_t pageId) {
	return this->pageFrameMap[pageId];
}

BufferFrame *BufferManager::createFrame(uint64_t pageId, uint64_t segmentId) {
	void *page = this->getFreePage();
	BufferFrame *frame = new BufferFrame(pageId, segmentId, page);
	this->pageFrameMap[pageId] = frame;
	return frame;
}

bool BufferManager::isSpaceAvailable() {
	return !this->freePages.empty();
}

void *BufferManager::getFreePage() {
	void *result = this->freePages.front();
	this->freePages.pop_front();
	return result;
}

void BufferManager::writeOutIfNecessary(BufferFrame *frame) {
	if (frame->isDirty()) {
		if (!frame->isUnfixed()) {
			throw "Frame is dirty but has not been unfixed";
		} else {
			this->pageIO->write(frame->getPageId(), frame->getSegmentId(), frame->getData(), PAGE_SIZE_BYTE);
		}
	}
}

void BufferManager::reinitialize(BufferFrame *frame, uint64_t newPageId) {
	this->pageIO->write(frame->getPageId(), frame->getSegmentId(), frame->getData(), PAGE_SIZE_BYTE);
	this->pageFrameMap.erase(frame->getPageId());
	frame->resetFlags();
	this->pageFrameMap[newPageId] = frame;
}

void BufferManager::loadFromDiskIfExists(BufferFrame *frame) {
	this->pageIO->read(frame->getPageId(), frame->getSegmentId(), frame->getData(), PAGE_SIZE_BYTE);
	//TODO open fd only once, save them and close them at the destructor of Buffermanager? -> machen wir ja in FileIOUtil
	//TODO do we open with WRITE-FLAG/READ-FLAG?
}
