//
// Created by daniel on 6/15/15.
//

#ifndef PROJECT_FUNCTIONCONTEXT_H
#define PROJECT_FUNCTIONCONTEXT_H

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

class FunctionContext {
public:
  LLVMContext Context;
  std::unique_ptr<Module> Owner;
  Module *M;
  std::string errStr;
  ExecutionEngine *EE;
  Function::arg_iterator iterator;

  FunctionContext() {
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

  bool execute(Function *function, std::vector<GenericValue> &params, GenericValue &returnValue);
};


#endif //PROJECT_FUNCTIONCONTEXT_H
