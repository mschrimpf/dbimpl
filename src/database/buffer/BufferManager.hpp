#ifndef PROJECT_BUFFER_MANAGER_H
#define PROJECT_BUFFER_MANAGER_H

#include <stdint.h>
#include <list>
#include <map>
#include "../util/spinlock.h"
#include "BufferFrame.hpp"
#include "IReplacementStrategy.h"
#include "../util/PageIOUtil.h"
#include "IPageIO.h"

class BufferManager {
public:
	BufferManager(uint64_t pagesInMemory);

	~BufferManager();

	BufferFrame &fixPage(uint64_t pageAndSegmentId, bool exclusive);

	void unfixPage(BufferFrame &frame, bool isDirty);

private:
	uint64_t maxFramesInMemory;
	char *cache;
	/** has to be synchronized */
	IReplacementStrategy * replacementStrategy;
	/** has to be synchronized */
	std::unordered_map<uint64_t, BufferFrame *> pageFrameMap;
	/** has to be synchronized */
	std::list <char *> freePages;
	/** synchronized */
	IPageIO * pageIO;

	spinlock global_mutex;

	void extractPageAndSegmentId(uint64_t pageAndSegmentId, uint64_t &pageId, uint64_t &segmentId);

	void initCache(uint64_t pages);

	void freeCache();

	BufferFrame *getPageInMemoryOrNull(uint64_t pageId);

	BufferFrame *createFrame(uint64_t pageId, uint64_t segmentId);

	void * getFreePage();

	bool isSpaceAvailable();

	void reinitialize(BufferFrame *frame, uint64_t newPageId);

	void writeOut(BufferFrame *frame);

	void loadFromDiskIfExists(BufferFrame *frame);

	void global_lock();

	void global_unlock();
};


#endif //PROJECT_BUFFER_MANAGER_H
