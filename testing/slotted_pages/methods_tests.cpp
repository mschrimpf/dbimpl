//
// Created by Martin on 11.05.2015.
//

#ifdef _WIN32
#define PRId64 "llu"
#define PRId16 "d"
#else
#include <inttypes.h>
#endif

#include "gtest/gtest.h"
#include "../../src/database/slotted_pages/SPSegment.hpp"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

class SPSegmentTest : public Test {
private:
	BufferManager *bufferManager;
	char *allocatedData = nullptr;
public:
	static const uint64_t SEGMENT_ID = 1;
	SPSegment *segment;

	void SetUp() {
		bufferManager = new BufferManager(65536);
		segment = new SPSegment(*bufferManager, SEGMENT_ID);
	}

	void TearDown() {
		delete this->segment;
		delete this->bufferManager;

		std::string filename = std::to_string(SEGMENT_ID);
		remove(filename.c_str());

		if (allocatedData != nullptr) {
			free(allocatedData);
		}
	};

	/**
	 * Allocates data that will automatically be free'd at the end of the test.
	 */
	char *allocateData(size_t len) {
		this->allocatedData = (char *) malloc(len);
		return this->allocatedData;
	}

	void testUpdate(size_t initialLength, size_t updatedLength) {
		TID tid = insertRecord(initialLength);

		char *updatedData = allocateData(updatedLength);
		const Record updateRecord(updatedLength, updatedData);
		segment->update(tid, updateRecord);
		Record lookedupRecord = segment->lookup(tid);

		expectRecordsEqual(&updateRecord, &lookedupRecord);
	}

	TID insertRecord(size_t length) {
		char *data = allocateData(length);
		const Record record(length, data);
		TID tid = segment->insert(record);
		return tid;
	}

	void expectRecordsEqual(const Record *r1, const Record *r2) {
		EXPECT_EQ(r1->getLen(), r2->getLen());
		const char *d1 = r1->getData();
		const char *d2 = r2->getData();
		bool recordsEqual = std::memcmp(d1, d2, r1->getLen()) == 0;
		EXPECT_TRUE(recordsEqual);
	}
};

TEST_F(SPSegmentTest, InsertAndLookupMatch) {
	size_t len = 100;
	char *data = allocateData(len);
	const Record record(len, data);
	TID tid = segment->insert(record);
	printf("Inserted TID: page %" PRId64 ", slot %" PRId16 "\n", tid.pageId, tid.slotOffset);
	Record lookedupRecord = segment->lookup(tid);

	expectRecordsEqual(&record, &lookedupRecord);
}

TEST_F(SPSegmentTest, RemoveEmpty) {
	TID tid(SEGMENT_ID, 0);
	bool exists = segment->remove(tid);
	EXPECT_FALSE(exists);
}

TEST_F(SPSegmentTest, RemoveOnce) {
	TID tid = insertRecord(100);
	bool exists = segment->remove(tid);
	EXPECT_TRUE(exists);
}

TEST_F(SPSegmentTest, RemoveTwice) {
	TID tid = insertRecord(100);
	bool exists = segment->remove(tid);
	EXPECT_TRUE(exists);
	exists = segment->remove(tid);
	EXPECT_FALSE(exists);
}

TEST_F(SPSegmentTest, InsertRemoveInsertSameTid) {
	TID tid1 = insertRecord(100);
	bool exists = segment->remove(tid1);
	EXPECT_TRUE(exists);
	TID tid2 = insertRecord(100);
	EXPECT_EQ(tid1, tid2);
}

TEST_F(SPSegmentTest, UpdateSameSize) {
	testUpdate(100, 100);
}

TEST_F(SPSegmentTest, UpdateSmallerSize) {
	testUpdate(100, 50);
}

TEST_F(SPSegmentTest, UpdateLargerSize) {
	testUpdate(100, 150);
}
