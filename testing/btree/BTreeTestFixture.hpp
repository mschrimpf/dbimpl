//
// Created by Martin on 01.06.2015.
//

#ifndef PROJECT_BTREETESTFIXTURE_HPP
#define PROJECT_BTREETESTFIXTURE_HPP

#include <gtest/gtest.h>
#include "../../src/database/buffer/BufferManager.hpp"
#include "../../src/database/btree/BTree.hpp"
#include "KeysAndComparators.hpp"

using ::testing::Test;

class BTreeTest : public Test {
private:
  BufferManager *bufferManager;
  static const uint64_t SEGMENT_ID = 1;
  MyCustomUInt64Cmp cmp;

public:
  BTree<uint64_t, MyCustomUInt64Cmp> *bTree;

  void SetUp() {
    bufferManager = new BufferManager(65536);
    bTree = new BTree<uint64_t, MyCustomUInt64Cmp>(*bufferManager, SEGMENT_ID, cmp);
    ASSERT_EQ(0, bTree->size());
  }

  void TearDown() {
    delete this->bufferManager;
    delete this->bTree;

    std::string filename = std::to_string(SEGMENT_ID);
    remove(filename.c_str());
  };

  void checkInserts(uint64_t from, uint64_t to) {
    for (uint64_t i = from;
         from <= to ? i <= to : true;
         from <= to ? i++ : i--) {
      bTree->insert(i, TID(i));
      if(from >= to && i == to) {
        break;
      }
    }
    uint64_t expectedSize = from <= to ? to - from + 1 : from - to + 1;
    ASSERT_EQ(expectedSize, bTree->size());

    for (uint64_t i = from;
         from <= to ? i <= to : true;
         from <= to ? i++ : i--) {
      TID lookupTid;
      ASSERT_TRUE(bTree->lookup(i, lookupTid));
      ASSERT_EQ(i, lookupTid.pageId);
      if(from >= to && i == to) {
        break;
      }
    }
  }
};

#endif //PROJECT_BTREETESTFIXTURE_HPP
