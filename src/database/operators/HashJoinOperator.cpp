#include <stdio.h>
#include "HashJoinOperator.hpp"

void HashJoinOperator::open() {
  left->open();
  while (left->next()) {
    auto vec = left->getOutput();
    Register *&reg = vec[leftJoinIndex];
    hashMap[*reg] = vec;
  }

  right->open();
}

bool HashJoinOperator::next() {
  while (right->next()) {
    auto rightVec = right->getOutput();
    Register *&rightReg = rightVec[rightJoinIndex];
    std::unordered_map<Register, std::vector<Register *>>::iterator it = hashMap.find(*rightReg);
    if (it == hashMap.end()) {
      continue;
    }
    std::vector<Register *> leftVec = it->second;
    size_t outputSize = leftVec.size() + rightVec.size();
    output = std::vector<Register *>();
    output.reserve(outputSize);
    output.insert(output.end(), leftVec.begin(), leftVec.end());
    output.insert(output.end(), rightVec.begin(), rightVec.end());
    return true;
  }
  return false;
};

void HashJoinOperator::close() {
  left->close();
  right->close();
};

std::vector<Register *> HashJoinOperator::getOutput() {
  return output;
};