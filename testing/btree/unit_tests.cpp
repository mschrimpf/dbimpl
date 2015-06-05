#include "BTreeTestFixture.hpp"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

TEST_F(BTreeTest, InsertFindTid) {
  TID tid(0);
  bTree->insert(0, tid);
  ASSERT_EQ(1, bTree->size());

  TID resultTid(0);
  ASSERT_TRUE(bTree->lookup(0, resultTid));
  ASSERT_EQ(tid.pageId, resultTid.pageId);
  ASSERT_EQ(tid.slotOffset, resultTid.slotOffset);
}

TEST_F(BTreeTest, InsertFindTidTwice) {
  TID tid1(1);
  bTree->insert(1, tid1);
  ASSERT_EQ(1, bTree->size());

  TID tid2(2);
  bTree->insert(2, tid2);
  ASSERT_EQ(2, bTree->size());

  TID resultTid(0);
  ASSERT_TRUE(bTree->lookup(1, resultTid));
  ASSERT_EQ(tid1.pageId, resultTid.pageId);
  ASSERT_EQ(tid1.slotOffset, resultTid.slotOffset);

  ASSERT_TRUE(bTree->lookup(2, resultTid));
  ASSERT_EQ(tid2.pageId, resultTid.pageId);
  ASSERT_EQ(tid2.slotOffset, resultTid.slotOffset);
}

TEST_F(BTreeTest, InsertFindTidMultiple) {
  uint64_t keys[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  int count = 0;
  for (const uint64_t key : keys) {
    TID tid(key);
    bTree->insert(key, tid);
    count++;
    ASSERT_EQ(count, bTree->size());

    TID resultTid(0);
    ASSERT_TRUE(bTree->lookup(key, resultTid));
    ASSERT_EQ(tid.pageId, resultTid.pageId);
    ASSERT_EQ(tid.slotOffset, resultTid.slotOffset);
  }
}

TEST_F(BTreeTest, InsertFindTidScrambled) {
  uint64_t keys[] = {1, 9, 2, 8, 3, 7, 4, 6, 5};
  int count = 0;
  for (const uint64_t key : keys) {
    TID tid(key);
    bTree->insert(key, tid);
    count++;
    ASSERT_EQ(count, bTree->size());

    TID resultTid(0);
    ASSERT_TRUE(bTree->lookup(key, resultTid));
    ASSERT_EQ(tid.pageId, resultTid.pageId);
    ASSERT_EQ(tid.slotOffset, resultTid.slotOffset);
  }
}

TEST_F(BTreeTest, InsertToSplitFind) {
  uint64_t valuesToInsert = maxLeafCapacity + 1;
  checkInserts(0, valuesToInsert);
}

TEST_F(BTreeTest, InsertToSplitFindReverse) {
  uint64_t valuesToInsert = maxLeafCapacity + 1;
  checkInserts(valuesToInsert, 0);
}

TEST_F(BTreeTest, InsertTwoSplitsFind) {
  uint64_t valuesToInsert = (maxLeafCapacity + 1) * 2;
  for (uint64_t i = 0; i < valuesToInsert; ++i) {
    bTree->insert(i, TID(i));
  }
  ASSERT_EQ(valuesToInsert, bTree->size());

  for (uint64_t i = 0; i < valuesToInsert; ++i) {
    TID lookupTid;
    ASSERT_TRUE(bTree->lookup(i, lookupTid));
    ASSERT_EQ(i, lookupTid.pageId);
  }
}

TEST_F(BTreeTest, InsertNodeSplitFind) {
  uint64_t valuesToInsert = maxLeafCapacity * maxNodeCapacity + 1;
  checkInserts(0, valuesToInsert);
}

TEST_F(BTreeTest, InsertRemoveSize) {
  TID tid(0);
  bTree->insert(0, tid);
  ASSERT_EQ(1, bTree->size());
  bTree->erase(0);
  ASSERT_EQ(0, bTree->size());
}

TEST_F(BTreeTest, InsertSameKeyThrowsAndSizeStaysTheSame) {
  uint64_t key = 0;
  TID tid(0);
  bTree->insert(key, tid);
  ASSERT_THROW(bTree->insert(key, tid), std::invalid_argument);
  ASSERT_EQ(1, bTree->size());
}

/*TEST_F(BTreeTest, LookupRange) {
  for (unsigned id = 0; id < 20; ++id) {
    bTree->insert(id, TID(0, id));
  }
  std::vector<TID> vec = bTree->lookupRange(0, 20);
  int cur = 0;
  for (std::vector<TID>::iterator it = vec.begin(); it != vec.end(); ++it) {
    TID tid = *it;
    ASSERT_EQ(tid.slotOffset, 0);
    ASSERT_EQ(tid.pageId, cur);
    cur++;
  }
}*/
