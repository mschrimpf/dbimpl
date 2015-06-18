#include <gtest/gtest.h>
#include "../../src/database/operators/Register.hpp"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

TEST(RegisterTest, SaveRetrieveInteger) {
  Register reg(1337);
  ASSERT_EQ(1337, reg.getIntegerValue());
}

TEST(RegisterTest, SaveRetrieveString) {
  std::string str("hello world");
  Register reg(str);
  ASSERT_EQ(str, reg.getStringValue());
}
