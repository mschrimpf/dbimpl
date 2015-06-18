#include "TableScanOperator.hpp"

void TableScanOperator::open() {
  iter = segment.begin();
  iterEnd = segment.end();
  unsigned int numColumns = relation.attributes.size();
  uint64_t numRows = segment.getNumRecords();
  registers.reserve((unsigned int) (numColumns * numRows));
  output.reserve(numColumns);
}

void TableScanOperator::close() {
  registers.clear();
};

bool TableScanOperator::next() {
  if (iter == iterEnd) {
    return false;
  }
  const Record &record = *iter;
  output = extractRegisters(record);
  ++iter;
  return true;
};

std::vector<Register *> TableScanOperator::extractRegisters(const Record &record) {
  std::vector<Register *> result;
  const char *recordData = record.getData();
  int offset = 0;
  for (auto attribute : relation.attributes) {
    char registerData[attribute.len];
    memcpy(registerData, recordData + offset, attribute.len);
    offset += attribute.len;
    Register reg;
    switch (attribute.type) {
      case Types::Tag::Char:
        reg.setStringValue(std::string(registerData, attribute.len));
        break;
      case Types::Tag::Integer:
        reg.setIntegerValue((Integer) *registerData);
        break;
    }
    registers.push_back(reg);
    const Register *regPtr = &registers.back();
    result.push_back((Register *) regPtr);
  }
  return result;
}

std::vector<Register *> TableScanOperator::getOutput() {
  return output;
};
