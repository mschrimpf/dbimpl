//
// Created by daniel on 6/9/15.
//

#ifndef PROJECT_PRINTOPERATOR_H
#define PROJECT_PRINTOPERATOR_H

#include "Operator.hpp"
#include <vector>

/**
 * Prints when #next() is called.
 */
class PrintOperator : public Operator {
private:
  Operator *input;
  std::ostream &outputStream;
  std::vector<Register *> output;

  std::string toString(Register &reg);

public:
  PrintOperator(Operator *input, std::ostream &outputStream)
      : input(input), outputStream(outputStream) { };

  void open();

  bool next();

  void close();

  std::vector<Register *> getOutput();
};

#endif //PROJECT_PRINTOPERATOR_H
