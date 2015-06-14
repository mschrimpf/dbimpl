//
// Created by daniel on 6/9/15.
//

#ifndef PROJECT_PRINTOPERATOR_H
#define PROJECT_PRINTOPERATOR_H

#include "Operator.hpp"
#include <vector>

class PrintOperator : Operator {
private:
  uint64_t index;
  bool isOpen;
  std::vector<Register *> output;
  std::vector<std::vector<Register *>> relation;
  std::ostream &outputStream;

  std::string toString(Register &reg);

public:
  PrintOperator(std::vector<std::vector<Register *>> relation, std::ostream &outputStream)
          : relation(relation), outputStream(outputStream), isOpen(false) { };

  void open();

  bool next();

  void close();

  std::vector<Register *> getOutput();
};

#endif //PROJECT_PRINTOPERATOR_H
