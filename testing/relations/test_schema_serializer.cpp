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

void Equals(Schema firstSchema, Schema secondSchema);
void Equals(Schema::Relation firstRelation, Schema::Relation secondRelation);
void Equals(Schema::Relation::Attribute firstAttribute, Schema::Relation::Attribute secondAttribute);

TEST(SchemaSerializer, SameSchema) {
    const char *tempFile = "schema.temp";
    Parser p("test.sql");
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
    Equals(schemaWrite, schemaRead);

    if (fd != -1) {
        close(fd);
        remove(tempFile);
    }
}

void Equals(Schema firstSchema, Schema secondSchema){
    EXPECT_EQ(firstSchema.relations.size(), secondSchema.relations.size());
    for (int r = 0; r < firstSchema.relations.size(); ++r) {
        Schema::Relation relationWrite = firstSchema.relations[r];
        Schema::Relation relationRead = secondSchema.relations[r];
        Equals(relationWrite, relationRead);
    }
}

void Equals(Schema::Relation firstRelation, Schema::Relation secondRelation){
    EXPECT_EQ(firstRelation.name, secondRelation.name);

    EXPECT_EQ(firstRelation.attributes.size(), secondRelation.attributes.size());
    for (int a = 0; a < firstRelation.attributes.size(); ++a) {
        Schema::Relation::Attribute firstAttribute = firstRelation.attributes[a];
        Schema::Relation::Attribute secondAttribute = secondRelation.attributes[a];
        Equals(firstAttribute, secondAttribute);
    }

    EXPECT_EQ(firstRelation.primaryKey.size(), secondRelation.primaryKey.size());
    for (int p = 0; p < firstRelation.primaryKey.size(); ++p) {
        EXPECT_EQ(firstRelation.primaryKey[p], secondRelation.primaryKey[p]);
    }
}

void Equals(Schema::Relation::Attribute firstAttribute, Schema::Relation::Attribute secondAttribute) {
    EXPECT_EQ(firstAttribute.name, secondAttribute.name);
    EXPECT_EQ(firstAttribute.type, secondAttribute.type);
    EXPECT_EQ(firstAttribute.notNull, secondAttribute.notNull);
    EXPECT_EQ(firstAttribute.len, secondAttribute.len);
}