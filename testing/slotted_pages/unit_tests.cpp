//
// Created by Martin on 11.05.2015.
//
#include <stdio.h>
#include <stdlib.h>

#include "gtest/gtest.h"
#include "../../src/database/slotted_pages/SlottedPage.hpp"
#include "../../src/database/slotted_pages/SPSegment.hpp"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

#include "../../src/database/slotted_pages/SlottedPage.hpp"


namespace {
	// The fixture for testing class Foo.
	class FooTest : public ::testing::Test {
	};

	class SlottedPageTest : public ::testing::Test {

	};

	TEST(SlottedPageTest, Positive){
		EXPECT_TRUE(sizeof(SlottedPage) == PAGE_SIZE_BYTE);
	}

	// Tests that the Foo::Bar() method does Abc.
	TEST(FooTest, MethodBarDoesAbc) {
		EXPECT_EQ(0, 0);
	}

	// Tests that Foo does Xyz.
	TEST(FooTest, DoesXyz) {
		// Exercises the Xyz feature of Foo.
	}

}  // namespace
TEST(SlottedPageTest, SizeMatchesPageSize) {
	EXPECT_EQ(PAGE_SIZE_BYTE, sizeof(SlottedPage));
}


// see https://code.google.com/p/googletest/source/browse/trunk/samples/sample10_unittest.cc
TEST(SlotDataTest, LengthSizeMatchesSlotLengthSize) {
	EXPECT_EQ(sizeof(SlottedPage::Slot::length), sizeof(SPSegment::SlotData::length));
}

int main(int argc, char **argv) {
	InitGoogleTest(&argc, argv);

	bool check_for_leaks = false;
	if (argc > 1 && strcmp(argv[1], "--check_for_leaks") == 0)
		check_for_leaks = true;
	else
		printf("%s\n", "Run this program with --check_for_leaks to enable "
				"custom leak checking in the tests.");

	// If we are given the --check_for_leaks command line flag, installs the
	// leak checker.
	if (check_for_leaks) {
		TestEventListeners &listeners = UnitTest::GetInstance()->listeners();

		// Adds the leak checker to the end of the test event listener list,
		// after the default text output printer and the default XML report
		// generator.
		//
		// The order is important - it ensures that failures generated in the
		// leak checker's OnTestEnd() method are processed by the text and XML
		// printers *before* their OnTestEnd() methods are called, such that
		// they are attributed to the right test. Remember that a listener
		// receives an OnXyzStart event *after* listeners preceding it in the
		// list received that event, and receives an OnXyzEnd event *before*
		// listeners preceding it.
		//
		// We don't need to worry about deleting the new listener later, as
		// Google Test will do it.
		//listeners.Append(new LeakChecker);
	}
	return RUN_ALL_TESTS();
}
