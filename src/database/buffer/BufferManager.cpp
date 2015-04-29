#include <stdlib.h>
#include "BufferManager.hpp"

const int PAGE_SIZE_BYTE = 4096;

/* Keeps up to size frames in main memory*/
BufferManager::BufferManager(uint64_t pagesInMemory) {
	this->maxFramesInMemory = pagesInMemory;
	this->initCache(this->maxFramesInMemory);
}

/* Writes all dirty frames to disk and free all resources */
BufferManager::~BufferManager() {
	this->freeCache();
}

/* Retrieve frames given a page Id and indication whether or not it is exclusive for the thread
 * Can fail, if no free frame is available and no used frame can be freed*/
BufferFrame &BufferManager::fixPage(uint64_t pageAndSegmentId, bool exclusive) {
	//mutex lock

	uint64_t pageId, segmentId;
	this->extractPageAndSegmentId(pageAndSegmentId, pageId, segmentId);

	BufferFrame *frame = this->getPageInMemory(pageId);
	if (frame != nullptr) {
		if (exclusive || frame->isExclusive()) {
			throw "Fixing the same page with an exclusive flag is not allowed";
		}
		replacementStrategy.onUse(frame);
	}
		// frame does not exist
	else {
		if (this->isSpaceAvailable()) { // don't have to replace anything
			frame = this->createFrame(pageId);
			replacementStrategy.push(frame);
		} else {
			frame = replacementStrategy.pop();
			this->writeOutIfNecessary(frame);
			this->reinitialize(frame, pageId);
			replacementStrategy.push(frame);
		}
		this->loadFromDiskIfExists(frame);
	}

	//mutex unlock
	//frame lock(exclusive)
	//frame increase reader
	return *frame;
}

/* Return a frame to the buffer manager indicating whether it is dirty or not.
 * If so, the page mut be written back to disk.
 * If not, it must be written back before unfixPage is called */
void BufferManager::unfixPage(BufferFrame &frame, bool isDirty) {
	//mutex lock
	//frame decrease reader

	//frame unlock
	//mutex unlock
}

void BufferManager::extractPageAndSegmentId(uint64_t pageAndSegmentId, uint64_t &pageId, uint64_t &segmentId) {

}

void BufferManager::initCache(uint64_t pages) {
	this->cache = malloc((size_t) (pages * PAGE_SIZE_BYTE));
	for (int i = 0; i < pages; ++i) {
		this->freePages.add(this->cache + i * PAGE_SIZE_BYTE);
	}
}

void BufferManager::freeCache() {
	free(this->cache);
}

BufferFrame *BufferManager::getPageInMemory(uint64_t pageId) {
	void *data = this->pageMemoryMap[pageId]; // pre-assign space for map
	return nullptr;
}

BufferFrame *BufferManager::createFrame(uint64_t pageId) {
	void *page = this->getFreePage();
	BufferFrame *frame = new BufferFrame(pageId, page);
	this->pageMemoryMap[pageId] = frame;
}

bool BufferManager::isSpaceAvailable() {
	return !this->freePages->empty();
}

void *BufferManager::getFreePage() {
	void *result = this->freePages->front;
	this->freePages->pop_front();
	return result;
}

void BufferManager::writeOutIfNecessary(BufferFrame *frame) {
	if (frame->isDirty()) {
		if (!frame->isUnfixed()) {
			throw "Frame is dirty but has not been unfixed";
			// TODO: is this desired behaviour?
		} else {
			this->pageIO->write(frame->getPageId(), frame->getSegmentId(), frame->getData(), PAGE_SIZE_BYTE);
		}
	}
}

void BufferManager::reinitialize(BufferFrame *frame, uint64_t newPageId) {
	this->pageIO->write(frame->getPageId(), frame->getSegmentId(), frame->getData());
	this->pageMemoryMap.erase(frame->getPageId());
	this->pageMemoryMap[newPageId] = frame;
}

void BufferManager::loadFromDiskIfExists(BufferFrame *frame) {
	this->pageIO->load(frame->getPageId(), frame->getSegmentId(), frame->getData());
}
