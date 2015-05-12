//
// Created by Martin on 11.05.2015.
//

#include "gtest/gtest.h"
#include "../../src/database/slotted_pages/SlottedPage.hpp"
#include "../../src/database/slotted_pages/SPSegment.hpp"

TEST(SlottedPageTest, SizeMatchesPageSize) {
	EXPECT_EQ(PAGE_SIZE_BYTE, sizeof(SlottedPage));
}

TEST(SlotDataTest, LengthSizeMatchesSlotLengthSize) {
	EXPECT_EQ(sizeof(SlottedPage::Slot::length), sizeof(SPSegment::SlotData::length));
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}