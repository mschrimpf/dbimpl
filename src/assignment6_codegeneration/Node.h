//
// Created by daniel on 6/15/15.
//

#ifndef PROJECT_NODE_H
#define PROJECT_NODE_H


#include <stdint.h>
#include <iostream>
#include "FunctionContext.h"

class Node {
public:
  enum Operation : char {
    ADDITION = '+', SUBTRACTION = '-', DIVISION = '/', MULTIPLICATION = '*', CONSTANTS = 'c', VARIABLES = 'v'
  };
  Node *left;
  Node *right;
  Operation operation;
  uint64_t constant;
  std::string variable;

  Node(Operation operation) : operation(operation) { }

  Node(Operation operation, const char * variable) : operation(operation), variable(variable) { }

  Node(Operation operation, int64_t constant) : operation(operation), constant(constant) { }

  std::string string();

  size_t countVariables();

  Value *calculate(BasicBlock *basicBlock, FunctionContext &functionContext);
};


#endif //PROJECT_NODE_H
