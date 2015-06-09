//
// Created by daniel on 6/9/15.
//

#include <stdint.h>
#include "Register.hpp"

template<>
bool Register<uint64_t>::operator<(Register<uint64_t> val) {
  return false;
}

template<>
bool Register<uint64_t>::operator==(Register<uint64_t> val) {
  return false;
}

template<>
void Register<uint64_t>::setValue(uint64_t val) {

}

template<>
uint64_t Register<uint64_t>::getValue() {
  return 0;
}

template<>
size_t Register<uint64_t>::hashValue() const {
  return 0;
}


