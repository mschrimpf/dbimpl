//
// Created by daniel on 5/13/15.
//


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

class SerializerTest : public Test {

private:
    BufferManager *bufferManager;
    SchemaSerializer *schemaSerializer;
public:

    void SetUp() {
        bufferManager = new BufferManager(65536);
        schemaSerializer = new SchemaSerializer(bufferManager);
    }

    void TearDown() {
        delete this->bufferManager;
        delete this->schemaSerializer;
    };

    void Equals(Schema firstSchema, Schema secondSchema) {
        EXPECT_EQ(firstSchema.relations.size(), secondSchema.relations.size());
        EXPECT_EQ(firstSchema.toString(), secondSchema.toString());
        for (int r = 0; r < firstSchema.relations.size(); ++r) {
            Schema::Relation relationWrite = firstSchema.relations[r];
            Schema::Relation relationRead = secondSchema.relations[r];
            Equals(relationWrite, relationRead);
        }
    }

    void Equals(Schema::Relation firstRelation, Schema::Relation secondRelation) {
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

    void saveSchema(Schema schema) {
        schemaSerializer->saveSchema(schema);
    }

    Schema loadSchema() {
        return schemaSerializer->loadSchema();
    }
};

TEST_F(SerializerTest, SameSchema) {
    Parser p("test.sql");
    Schema schemaWrite;
    Schema schemaRead;

    try {
        schemaWrite = *(p.parse().get());
        saveSchema(schemaWrite);
    } catch (ParserError &e) {
        std::cerr << e.what() << std::endl;
        EXPECT_TRUE(false);
    }

    schemaRead = loadSchema();
    Equals(schemaWrite, schemaRead);
}

