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

  template<typename ParallelHashJoin, typename ReturnValue>
  inline void insertLookup(ParallelHashJoin *parallelHashJoin) {
    for (uint64_t t = 0; t < 100; t++) {
      if (t % 2 == 0) {
        continue;
      }
      parallelHashJoin->insert(t, 1);
    }

    for (uint64_t t = 0; t < 100; t++) {
      if (t % 2 == 0) {
        ReturnValue *returnValue = parallelHashJoin->lookup(t);
        ASSERT_NE(nullptr, returnValue);
        uint64_t val = returnValue->value;
        ASSERT_EQ(1, val);
      } else {
        ReturnValue *notFoundValue = parallelHashJoin->lookup(t);
        ASSERT_EQ(nullptr, notFoundValue);
      }
    }
  }

};

//Time-tests
TEST_F(ParallelHashJoinTest, ChainingWithLockingInsert) {
  ChainingLockingHT hashTable(testSize);
  insertLookup<ChainingLockingHT, ChainingLockingHT::Entry>(&hashTable);
}

TEST_F(ParallelHashJoinTest, ChainingInsert) {
  ChainingHT hashTable(testSize);
  insertLookup<ChainingHT, ChainingHT::Entry>(&hashTable);
}

TEST_F(ParallelHashJoinTest, LinearProbingInsert) {
  LinearProbingHT hashTable(testSize);
  insertLookup<LinearProbingHT, LinearProbingHT::Entry>(&hashTable);
}

//time-tests
TEST_F(ParallelHashJoinTest, ChainingWithLockingInserTime) {

}

TEST_F(ParallelHashJoinTest, ChainingInsertTime) {

}

TEST_F(ParallelHashJoinTest, LinearProbingInsertTime) {

}

//get-tests
TEST_F(ParallelHashJoinTest, ChainingWithLockingLookup) {

}

TEST_F(ParallelHashJoinTest, ChainingLookup) {

}

TEST_F(ParallelHashJoinTest, LinearProbingLookup) {

}

//insert parallel tests
TEST_F(ParallelHashJoinTest, ChainingWithLockingInsertParallel) {

}

TEST_F(ParallelHashJoinTest, ChainingTimeInsertParallel) {

}

TEST_F(ParallelHashJoinTest, LinearProbingInsertParallel) {

}


