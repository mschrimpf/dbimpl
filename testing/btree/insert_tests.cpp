#include <gtest/gtest.h>
#include "BTreeTestFixture.hpp"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;


TEST_F(BTreeTest, InsertOne) {
  ASSERT_TRUE(bTree->insert(0, TID(0)));
  ASSERT_EQ(1, bTree->size());
}

TEST_F(BTreeTest, InsertTwo) {
  ASSERT_TRUE(bTree->insert(0, TID(0)));
  ASSERT_TRUE(bTree->insert(1, TID(1)));
  ASSERT_EQ(2, bTree->size());
}

TEST_F(BTreeTest, InsertMoreThanOneNodeCanHold) {
  int valuesToInsert = sizeof(Leaf<uint64_t, TID>::entries) / sizeof(Entry<uint64_t, TID>) + 1;
  for (int i = 0; i < valuesToInsert; ++i) {
    ASSERT_TRUE(bTree->insert(i, TID(i)));
  }
  ASSERT_EQ(valuesToInsert, bTree->size());
}
