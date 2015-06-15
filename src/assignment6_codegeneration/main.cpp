#include <iostream>
#include "fibonacci.h"
#include "arithmetic_expressions.h"

int main(int argc, char **argv) {
  //fibonacci(2);
  Node rootNode(Operation::MULTIPLICATION);
  Node leftNode(Operation::ADDITION);
  Node rightNode(Operation::SUBTRACTION);
  rootNode.left = & leftNode;
  rootNode.right = & rightNode;

  Node leftLeftNode = Node(Operation::VARIABLES, "v0");
  Node leftRightNode = Node(Operation::VARIABLES, "v1");
  Node rightLeftNode = Node(Operation::VARIABLES, "v2");
  Node rightRightNode = Node(Operation::VARIABLES, "v3");
  leftNode.left = & leftLeftNode;
  leftNode.right = & leftRightNode;
  rightNode.left = & rightLeftNode;
  rightNode.right = & rightRightNode;

  std::cout << rootNode.string() << std::endl;
  return 0;
}

