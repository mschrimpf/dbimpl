#include <stdint-gcc.h>
#include <gtest/gtest.h>
#include "../../src/database/operators/Register.hpp"
#include "../../src/database/slotted_pages/SPSegment.hpp"
#include "../../src/database/operators/TableScanOperator.hpp"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

class TableScanOperatorTest : public Test {
private:
  BufferManager *bufferManager;
  uint64_t SEGMENT_ID = 1;

public:
  SPSegment *segment;

  void SetUp() {
    bufferManager = new BufferManager(65536);
    segment = new SPSegment(*bufferManager, SEGMENT_ID);
  }

  void TearDown() {
    delete segment;
    delete bufferManager;

    std::string filename = std::to_string(SEGMENT_ID);
    remove(filename.c_str());
  };

  void testInteger(unsigned numRecords, unsigned numColumns) {
    Integer values[numRecords][numColumns];
    Integer valueIt = 1;
    size_t recordLen = numColumns * sizeof(Integer);
    for (unsigned r = 0; r < numRecords; r++) {
      for (unsigned c = 0; c < numColumns; c++) {
        values[r][c] = valueIt;
      }
      valueIt++;
      segment->insert(Record(recordLen, (const char *) values[r]));
    }

    Schema::Relation relation("Pairs");
    for (unsigned c = 0; c < numColumns; c++) {
      std::string colString = std::to_string(c);
      Schema::Relation::Attribute attr("val-" + colString, Types::Tag::Integer, sizeof(Integer));
      relation.addAttribute(attr);
    }

    TableScanOperator op(relation, *segment);
    op.open();
    for (unsigned r = 0; r < numRecords; r++) {
      ASSERT_TRUE(op.next());
      ASSERT_EQ(numColumns, op.getOutput().size());
      for (unsigned c = 0; c < numColumns; c++) {
        Register *reg = op.getOutput()[c];
        ASSERT_EQ(values[r][c], reg->getIntegerValue());
      }
    }
    ASSERT_FALSE(op.next());

    op.close();
  }
};

TEST_F(TableScanOperatorTest, IntegerOneColumn) {
  testInteger(3, 1);
}

TEST_F(TableScanOperatorTest, IntegerTwoColumns) {
  testInteger(3, 2);
}
