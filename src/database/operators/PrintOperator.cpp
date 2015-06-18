#include "PrintOperator.hpp"
#include "Register.hpp"

void PrintOperator::open() {
  input->open();
}

std::string PrintOperator::toString(Register &reg) {
  switch (reg.type) {
    case Register::TYPE::STRING:
      return reg.getStringValue();
    case Register::TYPE::INTEGER:
      return std::to_string(reg.getIntegerValue());
  }
}

bool PrintOperator::next() {
  if(! input->next()) {
    return false;
  }
  output = input->getOutput();
  for (auto reg : output) {
    outputStream << toString(*reg);
  }
  return true;
};

void PrintOperator::close() {
  input->close();
};

std::vector<Register *> PrintOperator::getOutput() {
  return output;
};
