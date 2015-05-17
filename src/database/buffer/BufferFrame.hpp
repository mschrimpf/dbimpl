#ifndef PROJECT_BUFFER_FRAME_H
#define PROJECT_BUFFER_FRAME_H

#include "pthread.h"
//#include "BufferManager.hpp"
#include <stdint.h>

class BufferFrame {
	const uint8_t DIRTY_FLAG = 0x1; // 001
	const uint8_t USED_FLAG = 0x2; // 010

private:
	bool isFlagSet(uint8_t mask);

	void setFlagBool(bool flagSet, uint8_t flag);

	void setFlag(uint8_t flag);

	void unsetFlag(uint8_t flag);

public:
	struct Header {
		void *data;
		uint64_t pageId;
		uint64_t segmentId;
		pthread_rwlock_t rwlock;
		uint8_t state; // combine dirty and exclusive flag - see chapter 2, slide 17
	} header; // TODO: private

	bool usedBefore();

	void setUsedBefore();

	BufferFrame(uint64_t pageId, uint64_t segmentId, void *data);

	~BufferFrame();

	void *getData();

	void setDirty(bool dirty);

	bool isDirty();

	void setPageId(uint64_t pageId);

	uint64_t getPageId();

	void setSegmentId(uint64_t segmentId);

	uint64_t getSegmentId();

	void resetFlags();

	void lock(bool exclusive);

	void unlock();

	void setUnusedBefore();

	bool tryLock(bool exclusive);
};

#endif //PROJECT_BUFFER_FRAME_H
