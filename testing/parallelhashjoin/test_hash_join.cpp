//
// Created by daniel on 6/23/15.
//

#include "gtest/gtest.h"
#include "../../src/assignment6_parallelhashjoin/LinearProbingHT.hpp"
#include "../../src/assignment6_parallelhashjoin/ChainingLockingHT.hpp"


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
  inline bool insertLookup(ParallelHashJoin *parallelHashJoin) {
    for (uint64_t t = 0; t < 100; t++) {
      if (t % 2 == 0) {
        continue;
      }
      parallelHashJoin->insert(t, 1);
    }

    for (uint64_t t = 0; t < 100; t++) {
      if (t % 2 == 0) {
        ReturnValue *returnValue = parallelHashJoin->lookup(t);
        ASSERT_NE(returnValue, nullptr);
        ASSERT_EQ(1, returnValue->value);
      } else {
        ReturnValue *notFoundValue = parallelHashJoin->lookup(t);
        ASSERT_EQ(notFoundValue, nullptr);
      }
    }
  }

};

//Time-tests
TEST_F(ParallelHashJoinTest, ChainingWithLockingInsert) {
  LinearProbingHT linearProbingHT(testSize);
  insertLookup<LinearProbingHT, LinearProbingHT::Entry>(&linearProbingHT);
}

TEST_F(ParallelHashJoinTest, ChainingInsert) {
  ChainingLockingHT chainingLockingHT(testSize);
  insertLookup<ChainingLockingHT, ChainingLockingHT::ChainingLockingHT>(&chainingLockingHT);
}

TEST_F(ParallelHashJoinTest, LinearProbingInsert) {
  LinearProbingHT linearProbingHT(testSize);
  insertLookup<LinearProbingHT, LinearProbingHT::Entry>(&linearProbingHT);
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


