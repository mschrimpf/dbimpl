
#ifndef TEST_DUMMYOPERATOR_H
#define TEST_DUMMYOPERATOR_H

#include <vector>
#include "../../src/database/operators/Operator.hpp"

class DummyOperator : public Operator {
private:
  int index;
  std::vector<std::vector<Register *>> input;

public:
  DummyOperator(std::vector<std::vector<Register *>> input) : input(input) { };

  void open();

  bool next();

  void close();

  std::vector<Register *> getOutput();
};

#endif //TEST_DUMMYOPERATOR_H
