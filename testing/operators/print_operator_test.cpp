#include <stdint-gcc.h>
#include <gtest/gtest.h>
#include "../../src/database/operators/Register.hpp"
#include "DummyOperator.hpp"
#include "../../src/database/operators/PrintOperator.hpp"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

TEST(PrintOperatorTest, PrintSingleIntRegister) {
  Register reg(1337);
  std::vector<std::vector<Register *>> tableVector;
  std::vector<Register *> rowVector;
  rowVector.push_back(&reg);
  tableVector.push_back(rowVector);
  DummyOperator dummyOperator(tableVector);

  std::stringstream stream;
  PrintOperator printOperator(&dummyOperator, stream);

  printOperator.open();
  ASSERT_TRUE(printOperator.next());
  ASSERT_EQ(1, printOperator.getOutput().size());
  ASSERT_EQ(1337, (printOperator.getOutput()[0])->getIntegerValue());
  ASSERT_EQ("1337", stream.str());
}

TEST(PrintOperatorTest, PrintCharRegister) {
  Register reg("Hello World");
  std::vector<std::vector<Register *>> tableVector;
  std::vector<Register *> rowVector;
  rowVector.push_back(&reg);
  tableVector.push_back(rowVector);
  DummyOperator dummyOperator(tableVector);

  std::stringstream stream;
  PrintOperator printOperator(&dummyOperator, stream);

  printOperator.open();
  ASSERT_TRUE(printOperator.next());
  ASSERT_EQ(1, printOperator.getOutput().size());
  ASSERT_EQ("Hello World", (printOperator.getOutput()[0])->getStringValue());
  ASSERT_EQ("Hello World", stream.str());
}
