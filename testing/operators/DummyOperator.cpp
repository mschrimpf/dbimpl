#include "DummyOperator.hpp"

void DummyOperator::open() {
  index = 0;
}

bool DummyOperator::next() {
  return index < input.size();
};

void DummyOperator::close() {
};

std::vector<Register *> DummyOperator::getOutput() {
  return input[index++];
};
