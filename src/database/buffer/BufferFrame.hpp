#ifndef PROJECT_BUFFER_FRAME_H
#define PROJECT_BUFFER_FRAME_H

class BufferFrame {
	const uint8_t DIRTY_FLAG = 0x1; // 001
	const uint8_t EXCLUSIVE_FLAG = 0x2; // 010
	const uint8_t UNFIXED_FLAG = 0x4; // 100

private:
	void *data;
	uint64_t pageId;
	uint64_t segmentId;
	uint8_t state; // combine dirty and exclusive flag - see chapter 2, slide 17
	boost::shared_mutex latch; // TODO: make sure false conflicts are avoided - modulo(sizeof(BufferFrame), cache_line_size_byte = 64) = 0!

	bool isFlagSet(uint8_t mask);

	void setFlagBool(bool flagSet, uint8_t flag);

	void setFlag(uint8_t flag);

	void unsetFlag(uint8_t flag);

public:
	BufferFrame(uint64_t pageId, uint64_t segmentId, void *data);

	void *getData();

	void setDirty(bool dirty);

	bool isDirty();

	void setExclusive(bool exclusive);

	bool isExclusive();

	void setUnfixed(bool unfixed);

	bool isUnfixed();

	uint64_t getPageId();

	uint64_t getSegmentId();

	void resetFlags();
};

#endif //PROJECT_BUFFER_FRAME_H
