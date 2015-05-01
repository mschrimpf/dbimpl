#ifndef PROJECT_BUFFER_MANAGER_H
#define PROJECT_BUFFER_MANAGER_H

#include <stdint.h>
#include <boost/mutex>
#include <list>
#include <map>
#include "BufferFrame.hpp"
#include "IReplacementStrategy.h"
#include "../util/PageIOUtil.h"
#include "IPageIO.h"

class BufferManager {
public:
	BufferManager(uint64_t pagesInMemory);

	BufferManager::~BufferManager();

	BufferFrame &fixPage(uint64_t pageAndSegmentId, bool exclusive);

	void unfixPage(BufferFrame &frame, bool isDirty);

private:
	uint64_t maxFramesInMemory;
	void *cache;
	/** synchronized */
	IReplacementStrategy * replacementStrategy;
	/** synchronized */
	std::unordered_map<uint64_t, BufferFrame *> pageFrameMap;
	/** synchronized */
	std::list <void *> freePages;
	/** synchronized */
	IPageIO * pageIO;

	boost::mutex mtx;

	void extractPageAndSegmentId(uint64_t pageAndSegmentId, uint64_t &pageId, uint64_t &segmentId);

	void initCache(uint64_t pages);

	void freeCache();

	BufferFrame *getPageInMemoryOrNull(uint64_t pageId);

	BufferFrame *createFrame(uint64_t pageId, uint64_t segmentId);

	void * getFreePage();

	bool isSpaceAvailable();

	void reinitialize(BufferFrame *frame, uint64_t newPageId);

	void writeOutIfNecessary(BufferFrame *frame);

	void loadFromDiskIfExists(BufferFrame *frame);
};


#endif //PROJECT_BUFFER_MANAGER_H
