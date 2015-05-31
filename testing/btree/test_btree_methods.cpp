#include <gtest/gtest.h>
#include "BTree.hpp"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

class BTreeTest : public Test {
private:
    static const uint64_t maxNodeEntries = 5; //TODO
    static const uint64_t maxLeafEntries = 10; //TODO
    static const uint64_t SEGMENT_ID = 1;

    SPSegment *segment;
    BufferManager *bufferManager;
    char *allocatedData = nullptr;

    /**
     * Allocates data that will automatically be free'd at the end of the test.
     */
    char *allocateData(size_t len) {
        this->allocatedData = (char *) malloc(len);
        return this->allocatedData;
    }
public:
    BTree *bTree;

    void SetUp() {
        bufferManager = new BufferManager(65536);
        segment = new SPSegment(*bufferManager, SEGMENT_ID);
        bTree = new BTree(bufferManager, segment, maxNodeEntries, maxLeafEntries);
    }

    void TearDown() {
        delete this->segment;
        delete this->bufferManager;
        delete this->bTree;


        if (allocatedData != nullptr) {
            free(allocatedData);
        }
    };
};


TEST_F(BTreeTest, InsertFindTest) {
    TID tid (0);
    ASSERT_EQ(0, bTree->size());
    ASSERT_TRUE(bTree->insert(0, tid));
    ASSERT_EQ(1, bTree->size());

    TID tid2 (0);
    ASSERT_TRUE(bTree->lookup(0, tid2));
    ASSERT_EQ(tid.pageId, tid2.pageId);
    ASSERT_EQ(tid.slotOffset, tid2.slotOffset);
}

TEST_F(BTreeTest, EmptyAddAndRemoveTest) {
    TID tid(0);
    ASSERT_EQ(0, bTree->size());
    ASSERT_TRUE(bTree->insert(0, tid));
    ASSERT_EQ(1, bTree->size());
    ASSERT_TRUE(bTree->erase(0));
    ASSERT_EQ(0, bTree->size());
}

TEST_F(BTreeTest, InsertSameKeyTest) {
    uint64_t key = 0;
    TID tid(0);
    ASSERT_TRUE(bTree->insert(key, tid));
    ASSERT_FALSE(bTree->insert(key, tid));
    ASSERT_EQ(1, bTree->size());

    //TODO what is the behaviour here? should the insertion fail?
}

TEST_F(BTreeTest, TestSplit) {
}

TEST_F(BTreeTest, TestMerge) {
}

TEST_F(BTreeTest, TestSplitUp) {
}

TEST_F(BTreeTest, TestMergeUp) {
}

TEST_F(BTreeTest, TestLeafVisualization){
    uint64_t id = 0;
    BTree::Leaf leaf (nullptr, nullptr);
    leaf.visualize(&id);
}