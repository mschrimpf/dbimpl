#ifndef PROJECT_BUFFER_FRAME_H
#define PROJECT_BUFFER_FRAME_H

#include "../util/spinlock.h"
#include "pthread.h"

class BufferFrame {
	const uint8_t DIRTY_FLAG = 0x1; // 001

private:
	void *data;
	bool used = false;
	uint64_t pageId;
	uint64_t segmentId;
	unsigned usageCount = 0;
	pthread_rwlock_t rwlock;
	uint8_t state; // combine dirty and exclusive flag - see chapter 2, slide 17
	bool isFlagSet(uint8_t mask);

	void setFlagBool(bool flagSet, uint8_t flag);

	void setFlag(uint8_t flag);

	void unsetFlag(uint8_t flag);

public:

	bool usedBefore();

	void setUsedBefore();

	BufferFrame(uint64_t pageId, uint64_t segmentId, void *data);

	void *getData();

	void setDirty(bool dirty);

	bool isDirty();

	void setPageId(uint64_t pageId);

	uint64_t getPageId();

	void setSegmentId(uint64_t segmentId);

	uint64_t getSegmentId();

	void resetFlags();

	void increaseUsageCount();

	void decreaseUsageCount();

	bool isUsed();

	void lock(bool exclusive);

	void unlock();

	unsigned getWaitingCount();

	void setUnusedBefore();
};

#endif //PROJECT_BUFFER_FRAME_H
