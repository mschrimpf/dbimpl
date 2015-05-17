#ifndef PROJECT_BUFFER_FRAME_H
#define PROJECT_BUFFER_FRAME_H

#include "pthread.h"
//#include "BufferManager.hpp"
#include <stdint.h>

class BufferFrame {
private:
	static const uint8_t DIRTY_FLAG = 0x1; // 0000 0001
	static const uint8_t USED_FLAG = 0x2; // 0000 0010

public:
	static const uint8_t IN_LRU_QUEUE = 0x4; // 0000 0100
	static const uint8_t IN_FIFO_QUEUE = 0x8; // 0000 1000

	struct Header {
		void *data;
		uint64_t pageId;
		uint64_t segmentId;
		pthread_rwlock_t rwlock;
		uint8_t state; // combine dirty and exclusive flag - see chapter 2, slide 17
	} header; // TODO: private

	BufferFrame(uint64_t pageId, uint64_t segmentId, void *data);

	~BufferFrame();

	void *getData();

	/* Page and Segment */

	void setPageId(uint64_t pageId);

	uint64_t getPageId();

	void setSegmentId(uint64_t segmentId);

	uint64_t getSegmentId();

	/* Latching */

	void lock(bool exclusive);

	void unlock();

	bool tryLock(bool exclusive);

	/* Flagging */

	void setFlag(uint8_t flag);

	bool isFlagSet(uint8_t mask);

	void setFlagBool(bool flagSet, uint8_t flag);

	void unsetFlag(uint8_t flag);

	void setDirty(bool dirty);

	bool isDirty();

	bool usedBefore();

	void setUsedBefore();

	void setUnusedBefore();

	void resetFlags();
};

#endif //PROJECT_BUFFER_FRAME_H
