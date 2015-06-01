#include "BTreeTestFixture.hpp"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

TEST_F(BTreeTest, InsertFindTest) {
  TID tid(0);
  ASSERT_EQ(0, bTree->size());
  ASSERT_TRUE(bTree->insert(0, tid));
  ASSERT_EQ(1, bTree->size());

  TID tid2(0);
  ASSERT_TRUE(bTree->lookup(0, tid2));
  ASSERT_EQ(tid.pageId, tid2.pageId);
  ASSERT_EQ(tid.slotOffset, tid2.slotOffset);
}

TEST_F(BTreeTest, EmptyAddAndRemoveTest) {
  TID tid(0);
  ASSERT_EQ(0, bTree->size());
  ASSERT_TRUE(bTree->insert(0, tid));
  ASSERT_EQ(1, bTree->size());
  ASSERT_TRUE(bTree->erase(0));
  ASSERT_EQ(0, bTree->size());
}

TEST_F(BTreeTest, InsertSameKeyTest) {
  uint64_t key = 0;
  TID tid(0);
  ASSERT_TRUE(bTree->insert(key, tid));
  ASSERT_FALSE(bTree->insert(key, tid));
  ASSERT_EQ(1, bTree->size());

  //TODO what is the behaviour here? should the insertion fail?
}

TEST_F(BTreeTest, TestLookupRange) {
  for (unsigned id = 0; id < 20; ++id){
    std::cout << "Adding Pair(" << id << ",0) to bTree";
    bTree->insert(id, TID(0, id));
  }
  std::vector<TID>::iterator iterator = bTree->lookupRange(0, 20);
  for (int cur = 0; cur < 20; cur++){
    TID tid = * iterator;
    ASSERT_EQ(tid.slotOffset, 0);
    ASSERT_EQ(tid.pageId, cur);
    iterator++;
  }
}


TEST_F(BTreeTest, TestBTreeVisualization) {
  bTree->visualize();
  for (unsigned id = 0; id < 20; ++id){
    std::cout << "Adding Pair(" << id << ",0) to bTree";
    bTree->insert(id, TID(0));
    bTree->visualize();
  }
}


TEST_F(BTreeTest, TestSplit) {
}

TEST_F(BTreeTest, TestMerge) {
}

TEST_F(BTreeTest, TestSplitUp) {
}

TEST_F(BTreeTest, TestMergeUp) {
}
