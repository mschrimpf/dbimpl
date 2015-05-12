//
// Created by Martin on 11.05.2015.
//

#include "gtest/gtest.h"
#include "../../src/database/slotted_pages/SlottedPage.hpp"

TEST(SlottedPageTest, SizeMatchesPageSize) {
	EXPECT_EQ(PAGE_SIZE_BYTE, sizeof(SlottedPage));
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}