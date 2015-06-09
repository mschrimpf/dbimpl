//
// Created by daniel on 6/9/15.
//

#include "PrintOperator.hpp"
#include "Register.hpp"

void PrintOperator::open() {
  if (isOpen) {
    std::__throw_invalid_argument("Operator has been opened before.");
  }
  output.clear();
  index = 0;
  isOpen = true;
}


std::string PrintOperator::toString(Register &reg) {
  switch (reg.currentType) {
    case Register::type::STRING:
      return reg.getStringValue();
    case Register::type::INTEGER:
      return std::to_string(reg.getIntegerValue());
  }
}

bool PrintOperator::next() {
  if (!isOpen) {
    std::__throw_invalid_argument("Operator has to be opened before.");
  }
  if (index < relation.size()) {
    std::vector<Register *> vector = relation[index];
    for (auto reg : vector) {
      output.push_back(reg);
      outputStream << toString(reg);
    }
    index++;
    return true;
  } else {
    return false;
  }
};

void PrintOperator::close() {
  if (!isOpen) {
    std::__throw_invalid_argument("Operator has to be opened before.");
  }
  isOpen = false;
  output.clear();
};

std::vector<Register *> PrintOperator::getOutput() {
  if (!isOpen) {
    std::__throw_invalid_argument("Operator has to be opened before.");
  }
  return output;
};
