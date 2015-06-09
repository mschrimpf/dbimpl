//
// Created by daniel on 6/9/15.
//

#include <stdint.h>
#include "Register.hpp"

template<typename Value>
void Register<Value>::setValue(Value val) {
  this->value = val;
}

template<typename Value>
Value Register<Value>::getValue() {
  return this->value;
}
