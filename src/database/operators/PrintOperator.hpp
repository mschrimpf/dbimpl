//
// Created by daniel on 6/9/15.
//

#ifndef PROJECT_PRINTOPERATOR_H
#define PROJECT_PRINTOPERATOR_H

#include "Operator.hpp"

template<typename Type>
class PrintOperator : Operator<Type> {
private:
  std::vector<std::vector<Register<Type> *>> relation;
  std::ostream &outputStream;

  std::string toString(Type val);

public:
  PrintOperator(std::vector<std::vector<Register<Type> *>> relation, std::ostream &outputStream)
          : relation(relation), outputStream(outputStream) { };

  void open();

  bool next();

  void close();

  std::vector<Register<Type> *> getOutput();
};


#include "PrintOperator.inl.cpp"


#endif //PROJECT_PRINTOPERATOR_H
