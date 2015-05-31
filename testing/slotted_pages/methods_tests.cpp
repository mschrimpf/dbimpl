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
#include <vector>

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
	std::vector<char *> allocatedData;
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

		for(auto it : allocatedData) {
			free(it);
		}
	};

	/**
	 * Allocates data that will automatically be free'd at the end of the test.
	 */
	char *allocateData(size_t len) {
		char *data = (char *) malloc(len);
		this->allocatedData.push_back(data);
		return data;
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

TEST_F(SPSegmentTest, InsertTooMuch) {
	ASSERT_THROW(insertRecord(BufferManager::DATA_SIZE_BYTE /* header takes space away from this anyway */),
				 std::invalid_argument);
}

TEST_F(SPSegmentTest, InsertLookupBigData) {
	size_t len = BufferManager::DATA_SIZE_BYTE / 2;
	char *data = this->allocateData(len);
	const Record record1(len, data);
	TID tid1 = segment->insert(record1);

	char *data2 = this->allocateData(len);
	const Record record2(len, data2);
	TID tid2 = segment->insert(record2);

	Record lookedupRecord1 = segment->lookup(tid1);
	expectRecordsEqual(&record1, &lookedupRecord1);

	Record lookedupRecord2 = segment->lookup(tid2);
	expectRecordsEqual(&record2, &lookedupRecord2);
}

TEST_F(SPSegmentTest, InsertAndLookupMatch) {
	size_t len = 100;
	char *data = this->allocateData(len);
	const Record record(len, data);
	TID tid = segment->insert(record);
	Record lookedupRecord = segment->lookup(tid);

	expectRecordsEqual(&record, &lookedupRecord);
}

TEST_F(SPSegmentTest, InsertAndLookupTwiceMatch) {
	SPSegmentTest_InsertAndLookupMatch_Test();
	SPSegmentTest_InsertAndLookupMatch_Test();
}

TEST_F(SPSegmentTest, InsertAndLookupMatchWithRedirect) {
	unsigned int length = BufferManager::DATA_SIZE_BYTE / 2;
	insertRecord(length);

	char *data = allocateData(length);
	const Record record(length, data);
	TID tid = segment->insert(record);
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
	EXPECT_EQ(tid1.pageId, tid2.pageId);
	EXPECT_EQ(tid1.slotOffset, tid2.slotOffset);
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

TEST_F(SPSegmentTest, UpdateWithRedirect) {
	unsigned int length = BufferManager::DATA_SIZE_BYTE / 2;
	insertRecord(length);

	testUpdate(100, length);
}

TEST_F(SPSegmentTest, UpdateTooMuch) {
	ASSERT_THROW(testUpdate(100, BufferManager::DATA_SIZE_BYTE /* header takes space away from this anyway */),
				 std::invalid_argument);
}
