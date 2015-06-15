//===--- examples/Fibonacci/fibonacci.cpp - An example use of the JIT -----===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This small program provides an example of how to build quickly a small module
// with function Fibonacci and execute it with the JIT.
//
// The goal of this snippet is to create in the memory the LLVM module
// consisting of one function as follow:
//
//   int fib(int x) {
//     if(x<=2) return 1;
//     return fib(x-1)+fib(x-2);
//   }
//
// Once we have this, we compile the module via JIT, then execute the `fib'
// function and return result to a driver, i.e. to a "host program".
//
//===----------------------------------------------------------------------===//

#include <iostream>
#include "llvm/IR/Verifier.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/Interpreter.h"
//#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

struct FunctionContext {
  LLVMContext Context;
  std::unique_ptr<Module> Owner;
  Module *M;
  std::string errStr;
  ExecutionEngine *EE;

  FunctionContext () {
    InitializeNativeTarget();
    Owner = std::unique_ptr<Module>(new Module("test", Context));
    M = Owner.get();

    // Now we going to create JIT
    std::string errStr;
    EE = EngineBuilder(std::move(Owner))
                    .setErrorStr(&errStr)
                    .setEngineKind(EngineKind::Interpreter)
                    .create();
  }
};

static Function *CreateFibFunction(FunctionContext &functionContext) {
  // Create the fib function and insert it into module M. This function is said
  // to return an int and take an int parameter.
  Function *FibF =
          cast<Function>(functionContext.M->getOrInsertFunction("fib", Type::getInt32Ty(functionContext.Context),
                                                Type::getInt32Ty(functionContext.Context),
                                                (Type *)0));

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
  BasicBlock* RecurseBB = BasicBlock::Create(functionContext.Context, "recurse", FibF);

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
}


bool doFunction(FunctionContext &context, Function * function, std::vector<GenericValue> &params, GenericValue &value){
  if (!context.EE) {
    errs() <<  "Failed to construct ExecutionEngine: " << context.errStr
    << "\n";
    return false;
  }

  errs() << "verifying... ";
  if (verifyModule(*context.M)) {
    errs() << ": Error constructing function!\n";
    return false;
  }

  errs() << "OK\n";
  errs() << "We just constructed this LLVM module:\n\n---------\n" << *context.M;
  errs() << "---------\nstarting function with JIT...\n";

  value = context.EE->runFunction(function, params);
  return true;
}

APInt fibonacci(unsigned n){
  FunctionContext context;
  // We are about to create the "fib" function:
  Function *FibF = CreateFibFunction(context);
  std::vector<GenericValue> Args(1);
  Args[0].IntVal = APInt(32, n);
  GenericValue result;
  if (doFunction(context, FibF, Args, result)){
    return result.IntVal;
  }
  return APInt(0,0,false);
}

int main(int argc, char **argv) {
  APInt result = fibonacci(10);
  outs() << "fibonacci(10): " << result;
  return 0;
}

