//
// Created by Martin on 11.05.2015.
//

#include "gtest/gtest.h"
#include "../../src/database/slotted_pages/SlottedPage.hpp"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;


TEST(TidTest, SizeIs8Byte) {
	EXPECT_EQ(8, sizeof(TID));
}

TEST(SlotTest, SizeIs8Byte) {
	EXPECT_EQ(8, sizeof(SlottedPage::Slot));
}

TEST(SlottedPageTest, SizeMatchesPageSize) {
	EXPECT_EQ(BufferManager::DATA_SIZE_BYTE, sizeof(SlottedPage));
}
