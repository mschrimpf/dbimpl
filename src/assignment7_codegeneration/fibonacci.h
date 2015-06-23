//
// Created by daniel on 6/15/15.
//

#ifndef PROJECT_FIBONACCI_H
#define PROJECT_FIBONACCI_H

#include "FunctionContext.h"

Function *CreateFibFunction(FunctionContext & functionContext);

uint64_t fibonacci(unsigned n);

#endif //PROJECT_FIBONACCI_H
