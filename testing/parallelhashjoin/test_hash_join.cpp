//
// Created by daniel on 6/23/15.
//

#include "gtest/gtest.h"

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
};

//Time-tests
TEST_F(ParallelHashJoinTest, ChainingWithLockingTime) {

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
TEST_F(ParallelHashJoinTest, ChainingWithLockingGet) {

}

TEST_F(ParallelHashJoinTest, ChainingGet) {

}

TEST_F(ParallelHashJoinTest, LinearProbingGet) {

}

//insert parallel tests
TEST_F(ParallelHashJoinTest, ChainingWithLockingInsertParallel) {

}

TEST_F(ParallelHashJoinTest, ChainingTimeInsertParallel) {

}

TEST_F(ParallelHashJoinTest, LinearProbingInsertParallel) {

}


