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
  ASSERT_EQ(0, bTree->size());
  bTree->insert(0, tid);
  ASSERT_EQ(1, bTree->size());

  TID tid2(0);
  ASSERT_TRUE(bTree->lookup(0, tid2));
  ASSERT_EQ(tid.pageId, tid2.pageId);
  ASSERT_EQ(tid.slotOffset, tid2.slotOffset);
}

TEST_F(BTreeTest, InsertRemoveSize) {
  TID tid(0);
  ASSERT_EQ(0, bTree->size());
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

TEST_F(BTreeTest, LookupRange) {
  for (unsigned id = 0; id < 20; ++id) {
    std::cout << "Adding Pair(" << id << ",0) to bTree";
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
}
