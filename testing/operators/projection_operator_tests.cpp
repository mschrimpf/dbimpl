#include <gtest/gtest.h>
#include "../../src/database/operators/Register.hpp"
#include "DummyOperator.hpp"
#include "../../src/database/operators/ProjectionOperator.hpp"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

namespace projection_operator_tests {
  Register reg1(1);
  Register reg2(2);
  Register reg3(3);

  DummyOperator init() {
    int rowSize = 5;
    std::vector<std::vector<Register *>> tableVector;
    std::vector<Register *> rowVector;
    // 1
    for (int i = 0; i < rowSize; i++) {
      rowVector.push_back(&reg1);
    }
    tableVector.push_back(rowVector);
    rowVector.clear();
    // 2
    for (int i = 0; i < rowSize; i++) {
      rowVector.push_back(&reg2);
    }
    tableVector.push_back(rowVector);
    rowVector.clear();
    // 3
    for (int i = 0; i < rowSize; i++) {
      rowVector.push_back(&reg3);
    }
    tableVector.push_back(rowVector);
    rowVector.clear();

    return DummyOperator(tableVector);
  }

  TEST(ProjectionOperatorTest, ProjectAll) {
    DummyOperator input = init();
    ProjectionOperator op(&input, std::vector<int>{0, 1, 2, 3, 4});

    op.open();
    for (int rowNum = 1; rowNum <= 3; rowNum++) {
      ASSERT_TRUE(op.next());
      ASSERT_EQ(5, op.getOutput().size());
      for (int i = 0; i < 5; i++) {
        ASSERT_EQ(rowNum, (op.getOutput()[i])->getIntegerValue());
      }
    }
    ASSERT_FALSE(op.next());
  }

  TEST(ProjectionOperatorTest, ProjectNothing) {
    DummyOperator input = init();
    ProjectionOperator op(&input, std::vector<int>{});

    op.open();
    for (int i = 1; i <= 3; i++) {
      ASSERT_TRUE(op.next());
      ASSERT_EQ(0, op.getOutput().size());
    }
    ASSERT_FALSE(op.next());
  }

  TEST(ProjectionOperatorTest, ProjectSecond) {
    DummyOperator input = init();
    ProjectionOperator op(&input, std::vector<int>{1});

    op.open();
    for (int rowNum = 1; rowNum <= 3; rowNum++) {
      ASSERT_TRUE(op.next());
      ASSERT_EQ(1, op.getOutput().size());
      ASSERT_EQ(rowNum, (op.getOutput()[0])->getIntegerValue());
    }
    ASSERT_FALSE(op.next());
  }
}
