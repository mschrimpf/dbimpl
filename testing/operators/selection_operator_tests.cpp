#include <stdint-gcc.h>
#include <gtest/gtest.h>
#include "../../src/database/operators/Register.hpp"
#include "../../src/database/operators/PrintOperator.hpp"
#include "DummyOperator.hpp"
#include "../../src/database/operators/SelectionOperator.hpp"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

namespace selection_operator_tests {
  Register reg1(1);
  Register reg2(2);
  Register reg3(3);

  DummyOperator init() {
    std::vector<std::vector<Register *>> tableVector;
    std::vector<Register *> rowVector;
    // 1
    rowVector.push_back(&reg1);
    tableVector.push_back(rowVector);
    rowVector.clear();
    // 2
    rowVector.push_back(&reg2);
    tableVector.push_back(rowVector);
    rowVector.clear();
    // 3
    rowVector.push_back(&reg3);
    tableVector.push_back(rowVector);
    rowVector.clear();

    return DummyOperator(tableVector);
  }

  bool accept_twos(std::vector<Register *> row) {
    return row[0]->getIntegerValue() == 2;
  }

  TEST(SelectionOperatorTest, SelectSingleColumnInteger) {
    DummyOperator input = init();
    SelectionOperator op(&input, accept_twos);

    op.open();
    ASSERT_TRUE(op.next());
    ASSERT_EQ(1, op.getOutput().size());
    ASSERT_EQ(2, (op.getOutput()[0])->getIntegerValue());
    ASSERT_FALSE(op.next());
  }

  bool accept_nothing(std::vector<Register *> row) {
    return false;
  }

  TEST(SelectionOperatorTest, SelectSingleColumnNothingInteger) {
    DummyOperator input = init();
    SelectionOperator op(&input, accept_nothing);

    op.open();
    ASSERT_FALSE(op.next());
  }
}
