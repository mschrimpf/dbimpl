//
// Created by daniel on 6/9/15.
//

#include "PrintOperator.hpp"
#include "Register.hpp"

template<typename Type>
void PrintOperator<Type>::open() {
  if (isOpen) {
    std::__throw_invalid_argument("Operator has been opened before.");
  }
  output.clear();
  index = 0;
  isOpen = true;
}

template<typename Type>
std::string PrintOperator<Type>::toString(Type val) {
  return std::to_string(val);
}

template<typename Type>
bool PrintOperator<Type>::next() {
  if (!isOpen) {
    std::__throw_invalid_argument("Operator has to be opened before.");
  }
  if (index < relation.size()) {
    std::vector<Register<Type> *> vector = relation[index];
    for (auto reg : vector) {
      output.push_back(reg);
      outputStream << toString(reg->getValue());
    }
    index++;
    return true;
  } else {
    return false;
  }
};

template<typename Type>
void PrintOperator<Type>::close() {
  if (!isOpen) {
    std::__throw_invalid_argument("Operator has to be opened before.");
  }
  isOpen = false;
  output.clear();
};

template<typename Type>
std::vector<Register<Type> *>
PrintOperator<Type>::getOutput() {
  if (!isOpen) {
    std::__throw_invalid_argument("Operator has to be opened before.");
  }
  return output;
};
