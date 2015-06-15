//
// Created by daniel on 6/15/15.
//

#ifndef PROJECT_ARITHMETIC_EXPRESSIONS_H
#define PROJECT_ARITHMETIC_EXPRESSIONS_H

#include <sstream>
#include "FunctionContext.h"

using namespace llvm;

enum Operation : char {
  ADDITION = '+', SUBTRACTION = '-', DIVISION = '/', MULTIPLICATION = '*', CONSTANTS = 'c', VARIABLES = 'v'
};

struct Node {
  Node *left;
  Node *right;
  Operation operation;
  int64_t constant;
  std::string variable;

  Node(Operation operation) : operation(operation) { }

  Node(Operation operation, const char *variable) : operation(operation), variable(variable) { }

  Node(Operation operation, int64_t constant) : operation(operation), constant(constant) { }

  std::string string() {
    switch (operation) {
      case CONSTANTS:
        return std::to_string(constant);
      case VARIABLES:
        return variable;
      default:
        std::stringstream stream;
        stream << "(" << left->string() << " " << static_cast<char>(operation) << " " <<
        right->string() << ")";
        return stream.str();
    }
  }
};


Function *CreateArithmeticFunction(FunctionContext &functionContext, Node node);

uint64_t calculate(std::vector<uint64_t> arguments, Node node);

#endif //PROJECT_ARITHMETIC_EXPRESSIONS_H
