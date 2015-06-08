#include "BTreeTestFixture.hpp"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;


TEST_F(BTreeTest, BTreeVisualizationInsert) {
  for (unsigned id = 0; id < 13; ++id){
    std::cout << "Adding Pair(" << id << ",0) to bTree\n";
    bTree->insert(id, TID(0));
  }
  bTree->outputVisualize();
  std::cout << "Success" << std::endl;
  //std::cout << "Visualization\n" << bTree->visualize();
}

TEST_F(BTreeTest, BTreeVisualizationDelete) {
  for (unsigned id = 0; id < 20; ++id){
    std::cout << "Adding Pair(" << id << ",0) to bTree\n";
    bTree->insert(id, TID(0));
  }
  for (unsigned id = 0; id < 20; ++id){
    if (id % 7 != 0){
      continue;
    }
    bTree->erase(id);
  }

  bTree->outputVisualize();
  std::cout << "Success" << std::endl;
  //std::cout << "Visualization\n" << bTree->visualize();
}






