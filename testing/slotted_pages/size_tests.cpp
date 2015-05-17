//
// Created by Martin on 11.05.2015.
//

#include "gtest/gtest.h"
#include "../../src/database/slotted_pages/SlottedPage.hpp"
#include "../../src/database/buffer/BufferManager.hpp"

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
	EXPECT_EQ(8, sizeof(Slot));
}

TEST(SlottedPageTest, SizeMatchesPageSize) {
	unsigned int dataSizeByte = BufferManager::DATA_SIZE_BYTE;
	EXPECT_EQ(dataSizeByte, sizeof(SlottedPage));
}
