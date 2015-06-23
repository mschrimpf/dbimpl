//
// Created by daniel on 6/15/15.
//

#include <sstream>
#include "Node.h"

std::string Node::string() {
  switch (operation) {
    case Operation::CONSTANTS:
      return std::to_string(constant);
    case Operation::VARIABLES:
      return variable;
    default:
      std::stringstream stream;
      stream << "(" << left->string() << " " << static_cast<char>(operation) << " " <<
      right->string() << ")";
      return stream.str();
  }
}

size_t Node::countVariables() {
  switch (operation) {
    case Operation::CONSTANTS:
      return 0;
    case Operation::VARIABLES:
      return 1;
    default:
      return left->countVariables() + right->countVariables();
  }
}

Value *Node::calculate(BasicBlock *basicBlock, FunctionContext &functionContext) {
  switch (operation) {
    case Operation::CONSTANTS: {
      return ConstantInt::get(Type::getInt64Ty(functionContext.Context), constant, true);
    };
    case Operation::VARIABLES: {
      Value *val = functionContext.iterator++;
      val->setName(variable);
      return val;
    }
    case Operation::ADDITION: {
      return BinaryOperator::CreateAdd(left->calculate(basicBlock, functionContext),
                                       right->calculate(basicBlock, functionContext), "Addition",
                                       basicBlock);
    }
    case Operation::SUBTRACTION: {
      return BinaryOperator::CreateSub(left->calculate(basicBlock, functionContext),
                                       right->calculate(basicBlock, functionContext), "Subtraction",
                                       basicBlock);
    }
    case Operation::MULTIPLICATION: {
      return BinaryOperator::CreateMul(left->calculate(basicBlock, functionContext),
                                       right->calculate(basicBlock, functionContext),
                                       "Multiplication",
                                       basicBlock);
    }
    case Operation::DIVISION: {
      return BinaryOperator::CreateSDiv(left->calculate(basicBlock, functionContext),
                                        right->calculate(basicBlock, functionContext), "Division",
                                        basicBlock);
    }
  }
}