#ifndef PROJECT_PROJECTIONOPERATOR_H
#define PROJECT_PROJECTIONOPERATOR_H

#include "Operator.hpp"

class ProjectionOperator {
private:
  Operator *input;
  std::vector<Register *> output;
  std::vector<int> projectedIndices;

public:
  ProjectionOperator(Operator *input, std::vector<int> projectedIndices)
      : input(input), projectedIndices(projectedIndices) { }

  void open();

  bool next();

  void close();

  std::vector<Register *> getOutput();
};

#endif //PROJECT_PROJECTIONOPERATOR_H
