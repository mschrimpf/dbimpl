#include <gtest/gtest.h>
#include "../../src/database/buffer/BufferManager.hpp"
#include "../../src/database/btree/BTree.hpp"
#include "KeysAndComparators.hpp"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

const unsigned int pageSize = BufferManager::DATA_SIZE_BYTE;

TEST(LeafUInt64Test, SizeLessEqualPageSize) {
  size_t size = sizeof(Leaf<uint64_t, MyCustomUInt64Cmp>);
  EXPECT_LE(size, pageSize);
}

TEST(LeafUInt64Test, SizeCloseToPageSize) {
  size_t size = sizeof(Leaf<uint64_t, MyCustomUInt64Cmp>);
  EXPECT_GE(size, pageSize - 100);
}

TEST(InnerNodeUInt64Test, SizeLessEqualPageSize) {
  size_t size = sizeof(InnerNode<uint64_t, MyCustomUInt64Cmp>);
  EXPECT_LE(size, pageSize);
}

TEST(InnerNodeUInt64Test, SizeCloseToPageSize) {
  size_t size = sizeof(InnerNode<uint64_t, MyCustomUInt64Cmp>);
  EXPECT_GE(size, pageSize - 100);
}