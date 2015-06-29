//
// Created by daniel on 6/23/15.
//

#include "gtest/gtest.h"
#include "../../src/assignment6_parallelhashjoin/LinearProbingHT.cpp"
#include "../../src/assignment6_parallelhashjoin/ChainingLockingHT.cpp"
#include "../../src/assignment6_parallelhashjoin/ChainingHT.cpp"


using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

class ParallelHashJoinTest : public Test {

private:
public:

  const uint64_t testSize = 100;

  void SetUp() {

  }

  void TearDown() {

  };

  template<typename ParallelHashJoin, typename Entry>
  inline void insertLookupOnce(ParallelHashJoin *parallelHashJoin) {
    parallelHashJoin->insert(1, 1);

    Entry *entry = parallelHashJoin->lookup(1);
    ASSERT_NE(nullptr, entry);
    ASSERT_EQ(1, entry->value);
  }

  template<typename ParallelHashJoin, typename Entry>
  inline void insertLookup50(ParallelHashJoin *parallelHashJoin) {
    for (uint64_t t = 0; t < 100; t++) {
      if (t % 2 == 0) {
        continue;
      }
      parallelHashJoin->insert(t, 1);
    }

    for (uint64_t t = 0; t < 100; t++) {
      if (t % 2 == 0) {
        Entry *notFoundValue = parallelHashJoin->lookup(t);
        ASSERT_EQ(nullptr, notFoundValue);
      } else {
        Entry *returnValue = parallelHashJoin->lookup(t);
        ASSERT_NE(nullptr, returnValue);
        ASSERT_EQ(1, returnValue->value);
      }
    }
  }

};

// Insert
TEST_F(ParallelHashJoinTest, ChainingInsertOnce) {
  ChainingHT hashTable(testSize);
  insertLookupOnce<ChainingHT, ChainingHT::Entry>(&hashTable);
}


TEST_F(ParallelHashJoinTest, ChainingWithLockingInsert50) {
  ChainingLockingHT hashTable(testSize);
  insertLookup50<ChainingLockingHT, ChainingLockingHT::Entry>(&hashTable);
}

TEST_F(ParallelHashJoinTest, LinearProbingInsert50) {
  LinearProbingHT hashTable(testSize);
  insertLookup50<LinearProbingHT, LinearProbingHT::Entry>(&hashTable);
}

TEST_F(ParallelHashJoinTest, ChainingInsert50) {
  ChainingHT hashTable(testSize);
  insertLookup50<ChainingHT, ChainingHT::Entry>(&hashTable);
}

// Time
TEST_F(ParallelHashJoinTest, ChainingWithLockingInserTime) {

}

TEST_F(ParallelHashJoinTest, ChainingInsertTime) {

}

TEST_F(ParallelHashJoinTest, LinearProbingInsertTime) {

}

// Lookup
TEST_F(ParallelHashJoinTest, ChainingWithLockingLookup) {

}

TEST_F(ParallelHashJoinTest, ChainingLookup) {

}

TEST_F(ParallelHashJoinTest, LinearProbingLookup) {

}

// Insert parallel
TEST_F(ParallelHashJoinTest, ChainingWithLockingInsertParallel) {

}

TEST_F(ParallelHashJoinTest, ChainingTimeInsertParallel) {

}

TEST_F(ParallelHashJoinTest, LinearProbingInsertParallel) {

}


