#ifndef PROJECT_SELECTIONOPERATOR_H
#define PROJECT_SELECTIONOPERATOR_H


#include "Operator.hpp"

class SelectionOperator : public Operator {
private:
  Operator *input;
  std::function<bool(std::vector<Register *> /* TODO: might have to adjust for attribute name */)> condition;
  std::vector<Register *> output;

public:
  SelectionOperator(Operator *input, std::function<bool(std::vector<Register *>)> condition)
      : input(input), condition(condition) { };

  void open();

  bool next();

  std::vector<Register *> getOutput();

  void close();
};


#endif //PROJECT_SELECTIONOPERATOR_H
