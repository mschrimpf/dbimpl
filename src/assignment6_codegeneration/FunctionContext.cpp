//
// Created by daniel on 6/15/15.
//

#include "FunctionContext.h"


// Method to execute a function where the result is saved in value
bool FunctionContext::execute(Function *function, std::vector<GenericValue> &params, GenericValue &returnValue) {

  if (!EE) {
    errs() << "Failed to construct ExecutionEngine: " << errStr
    << "\n";
    return false;
  }

  errs() << "verifying... ";
  if (verifyModule(*M)) {
    errs() << ": Error constructing function!\n";
    return false;
  }

  errs() << "OK\n";
  errs() << "We just constructed this LLVM module:\n\n---------\n" << *M;
  errs() << "---------\nstarting function with JIT...\n";

  returnValue = EE->runFunction(function, params);
  return true;
}