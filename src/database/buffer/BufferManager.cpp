#include <stdlib.h>
#include <stdio.h>
#include "BufferManager.hpp"
#include "../util/TwoQList.h"

const unsigned PAGE_SIZE_BYTE = 4096;
const uint64_t PAGE_MASK = 0xff; // TODO
const uint64_t SEGMENT_MASK = 0xff; // TODO

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
	uint64_t pageId, segmentId;
	this->extractPageAndSegmentId(pageAndSegmentId, pageId, segmentId);

	printf("Page and Segment Id extracted. PageId :%llu, SegmentId :%llu\n", pageId, segmentId);

	printf("trying to get global lock\n");
	this->global_lock();
	printf("global lock aquired\n");

	BufferFrame *frame = this->getPageInMemoryOrNull(pageId);
	if (frame != nullptr) {
		printf("Frame for pageId %llu is already in use\n", pageId);
		/*
		 * Since this path is only reached when the frame is already used by some thread,
		 * it is sufficient to only check for the exclusiveness and not the explicit reader count
		 */
		if (exclusive || frame->isExclusive()) {
			this->global_unlock();
			throw "Fixing the same page with an exclusive flag is not allowed";
		}
		this->replacementStrategy->onUse(frame);
	}
		// frame does not exist
	else {
		printf("Frame for pageId %llu does not exist\n", pageId);
		if (this->isSpaceAvailable()) { // don't have to replace anything
			frame = this->createFrame(pageId, segmentId);
			frame->setExclusive(exclusive);
			this->replacementStrategy->push(frame);
		} else {
			frame = this->replacementStrategy->pop();
			if (frame == nullptr) {
				this->global_unlock();
				throw "Frame is not swapped in, no space is available and no pages are poppable";
			}
			frame->setExclusive(exclusive);
			frame->lock();
			this->global_unlock();
			this->writeOutIfNecessary(frame);
			this->global_lock();
			frame->unlock();
			this->reinitialize(frame, pageId);
			this->replacementStrategy->push(frame);
		}
		frame->lock();
		this->loadFromDiskIfExists(frame);
		frame->unlock();
	}

	this->global_unlock();
	printf("global lock released\n");

	return *frame;
}

/**
 * Return a frame to the buffer manager indicating whether it is dirty or not.
 * If dirty, the page manager must write it back to disk.
 * It does not have to write it back immediately,
 * but must not write it back before unfixPage is called.
 */
void BufferManager::unfixPage(BufferFrame &frame, bool isDirty) {
	this->global_lock();

	/*
	 * Only set the dirty flag.
	 * The frame will be swapped out when it is replaced in memory
	 * or when the BufferManager dies.
	 * Hence, the data pointer of the frame is not added to the free pages
	 * because the page data is still occupied although it can be replaced.
	 */
	frame.setDirty(isDirty);
	frame.setUnfixed(true); // TODO set unfixed for multiple readers
	// TODO: delete from map?
	this->global_unlock();
}

void BufferManager::extractPageAndSegmentId(uint64_t pageAndSegmentId, uint64_t &pageId, uint64_t &segmentId) {
	pageId = pageAndSegmentId & PAGE_MASK;
	segmentId = pageAndSegmentId & SEGMENT_MASK;
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
			this->pageIO->writePage(frame->getPageId(), frame->getSegmentId(), frame->getData(), PAGE_SIZE_BYTE);
		}
	}
}

void BufferManager::reinitialize(BufferFrame *frame, uint64_t newPageId) {
	this->pageIO->writePage(frame->getPageId(), frame->getSegmentId(), frame->getData(), PAGE_SIZE_BYTE);
	this->pageFrameMap.erase(frame->getPageId());
	frame->resetFlags();
	this->pageFrameMap[newPageId] = frame;
}

void BufferManager::loadFromDiskIfExists(BufferFrame *frame) {
	this->pageIO->readPage(frame->getPageId(), frame->getSegmentId(), frame->getData(), PAGE_SIZE_BYTE);
}

void BufferManager::global_lock() {
	this->global_mutex.lock();
}

void BufferManager::global_unlock() {
	this->global_mutex.unlock();
}