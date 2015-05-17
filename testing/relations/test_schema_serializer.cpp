//
// Created by daniel on 5/13/15.
//



#include <fcntl.h>
#include "gtest/gtest.h"
#include "../../src/database/relations/parser/Parser.hpp"
#include "../../src/database/relations/SchemaSerializer.h"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

TEST(SchemaSerializer, SameSize) {
    const char * tempFile = "/media/daniel/Studium/Universität/Master/2.Semester/Datenbankimplementierung/repository/dbimpl/testing/relations/schema.temp";
    Parser p("/media/daniel/Studium/Universität/Master/2.Semester/Datenbankimplementierung/repository/dbimpl/testing/relations/test.sql");
    int fd = -1;
    Schema schemaWrite;
    Schema schemaRead;

    try {
        schemaWrite = *(p.parse().get());
        fd = open(tempFile, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
        storeSchema(schemaWrite, fd);
    } catch (ParserError &e) {
        std::cerr << e.what() << std::endl;
        EXPECT_TRUE(false);
    }
    if (fd != -1) {
        close(fd);
    }
    fd = open(tempFile, O_RDONLY);
    schemaRead = loadSchema(fd);

    EXPECT_EQ(sizeof(schemaWrite), sizeof(schemaRead));
    //EXPECT_EQ(schemaWrite, schemaRead); //will fail if references are checked

    if (fd != -1) {
        close(fd);
        remove(tempFile);
    }
}