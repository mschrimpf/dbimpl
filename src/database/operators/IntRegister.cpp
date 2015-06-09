//
// Created by daniel on 6/9/15.
//

#include <stdint.h>
#include "Register.hpp"

template<>
size_t Register<uint64_t>::hashValue() const {
  return 0;
}

template<>
bool Register<uint64_t>::operator<(Register <uint64_t> val) {
  return this->getValue() < val.getValue();
}

template<>
bool Register<uint64_t>::operator==(Register <uint64_t> val) {
  return this->getValue() == val.getValue();
}
