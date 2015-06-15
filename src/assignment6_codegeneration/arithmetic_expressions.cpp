//
// Created by daniel on 6/15/15.
//

#include <stdint.h>
#include <sstream>
#include "FunctionContext.h"
#include "arithmetic_expressions.h"

Function *CreateArithmeticFunction(FunctionContext & functionContext, const char * expression){
/*// Create the fib function and insert it into module M. This function is said
  // to return an int and take an int parameter.
  Function *FibF =
          cast<Function>(functionContext.M->getOrInsertFunction("fib", Type::getInt32Ty(functionContext.Context),
                                                                Type::getInt32Ty(functionContext.Context),
                                                                (Type *) 0));

  // Add a basic block to the function.
  BasicBlock *BB = BasicBlock::Create(functionContext.Context, "EntryBlock", FibF);

  // Get pointers to the constants.
  Value *One = ConstantInt::get(Type::getInt32Ty(functionContext.Context), 1);
  Value *Two = ConstantInt::get(Type::getInt32Ty(functionContext.Context), 2);

  // Get pointer to the integer argument of the add1 function...
  Argument *ArgX = FibF->arg_begin();   // Get the arg.
  ArgX->setName("AnArg");            // Give it a nice symbolic name for fun.

  // Create the true_block.
  BasicBlock *RetBB = BasicBlock::Create(functionContext.Context, "return", FibF);
  // Create an exit block.
  BasicBlock *RecurseBB = BasicBlock::Create(functionContext.Context, "recurse", FibF);

  // Create the "if (arg <= 2) goto exitbb"
  Value *CondInst = new ICmpInst(*BB, ICmpInst::ICMP_SLE, ArgX, Two, "cond");
  BranchInst::Create(RetBB, RecurseBB, CondInst, BB);

  // Create: ret int 1
  ReturnInst::Create(functionContext.Context, One, RetBB);

  // create fib(x-1)
  Value *Sub = BinaryOperator::CreateSub(ArgX, One, "arg", RecurseBB);
  CallInst *CallFibX1 = CallInst::Create(FibF, Sub, "fibx1", RecurseBB);
  CallFibX1->setTailCall();

  // create fib(x-2)
  Sub = BinaryOperator::CreateSub(ArgX, Two, "arg", RecurseBB);
  CallInst *CallFibX2 = CallInst::Create(FibF, Sub, "fibx2", RecurseBB);
  CallFibX2->setTailCall();


  // fib(x-1)+fib(x-2)
  Value *Sum = BinaryOperator::CreateAdd(CallFibX1, CallFibX2,
                                         "addresult", RecurseBB);

  // Create the return instruction and add it to the basic block
  ReturnInst::Create(functionContext.Context, Sum, RecurseBB);

  return FibF;
  */
}

uint64_t calculate(Node node, std::vector<uint64_t> arguments){
  /*FunctionContext context;
  // We are about to create the "fib" function:
  Function *FibF = CreateArithmeticFunction(context, node);
  std::vector <GenericValue> Args(arguments.size());
  std::stringstream stream;
  for (int i = 0; i < arguments.size(); ++i){
    Args[i].IntVal = APInt(64, arguments[i]);
    stream << arguments[i] << ",";
  }
  GenericValue result;
  if (context.execute(FibF, Args, result)) {
    uint64_t value = result.IntVal.getLimitedValue();
    outs() << "calculate(" << node.string() << ") with values: " << stream.str() <<  "\n";
    return value;
  }
  return 0;*/
}