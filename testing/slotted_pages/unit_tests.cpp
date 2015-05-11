//
// Created by Martin on 11.05.2015.
//

#include "gtest/gtest.h"

namespace {
	// The fixture for testing class Foo.
	class FooTest : public ::testing::Test {
	};

	// Tests that the Foo::Bar() method does Abc.
	TEST(FooTest, MethodBarDoesAbc) {
		EXPECT_EQ(0, 0);
	}

	// Tests that Foo does Xyz.
	TEST(FooTest, DoesXyz) {
		// Exercises the Xyz feature of Foo.
	}

}  // namespace

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}