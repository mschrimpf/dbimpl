#include "PrintOperator.hpp"
#include "Register.hpp"

void PrintOperator::open() {
  input->open();
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
