#include "BTreeTestFixture.hpp"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;


TEST_F(BTreeTest, TestBTreeVisualization) {
  for (unsigned id = 0; id < 13; ++id){
    bTree->insert(id, TID(0));
  }
  bTree->outputVisualize();
}






