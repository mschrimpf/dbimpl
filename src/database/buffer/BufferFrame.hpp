#ifndef PROJECT_BUFFER_FRAME_H
#define PROJECT_BUFFER_FRAME_H

#include "../util/spinlock.h"
#include "pthread.h"

class BufferFrame {
	const uint8_t DIRTY_FLAG = 0x1; // 001

private:
	void *data;
	uint64_t pageId;
	uint64_t segmentId;
	unsigned usageCount;
	pthread_rwlock_t rwlock;
	uint8_t state; // combine dirty and exclusive flag - see chapter 2, slide 17
	spinlock latch; // TODO: make sure false conflicts are avoided - modulo(sizeof(BufferFrame), cache_line_size_byte = 64) = 0!
	bool isFlagSet(uint8_t mask);

	void setFlagBool(bool flagSet, uint8_t flag);

	void setFlag(uint8_t flag);

	void unsetFlag(uint8_t flag);

public:

	BufferFrame(uint64_t pageId, uint64_t segmentId, void *data);

	void *getData();

	void setDirty(bool dirty);

	bool isDirty();

	uint64_t getPageId();

	uint64_t getSegmentId();

	void resetFlags();

	void latchFlags();

	void unlatchFlags();

	void increaseUsageCount();

	void decreaseUsageCount();

	bool isUsed();

	void lock(bool exclusive);

	void unlock();
};

#endif //PROJECT_BUFFER_FRAME_H
