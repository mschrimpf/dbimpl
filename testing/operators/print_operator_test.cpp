#include <stdint-gcc.h>
#include <gtest/gtest.h>
#include "../../src/database/operators/Register.hpp"
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
  Register<uint64_t> reg(1337);
  std::vector<std::vector<Register<uint64_t> *>> tableVector;
  std::vector<Register<uint64_t> *> rowVector;
  rowVector.push_back(&reg);
  tableVector.push_back(rowVector);

  std::stringstream stream;
  PrintOperator<uint64_t> printOperator(tableVector, stream);

  printOperator.open();
  ASSERT_TRUE(printOperator.next());
  ASSERT_EQ(1, printOperator.getOutput().size());
  ASSERT_EQ(1337, (printOperator.getOutput()[0])->getValue());
  ASSERT_EQ("1337", stream.str());
}
