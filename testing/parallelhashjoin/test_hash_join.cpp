//
// Created by daniel on 6/23/15.
//

#include "gtest/gtest.h"
#include "../parallelhashjoin"

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

  void SetUp() {

  }

  void TearDown() {

  };

  template<typename ParallelHashJoin>
  inline bool insertLookup(ParallelHashJoin parallelHashJoin, uint64_t hashValue) {
    parallelHashJoin.insert(1, 1);
    Entry *entry = parallelHashJoin.lookup(1);
    ASSERT_EQ(1, entry->value);
  }
};

//Time-tests
TEST_F(ParallelHashJoinTest, ChainingWithLockingTime) {
  LinearProbingHT linearProbingHT;
}

TEST_F(ParallelHashJoinTest, ChainingTime) {

}

TEST_F(ParallelHashJoinTest, LinearProbingTime) {

}

//insert-tests
TEST_F(ParallelHashJoinTest, ChainingWithLockingInsert) {

}

TEST_F(ParallelHashJoinTest, ChainingInsert) {

}

TEST_F(ParallelHashJoinTest, LinearProbingInsert) {

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


