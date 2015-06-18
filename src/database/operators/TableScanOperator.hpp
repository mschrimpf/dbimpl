#ifndef PROJECT_TABLESCANOPERATOR_H
#define PROJECT_TABLESCANOPERATOR_H

#include "Operator.hpp"
#include "../slotted_pages/SPSegment.hpp"
#include "../relations/Schema.hpp"

class TableScanOperator : public Operator {
private:
  Schema::Relation &relation;
  SPSegment &segment;
  SPSegment::iterator iter;
  SPSegment::iterator iterEnd;
  std::vector<Register *> output;
  std::vector<Register> registers;

  std::vector<Register *> extractRegisters(const Record &record);

public:
  TableScanOperator(Schema::Relation &relation, SPSegment &segment)
      : relation(relation), segment(segment) { }

  void open();

  bool next();

  void close();

  std::vector<Register *> getOutput();
};

#endif //PROJECT_TABLESCANOPERATOR_H
