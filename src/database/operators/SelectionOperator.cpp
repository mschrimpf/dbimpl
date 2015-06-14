#include "SelectionOperator.hpp"

void SelectionOperator::open() {
  input->open();
}

bool SelectionOperator::next() {
  while(input->next()) {
    output = input->getOutput();
    if(condition(output)) {
      return true;
    }
  }
  return false;
}

std::vector<Register *> SelectionOperator::getOutput() {
  return output;
}

void SelectionOperator::close() {
  input->close();
}
