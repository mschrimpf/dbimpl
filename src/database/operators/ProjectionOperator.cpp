#include "ProjectionOperator.hpp"

void ProjectionOperator::open() {
  input->open();
}

bool ProjectionOperator::next() {
  if (!input->next()) {
    return false;
  }
  auto inputVector = input->getOutput();
  output = std::vector<Register *>(projectedIndices.size());
  int index = 0;
  for (auto projectedIndex : projectedIndices) {
    output[index] = inputVector[projectedIndex];
    index++;
  }
  return true;
};

void ProjectionOperator::close() {
  input->close();
};

std::vector<Register *> ProjectionOperator::getOutput() {
  return output;
};