#ifndef PROJECT_BUFFER_MANAGER_H
#define PROJECT_BUFFER_MANAGER_H

#include <stdint.h>
#include <mutex>
#include <map>
#include "BufferFrame.hpp"
#include "IReplacementStrategy.h"

class BufferManager {
public:
	BufferManager(uint64_t pagesInMemory);

	BufferManager::~BufferManager();

	BufferFrame &fixPage(uint64_t pageAndSegmentId, bool exclusive);

	void unfixPage(BufferFrame &frame, bool isDirty);

private:
	uint64_t maxFramesInMemory;
	IReplacementStrategy replacementStrategy;
	void *cache;
	map<uint64_t, BufferFrame *> pageMemoryMap;
	list <void *> freePages;
	IPageIO pageIO;

	void extractPageAndSegmentId(uint64_t pageAndSegmentId, uint64_t &pageId, uint64_t &segmentId);

	void initCache(uint64_t pages);

	void freeCache();

	BufferFrame *getPageInMemory(uint64_t pageId);

	BufferFrame *createFrame(uint64_t pageId);

	void * getFreePage();

	bool isSpaceAvailable();

	void reinitialize(BufferFrame *frame, uint64_t newPageId);

	void writeOutIfNecessary(BufferFrame *frame);

	void loadFromDiskIfExists(BufferFrame *frame);
};


#endif //PROJECT_BUFFER_MANAGER_H
