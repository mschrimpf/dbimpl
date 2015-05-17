#ifndef PROJECT_BUFFER_MANAGER_H
#define PROJECT_BUFFER_MANAGER_H

#include <stdint.h>
#include <list>
#include <map>
#include <condition_variable>
#include "mutex"
#include "BufferFrame.hpp"
#include "IReplacementStrategy.h"
#include "PageIOUtil.h"
#include "IPageIO.h"

class BufferManager {
public:
	static const unsigned FRAME_SIZE_BYTE = 4 * 1024;
	static const unsigned DATA_SIZE_BYTE = FRAME_SIZE_BYTE - sizeof(BufferFrame::Header);

	BufferManager(uint64_t pagesInMemory);

	~BufferManager();

	BufferFrame &fixPage(uint64_t segmentId, uint64_t pageId, bool exclusive);

	BufferFrame &fixPage(uint64_t pageAndSegmentId, bool exclusive);

	void unfixPage(BufferFrame &frame, bool isDirty);

private:
	static const uint64_t SEGMENT_MASK = 0xFFFF000000000000;
	static const uint64_t PAGE_MASK = 0xFFFFFFFFFFFF;

	std::condition_variable replacementAccessed;
	uint64_t maxFramesInMemory;
	char *cache;
	/** has to be synchronized */
	IReplacementStrategy *replacementStrategy;
	/** has to be synchronized */
	std::unordered_map<uint64_t, BufferFrame *> pageFrameMap;
	/** has to be synchronized */
	std::list<char *> freePages;
	/** synchronized */
	IPageIO *pageIO;

	std::mutex global_mutex;

	void extractPageAndSegmentId(uint64_t pageAndSegmentId, uint64_t &pageId, uint64_t &segmentId);

	void initCache(uint64_t pages);

	void freeCache();

	BufferFrame *getPageInMemoryOrNull(uint64_t pageId);

	BufferFrame *createFrame(uint64_t pageId, uint64_t segmentId);

	void *getFreePage();

	bool isSpaceAvailable();

	void reinitialize(BufferFrame *frame, uint64_t newPageId, uint64_t newSegmentId);

	void writeOut(BufferFrame *frame);

	void loadFromDiskIfExists(BufferFrame *frame);

	void global_lock();

	void global_unlock();
};


#endif //PROJECT_BUFFER_MANAGER_H
