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

#include "SPSegmentTest.hpp"

class SPSegmentIterationTest : public SPSegmentTest {
public:
  void test(int numEntries, size_t len, int expectedNumPages) {
    std::vector<Record *> insertedRecords;
    for (int i = 0; i < numEntries; i++) {
      Record *record = newRecord(len);
      insertedRecords.push_back(record);
      segment->insert(*record);
    }

    auto pageIter = segment->begin();
    auto pageIterEnd = segment->end();
    int i = 0;
    for (; i < numEntries; i++, ++pageIter) {
      ASSERT_NE(pageIterEnd, pageIter);
      const Record *insertedRecord = insertedRecords[i];
      ASSERT_TRUE(SPSegmentTest::RecordsMatch(*insertedRecord, *pageIter));
    }

    EXPECT_EQ(pageIterEnd, pageIter);
  }
};

TEST_F(SPSegmentIterationTest, IterateAfterInsertsInSinglePage) {
  size_t len = 100;
  int numEntries = 10;
  test(numEntries, len, 1);
}

TEST_F(SPSegmentIterationTest, IterateAfterInsertsOnTwoPages) {
  size_t len = 100;
  int numEntries = (BufferManager::DATA_SIZE_BYTE - sizeof(SlottedPage::header)) / (len - sizeof(Slot)) + 1;
  test(numEntries, len, 2);
}
