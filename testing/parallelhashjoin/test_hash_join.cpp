#include "gtest/gtest.h"
#include "../../src/assignment6_parallelhashjoin/LinearProbingHT.cpp"
#include "../../src/assignment6_parallelhashjoin/ChainingLockingHT.cpp"
#include "../../src/assignment6_parallelhashjoin/ChainingHT.cpp"
#include "../../src/assignment6_parallelhashjoin/ChainingBloomHT.cpp"
#include "../../src/assignment6_parallelhashjoin/ChainingBloomAggregateHT.cpp"
#include "../../src/assignment6_parallelhashjoin/MatrixHT.cpp"


using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

class ParallelHashJoinTest : public Test {
public:
  const uint64_t testSize = 100;

  template<typename ParallelHashJoin, typename Entry>
  inline void insertLookupOnce(ParallelHashJoin *parallelHashJoin) {
    Entry *entry = new Entry(1);
    parallelHashJoin->insert(entry);

    uint64_t count = parallelHashJoin->lookup(1);
    ASSERT_EQ(1, count);
    delete entry;
  }

  template<typename ParallelHashJoin, typename Entry>
  inline void insertLookupSameKey(ParallelHashJoin *parallelHashJoin) {
    Entry *entry1 = new Entry(1);
    Entry *entry2 = new Entry(1);

    parallelHashJoin->insert(entry1);
    parallelHashJoin->insert(entry2);

    uint64_t count = parallelHashJoin->lookup(1);
    ASSERT_EQ(2, count);

    delete entry1;
    delete entry2;
  }

  template<typename ParallelHashJoin, typename Entry>
  inline void insertLookup50(ParallelHashJoin *parallelHashJoin) {
    std::vector<Entry *> entryPointers;
    for (uint64_t t = 0; t < 100; t++) {
      if (t % 2 == 0) {
        continue;
      }
      Entry *entry = new Entry(t);
      entryPointers.push_back(entry);
      parallelHashJoin->insert(entry);
    }

    for (uint64_t t = 0; t < 100; t++) {
      uint64_t count = parallelHashJoin->lookup(t);
      if (t % 2 == 0) {
        ASSERT_EQ(0, count);
      } else {
        ASSERT_EQ(1, count);
      }
    }

    for (auto &entryIt : entryPointers) {
      delete entryIt;
    }
  }
};

#define HT_TESTS(TABLE_NAME) \
  TEST_F(ParallelHashJoinTest, TABLE_NAME ## InsertOnce) {\
    TABLE_NAME hashTable(testSize);\
    insertLookupOnce<TABLE_NAME, TABLE_NAME::Entry>(&hashTable);\
  }\
  \
  TEST_F(ParallelHashJoinTest, TABLE_NAME ## InsertLookupSameKey) {\
    TABLE_NAME hashTable(testSize);\
    insertLookupSameKey<TABLE_NAME, TABLE_NAME::Entry>(&hashTable);\
  }\
  \
  TEST_F(ParallelHashJoinTest, TABLE_NAME ## Insert50) {\
    TABLE_NAME hashTable(testSize);\
    insertLookup50<TABLE_NAME, TABLE_NAME::Entry>(&hashTable);\
  }

HT_TESTS(ChainingHT)
HT_TESTS(LinearProbingHT)
HT_TESTS(ChainingLockingHT)
HT_TESTS(ChainingBloomHT)
HT_TESTS(MatrixHT)