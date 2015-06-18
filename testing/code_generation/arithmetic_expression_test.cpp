#include <stdint-gcc.h>
#include <gtest/gtest.h>
#include "../../src/assignment6_codegeneration/arithmetic_expressions.h"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

TEST(ArithmeticTest, Exercise6Example) {
  Node rootNode(Node::MULTIPLICATION);
  Node leftNode(Node::ADDITION);
  Node rightNode(Node::SUBTRACTION);
  rootNode.left = &leftNode;
  rootNode.right = &rightNode;

  Node leftLeftNode = Node(Node::VARIABLES, "v0");
  Node leftRightNode = Node(Node::VARIABLES, "v1");
  Node rightLeftNode = Node(Node::VARIABLES, "v2");
  Node rightRightNode = Node(Node::VARIABLES, "v3");
  leftNode.left = &leftLeftNode;
  leftNode.right = &leftRightNode;
  rightNode.left = &rightLeftNode;
  rightNode.right = &rightRightNode;

  std::vector<uint64_t> params(4);
  params[0] = 5;
  params[1] = 1;
  params[2] = 1;
  params[3] = 0;
  int64_t result = calculate(params, rootNode);
  ASSERT_EQ(6, result);
}

TEST(ArithmeticTest, AdditionConstantVariableTest) {
  Node rootNode(Node::ADDITION);
  Node leftNode(Node::VARIABLES, "a");
  Node rightNode(Node::CONSTANTS);
  rightNode.constant = 12;
  rootNode.left = &leftNode;
  rootNode.right = &rightNode;

  std::vector<uint64_t> params;
  params.reserve(1);
  params.push_back(1);
  int64_t result = calculate(params, rootNode);
  ASSERT_EQ(13, result);
}

TEST(ArithmeticTest, SubtractionTest) {
  Node rootNode(Node::SUBTRACTION);
  Node leftNode(Node::VARIABLES, "a");
  Node rightNode(Node::VARIABLES, "b");
  rootNode.left = &leftNode;
  rootNode.right = &rightNode;

  std::vector<uint64_t> params;
  params.reserve(2);
  params.push_back(5);
  params.push_back(1);
  int64_t result = calculate(params, rootNode);
  ASSERT_EQ(4, result);
}

TEST(ArithmeticTest, SubtractionDoubleNegativeTest) {
  Node rootNode(Node::SUBTRACTION);
  Node leftNode(Node::VARIABLES, "a");
  Node rightNode(Node::VARIABLES, "b");
  rootNode.left = &leftNode;
  rootNode.right = &rightNode;

  std::vector<uint64_t> params;
  params.reserve(2);
  params.push_back(-5);
  params.push_back(-1);
  int64_t result = calculate(params, rootNode);
  ASSERT_EQ(-4, result);
}

TEST(ArithmeticTest, AdditionTest) {
  Node rootNode(Node::ADDITION);
  Node leftNode(Node::VARIABLES, "a");
  Node rightNode(Node::VARIABLES, "b");
  rootNode.left = &leftNode;
  rootNode.right = &rightNode;

  std::vector<uint64_t> params;
  params.reserve(2);
  params.push_back(1);
  params.push_back(5);
  int64_t result = calculate(params, rootNode);
  ASSERT_EQ(6, result);
}

TEST(ArithmeticTest, AdditionNegativeTest) {
  Node rootNode(Node::ADDITION);
  Node leftNode(Node::VARIABLES, "a");
  Node rightNode(Node::VARIABLES, "b");
  rootNode.left = &leftNode;
  rootNode.right = &rightNode;

  std::vector<uint64_t> params;
  params.reserve(2);
  params.push_back(-5);
  params.push_back(1);
  int64_t result = calculate(params, rootNode);
  ASSERT_EQ(-4, result);
}

TEST(ArithmeticTest, MultiplicationTest) {
  Node rootNode(Node::MULTIPLICATION);
  Node leftNode(Node::VARIABLES, "a");
  Node rightNode(Node::VARIABLES, "b");
  rootNode.left = &leftNode;
  rootNode.right = &rightNode;

  std::vector<uint64_t> params;
  params.reserve(2);
  params.push_back(1);
  params.push_back(5);
  int64_t result = calculate(params, rootNode);
  ASSERT_EQ(5, result);
}

TEST(ArithmeticTest, MultiplicationTestNegative) {
  Node rootNode(Node::MULTIPLICATION);
  Node leftNode(Node::VARIABLES, "a");
  Node rightNode(Node::VARIABLES, "b");
  rootNode.left = &leftNode;
  rootNode.right = &rightNode;

  std::vector<uint64_t> params;
  params.reserve(2);
  params.push_back(1);
  params.push_back(-5);
  int64_t result = calculate(params, rootNode);
  ASSERT_EQ(-5, result);
}

TEST(ArithmeticTest, DivisionTestNegative) {
  Node rootNode(Node::DIVISION);
  Node leftNode(Node::VARIABLES, "a");
  Node rightNode(Node::VARIABLES, "b");
  rootNode.left = &leftNode;
  rootNode.right = &rightNode;

  std::vector<uint64_t> params;
  params.reserve(2);
  params.push_back(-10);
  params.push_back(2);
  int64_t result = calculate(params, rootNode);
  ASSERT_EQ(-5, result);
}

TEST(ArithmeticTest, DivisionTest) {
  Node rootNode(Node::DIVISION);
  Node leftNode(Node::VARIABLES, "a");
  Node rightNode(Node::VARIABLES, "b");
  rootNode.left = &leftNode;
  rootNode.right = &rightNode;

  std::vector<uint64_t> params;
  params.reserve(2);
  params.push_back(8);
  params.push_back(2);
  int64_t result = calculate(params, rootNode);
  ASSERT_EQ(4, result);
}