#include <stdlib.h>
#include <stdint.h>
#include <mutex>
#include "BufferManager.hpp"
#include "TwoQList.h"
#include "../util/debug.h"
#include "PageIOUtil.h"

#ifdef _WIN32
#define PRId64 "llu"
#else
#include <inttypes.h>
#endif

/* Keeps up to size frames in main memory*/
BufferManager::BufferManager(uint64_t pagesInMemory) {
	this->maxFramesInMemory = pagesInMemory;
	this->pageIO = new PageIOUtil();
	this->replacementStrategy = new TwoQList();
	this->initCache(this->maxFramesInMemory);
}

/* Writes all dirty frames to disk and free all resources */
BufferManager::~BufferManager() {
	debug("tear down\n");
	this->global_lock();
	/* write back all frames */
	BufferFrame *frame;
	while ((frame = replacementStrategy->pop()) != nullptr) {
		frame->lock(true);
		if (frame->isDirty()) {
			debug(frame->getPageId(), "writing out in destructor");
			writeOut(frame);
		}
		frame->unlock();
	}

	/* clean frames */
	for (auto pair : pageFrameMap) {
		if (pair.second != nullptr) {
			delete(pair.second);
		}
	}

	this->freeCache();
	delete this->pageIO;
	delete this->replacementStrategy;
	this->global_unlock();
}

/* Retrieve frames given a page Id and indication whether or not it is exclusive for the thread
 * Can fail, if no free frame is available and no used frame can be freed*/
BufferFrame &BufferManager::fixPage(uint64_t pageAndSegmentId, bool exclusive) {
	uint64_t pageId, segmentId;
	this->extractPageAndSegmentId(pageAndSegmentId, pageId, segmentId);
	return this->fixPage(segmentId, pageId, exclusive);
}

BufferFrame &BufferManager::fixPage(uint64_t segmentId, uint64_t pageId, bool exclusive) {
	global_lock();

	debug(pageId, "trying to get global lock after extracting page and segment id");

	BufferFrame *frame = this->getPageInMemoryOrNull(pageId);
	if (frame != nullptr) {
		//frame exists
		debug(pageId, "Frame exists in Memory");
		this->replacementStrategy->remove(frame);
		/* set used as we get the frame FROM the replacement strategy and therefore it has to be inserted in LRU next time */
		frame->setUsedBefore();
		this->global_unlock();
		debug(pageId, "global lock released");
		debug(pageId, "try to get frame lock");
		frame->lock(exclusive);
		debug(pageId, "framelock aquired");
	} else {
		// frame does not exist
		debug(pageId, "Frame for pageId %" PRId64 " does not exist", pageId);
		if (this->isSpaceAvailable()) { // don't have to replace anything
			debug(pageId, "space available -> create frame");
			frame = this->createFrame(pageId, segmentId);

			/* set unused as we create a new frame and therefore it has to be inserted in FIFO */
			frame->setUnusedBefore();

			debug(pageId, "New frame created");
			this->global_unlock();
			debug(pageId, "global unlocked");
		} else { // TODO: might need a while here
			debug(pageId, "no space available -> replace");
			frame = this->replacementStrategy->pop();
			debug(pageId, "Popping frame with page id %" PRId64,
				  frame != nullptr ? frame->getPageId() : (unsigned long long) -1);
			if (frame == nullptr) {
				this->global_unlock();
				throw std::runtime_error("Frame is not swapped in, no space is available and no pages are poppable");
			}
			frame->lock(true);
			// write out if necessary
			if (frame->isDirty()) {
				debug(pageId, "locking frame for dirty write out");
				this->global_unlock(); // TODO: we're running into concurency issues here - but if we swap the order, we do so as well
				this->writeOut(frame);
				frame->setDirty(false);
				debug(pageId, "written out");
				frame->unlock();
				this->global_lock();

				if (frame->tryLock(true)) {
					if (frame->isDirty()) {
						frame->unlock();
					} else {
						frame->unlock();
						replacementStrategy->remove(frame);
					}
				}
			} else {
				debug(pageId, "Not writing out since not dirty");
				frame->unlock();
			}
			this->reinitialize(frame, pageId, segmentId);
			debug(pageId, "frame reinitialized");

			// TODO: use another mutex for downgrade
		}
		debug(pageId, "Lock frame to load from disk");
		frame->lock(true); // TODO: optimize - only lock if page exists
		debug(pageId, "frame locked");
		this->loadFromDiskIfExists(frame);
		debug(pageId, "loaded from disk");
		frame->unlock();
		debug(pageId, "unlocked frame");
		debug(pageId, "try to lock frame with id: %" PRId64 " | exclusive: %s", frame->getPageId(),
			  exclusive ? "true" : "false");
		frame->lock(exclusive);
	}
	this->global_unlock();
	return *frame;
}

/**
 * Return a frame to the buffer manager indicating whether it is dirty or not.
 * If dirty, the page manager must write it back to disk.
 * It does not have to write it back immediately,
 * but must not write it back before unfixPage is called.
 */
void BufferManager::unfixPage(BufferFrame &frame, bool isDirty) {
	/*
	 * Only set the dirty flag.
	 * The frame will be swapped out when it is replaced in memory
	 * or when the BufferManager dies.
	 * Hence, the data pointer of the frame is not added to the free pages
	 * because the page data is still occupied although it can be replaced.
	 */
	if (isDirty) {
		frame.setDirty(true);
	}

	debug(frame.getPageId(), "Unfix - dirty: %s",
		  isDirty ? "true" : "false");
	debug(frame.getPageId(), "Add frame to replacement strategy");
	this->global_lock();
	this->replacementStrategy->push(&frame);
	frame.unlock();
}

void BufferManager::extractPageAndSegmentId(uint64_t pageAndSegmentId, uint64_t &pageId, uint64_t &segmentId) {
	pageId = pageAndSegmentId & PAGE_MASK;
	segmentId = pageAndSegmentId & SEGMENT_MASK;
}

void BufferManager::initCache(uint64_t pages) {
	pageFrameMap.reserve(pages);
	this->cache = (char *) malloc((size_t) (pages * PAGE_SIZE_BYTE));
	for (int i = 0; i < pages; ++i) {
		char *page_ptr = this->cache + i * PAGE_SIZE_BYTE;
		this->freePages.push_back(page_ptr);
	}
}

void BufferManager::freeCache() {
	debug("Freeing cache\n");
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

void BufferManager::reinitialize(BufferFrame *frame, uint64_t newPageId, uint64_t newSegmentId) {
	this->pageFrameMap.erase(frame->getPageId());
	frame->resetFlags();
	frame->setUnusedBefore();
	frame->setPageId(newPageId);
	frame->setSegmentId(newSegmentId);
	this->pageFrameMap[newPageId] = frame;
}

void BufferManager::writeOut(BufferFrame *frame) {
	this->pageIO->writePage(frame->getPageId(), frame->getSegmentId(), frame->getData(), PAGE_SIZE_BYTE);
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