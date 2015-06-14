#include <stdint-gcc.h>
#include <gtest/gtest.h>
#include "../../src/database/operators/Register.hpp"
#include "../../src/database/operators/PrintOperator.hpp"
#include "DummyOperator.hpp"
#include "../../src/database/operators/HashJoinOperator.hpp"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

class HashJoinOperatorTest : public Test {
private:
  std::vector<Register> registers;

public:
  const int max_vector_size = 10;

  void SetUp() {
    // set the vector size - the vector must not resize since that would invalidate all references.
    registers.reserve(max_vector_size);
  }

  void TearDown() {
    registers.clear();
  };

  template<typename T>
  DummyOperator init(std::initializer_list<T> values, int columns = 1) {
    if (values.size() > max_vector_size + registers.size()) {
      throw std::invalid_argument("initializer_list too big, vector would have to resize");
    }

    std::vector<std::vector<Register *>> tableVector;
    std::vector<Register *> rowVector;
    for (auto value: values) {
      for (int c = 0; c < columns; c++) {
        Register reg(value);
        registers.push_back(reg);
        Register &vecReg = registers.back(); // use vector-reference, local register goes out of scope
        rowVector.push_back(&vecReg);
      }
      tableVector.push_back(rowVector);
      rowVector.clear();
    }

    return DummyOperator(tableVector);
  }
};

TEST_F(HashJoinOperatorTest, PairwiseEqual) {
  DummyOperator left = init({1, 2, 3});
  DummyOperator right = init({1, 2, 3});

  HashJoinOperator op(&left, &right, 0, 0);

  op.open();
  for (int i = 1; i <= 3; i++) {
    ASSERT_TRUE(op.next());
    ASSERT_EQ(2, op.getOutput().size());
    ASSERT_EQ(i, (op.getOutput()[0])->getIntegerValue());
    ASSERT_EQ(i, (op.getOutput()[1])->getIntegerValue());
  }
  ASSERT_FALSE(op.next());
}

TEST_F(HashJoinOperatorTest, PairwiseEqualDifferentIndex) {
  DummyOperator left = init({1, 2, 3}, 2);
  DummyOperator right = init({1, 2, 3});

  HashJoinOperator op(&left, &right, 1, 0);

  op.open();
  for (int rowNum = 1; rowNum <= 3; rowNum++) {
    ASSERT_TRUE(op.next());
    ASSERT_EQ(3, op.getOutput().size());
    ASSERT_EQ(rowNum, (op.getOutput()[0])->getIntegerValue());
    ASSERT_EQ(rowNum, (op.getOutput()[1])->getIntegerValue());
    ASSERT_EQ(rowNum, (op.getOutput()[2])->getIntegerValue());
  }
  ASSERT_FALSE(op.next());
}

TEST_F(HashJoinOperatorTest, SingleMatchRight) {
  DummyOperator left = init({1, 2, 3});
  DummyOperator right = init({2});
  HashJoinOperator op(&left, &right, 0, 0);

  op.open();
  ASSERT_TRUE(op.next());
  ASSERT_EQ(2, op.getOutput().size());
  ASSERT_EQ(2, (op.getOutput()[0])->getIntegerValue());
  ASSERT_EQ(2, (op.getOutput()[1])->getIntegerValue());
  ASSERT_FALSE(op.next());
}

TEST_F(HashJoinOperatorTest, SingleMatchLeft) {
  DummyOperator left = init({2});
  DummyOperator right = init({1, 2, 3});
  HashJoinOperator op(&left, &right, 0, 0);

  op.open();
  ASSERT_TRUE(op.next());
  ASSERT_EQ(2, op.getOutput().size());
  ASSERT_EQ(2, (op.getOutput()[0])->getIntegerValue());
  ASSERT_EQ(2, (op.getOutput()[1])->getIntegerValue());
  ASSERT_FALSE(op.next());
}

TEST_F(HashJoinOperatorTest, NoMatch) {
  DummyOperator left = init({1, 2, 3});
  DummyOperator right = init({4, 5, 6});
  HashJoinOperator op(&left, &right, 0, 0);

  op.open();
  ASSERT_FALSE(op.next());
}