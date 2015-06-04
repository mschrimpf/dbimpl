#include "BTreeTestFixture.hpp"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

const MyCustomUInt64Cmp smaller = MyCustomUInt64Cmp();

TEST(EntriesHelperTest, EmptyInnerNodeFindInsertPosition) {
  Entry<uint64_t, uint64_t> entries[0];
  uint64_t key = 2;
  unsigned long position = EntriesHelper::findInsertPosition(entries, key, 1, 1, smaller);
  ASSERT_EQ(1, position);
}

TEST(EntriesHelperTest, EmptyLeafFindInsertPosition) {
  Entry<uint64_t, TID> entries[0];
  uint64_t key = 1;
  unsigned long position = EntriesHelper::findInsertPosition(entries, key, 0, 0, smaller);
  ASSERT_EQ(0, position);
}

TEST(EntriesHelperTest, Leaf1KeyFindInsertPositionLeft) {
  Entry<uint64_t, TID> entries[1];
  entries[0].key = 1;
  uint64_t key = 0;
  unsigned long position = EntriesHelper::findInsertPosition(entries, key, 0, 1, smaller);
  ASSERT_EQ(0, position);
}

TEST(EntriesHelperTest, Leaf1KeyFindInsertPositionRight) {
  Entry<uint64_t, TID> entries[1];
  entries[0].key = 1;
  uint64_t key = 2;
  unsigned long position = EntriesHelper::findInsertPosition(entries, key, 0, 1, smaller);
  ASSERT_EQ(1, position);
}

TEST(EntriesHelperTest, Leaf1KeyFindInsertPositionBetween) {
  Entry<uint64_t, TID> entries[3];
  entries[0].key = 1;
  entries[2].key = 3;
  uint64_t key = 2;
  unsigned long position = EntriesHelper::findInsertPosition(entries, key, 0, 2, smaller);
  ASSERT_EQ(1, position);
}

TEST(EntriesHelperTest, MoveRight) {
  Entry<uint64_t, TID> entries[3];
  entries[0].key = 3;
  entries[1].key = 4;
  EntriesHelper::moveEntriesToRight(entries, 0, 2);
  ASSERT_EQ(3, entries[1].key);
  ASSERT_EQ(4, entries[2].key);
}

TEST(EntriesHelperTest, MoveLeft) {
  Entry<uint64_t, TID> entries[3];
  entries[1].key = 2;
  entries[2].key = 3;
  EntriesHelper::moveEntriesToLeft(entries, 1, 3);
  ASSERT_EQ(2, entries[0].key);
  ASSERT_EQ(3, entries[1].key);
}

TEST(EntriesHelperTest, FindDirectionValueSameKey) {
  Entry<uint64_t, uint64_t> entries[2];
  entries[0].value = 0;
  entries[1].key = 3;
  entries[1].value = 2;
  uint64_t key = 3;
  uint64_t value = EntriesHelper::findDirectionValue(entries, key, 1, 1, smaller);
  ASSERT_EQ(entries[1].value, value);
}
