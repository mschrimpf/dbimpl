#include <iostream>
#include "fibonacci.h"
#include "arithmetic_expressions.h"
#include "Node.h"


void testExercise6(){

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
  std::cout << result << std::endl;
}

void testSimpleAddConstants(){
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
  std::cout << result << std::endl;
}

void testSimpleAdd(){
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
  std::cout << result << std::endl;
}

int main(int argc, char **argv) {
  fibonacci(2);
  testExercise6();
  testSimpleAdd();
  testSimpleAddConstants();
  return 0;
}



