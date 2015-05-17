
#include "gtest/gtest.h"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

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
