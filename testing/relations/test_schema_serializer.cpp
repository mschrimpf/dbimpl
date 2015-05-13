//
// Created by daniel on 5/13/15.
//

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

#include "gtest/gtest.h"
#include "../../src/database/relations/parser/Parser.hpp"
#include "../../src/database/relations/SchemaSerializer.h"


TEST(SchemaSerializer, SameSize) {
    Parser p(argv[1]);
    try {
        Schema* schema = p.parse().get();
        storeSchema(schema);
        Schema* schema2 = loadSchema();

        EXPECT_EQ(sizeof(*schema), sizeof(*schema2));
        EXPECT_EQ(*schema, *schema2); //will fail if references are checked

    } catch (ParserError& e) {
        std::cerr << e.what() << std::endl;
        EXPECT_TRUE(false);
    }
}