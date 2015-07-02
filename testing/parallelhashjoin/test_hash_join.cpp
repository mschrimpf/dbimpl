//
// Created by daniel on 6/23/15.
//

#include "gtest/gtest.h"
#include "../../src/assignment6_parallelhashjoin/LinearProbingHT.cpp"
//#include "../../src/assignment6_parallelhashjoin/ChainingLockingHT.cpp"
#include "../../src/assignment6_parallelhashjoin/ChainingHT.cpp"


using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

class ParallelHashJoinTest : public Test {

private:
public:

  const uint64_t testSize = 100;

  void SetUp() {

  }

  void TearDown() {

  };

  template<typename ParallelHashJoin, typename Entry>
  inline void insertLookupOnce(ParallelHashJoin *parallelHashJoin) {
    Entry * entry = new Entry(1, 1);
    parallelHashJoin->insert(entry);

    uint64_t count = parallelHashJoin->lookup(1);
    ASSERT_EQ(1, count);
    delete entry;
  }

  template<typename ParallelHashJoin, typename Entry>
  inline void insertLookupSameKey(ParallelHashJoin *parallelHashJoin) {
    Entry * entry1 = new Entry(1, 1);
    Entry * entry2 = new Entry(1, 1);

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
      Entry * entry = new Entry(t, 1);
      entryPointers.push_back(entry);
      parallelHashJoin->insert(entry);
    }

//    parallelHashJoin->print();

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

// Insert
TEST_F(ParallelHashJoinTest, ChainingInsertOnce) {
  ChainingHT hashTable(testSize);
  insertLookupOnce<ChainingHT, ChainingHT::Entry>(&hashTable);
}

TEST_F(ParallelHashJoinTest, ChainingInsertLookupSameKey) {
  ChainingHT hashTable(testSize);
  insertLookupSameKey<ChainingHT, ChainingHT::Entry>(&hashTable);
}

TEST_F(ParallelHashJoinTest, ChainingInsert50) {
  ChainingHT hashTable(testSize);
  insertLookup50<ChainingHT, ChainingHT::Entry>(&hashTable);
}

TEST_F(ParallelHashJoinTest, LinearProbingInsertOnce) {
  LinearProbingHT hashTable(testSize);
  insertLookupOnce<LinearProbingHT, LinearProbingHT::Entry>(&hashTable);
}

TEST_F(ParallelHashJoinTest, LinearProbingInsertLookupSameKey) {
  LinearProbingHT hashTable(testSize);
  insertLookupSameKey<LinearProbingHT, LinearProbingHT::Entry>(&hashTable);
}

TEST_F(ParallelHashJoinTest, LinearProbingInsert50) {
  LinearProbingHT hashTable(testSize);
  insertLookup50<LinearProbingHT, LinearProbingHT::Entry>(&hashTable);
}

TEST_F(ParallelHashJoinTest, DISABLED_ChainingWithLockingInsert50) {
//  ChainingLockingHT hashTable(testSize);
//  insertLookup50<ChainingLockingHT, ChainingLockingHT::Entry>(&hashTable);
}

// Time
TEST_F(ParallelHashJoinTest, DISABLED_ChainingWithLockingInserTime) {

}

TEST_F(ParallelHashJoinTest, DISABLED_ChainingInsertTime) {

}

TEST_F(ParallelHashJoinTest, DISABLED_LinearProbingInsertTime) {

}

// Lookup
TEST_F(ParallelHashJoinTest, DISABLED_ChainingWithLockingLookup) {

}

TEST_F(ParallelHashJoinTest, DISABLED_LinearProbingLookup) {

}

// Insert parallel
TEST_F(ParallelHashJoinTest, DISABLED_ChainingWithLockingInsertParallel) {

}

TEST_F(ParallelHashJoinTest, DISABLED_ChainingTimeInsertParallel) {

}

TEST_F(ParallelHashJoinTest, DISABLED_LinearProbingInsertParallel) {

}


