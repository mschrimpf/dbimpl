
#ifndef PROJECT_HASHJOINOPERATOR_H
#define PROJECT_HASHJOINOPERATOR_H

#include "Operator.hpp"
#include <unordered_map>

class HashJoinOperator : public Operator {
private:
  Operator *left;
  Operator *right;
  int leftJoinIndex;
  int rightJoinIndex;
  std::unordered_map<Register, std::vector<Register *>> hashMap;
  std::vector<Register *> output;

public:
  HashJoinOperator(Operator *left, Operator *right, int leftJoinIndex, int rightJoinIndex)
      : left(left), right(right), leftJoinIndex(leftJoinIndex), rightJoinIndex(rightJoinIndex),
        hashMap() { }

  void open();

  bool next();

  void close();

  std::vector<Register *> getOutput();
};


#endif //PROJECT_HASHJOINOPERATOR_H
