//
// Created by Martin on 11.05.2015.
//

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
	const uint64_t SEGMENT_ID = 1;

private:
	BufferManager *bufferManager;
	char *allocatedData = nullptr;
public:
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
		char *data = allocateData(initialLength);
		const Record record(initialLength, data);
		TID tid = segment->insert(record);

		char *updatedData = allocateData(updatedLength);
		const Record updateRecord(updatedLength, updatedData);
		segment->update(tid, updateRecord);
		Record lookedupRecord = segment->lookup(tid);

		EXPECT_EQ(updateRecord.getData(), lookedupRecord.getData());
		EXPECT_EQ(updateRecord.getLen(), lookedupRecord.getLen());
	}
};

TEST_F(SPSegmentTest, InsertAndLookupMatch) {
	size_t len = 100;
	char *data = allocateData(len);
	const Record record(len, data);
	TID tid = segment->insert(record);
	Record lookedupRecord = segment->lookup(tid);

	EXPECT_EQ(record.getData(), lookedupRecord.getData());
	EXPECT_EQ(record.getLen(), lookedupRecord.getLen());
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
