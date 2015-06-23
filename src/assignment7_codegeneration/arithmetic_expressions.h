//
// Created by daniel on 6/15/15.
//

#ifndef PROJECT_ARITHMETIC_EXPRESSIONS_H
#define PROJECT_ARITHMETIC_EXPRESSIONS_H

#include <sstream>
#include "FunctionContext.h"
#include "Node.h"

using namespace llvm;


Function *CreateArithmeticFunction(FunctionContext &functionContext, Node &node);

int64_t calculate(std::vector<uint64_t> arguments, Node node);

#endif //PROJECT_ARITHMETIC_EXPRESSIONS_H
