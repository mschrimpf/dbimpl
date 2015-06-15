//
// Created by daniel on 6/15/15.
//

#include <stdint.h>
#include <sstream>
#include "arithmetic_expressions.h"

Function *CreateArithmeticFunction(FunctionContext &functionContext, Node &rootNode) {

  // create signature
  size_t numberOfVariables = rootNode.countVariables();
  Type *returnValueType = Type::getInt64Ty(functionContext.Context);
  std::vector<Type *> params(numberOfVariables, Type::getInt64Ty(
          functionContext.Context)); // create a function with numberOfVariables * int64 parameters
  ArrayRef<Type *> parameters(params);
  bool isVarArg = false;
  FunctionType *functionType = FunctionType::get(returnValueType, parameters,
                                                 isVarArg); // result is int64, params are list<int64>, varargs is false
  Function *ArithmeticF = cast<Function>(
          functionContext.M->getOrInsertFunction("arithm", functionType));
  functionContext.iterator = ArithmeticF->arg_begin();

  // Adds basic block to the function
  BasicBlock *basicBlock = BasicBlock::Create(functionContext.Context, "EntryBlock", ArithmeticF);
  Value *Sum = rootNode.calculate(basicBlock, functionContext);
  ReturnInst::Create(functionContext.Context, Sum, basicBlock);

  return ArithmeticF;
}

int64_t calculate(std::vector<uint64_t> arguments, Node node) {
  FunctionContext context;
  // We are about to create the "fib" function:
  Function *Arithm = CreateArithmeticFunction(context, node);
  std::vector<GenericValue> Args(arguments.size());
  std::stringstream stream;
  for (int i = 0; i < arguments.size(); ++i) {
    Args[i].IntVal = APInt(64, arguments[i], true);
    stream << arguments[i] << ",";
  }
  GenericValue result;
  if (context.execute(Arithm, Args, result)) {
    int64_t value = result.IntVal.getLimitedValue();
    outs() << "calculate(" << node.string() << ") with values: " << stream.str() << "\n";
    return value;
  }
  return 0;
}