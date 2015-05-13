//
// Created by daniel on 5/13/15.
//

#include <sstream>
#include <fstream>
#include <iostream>
#include "SchemaSerializer.h"

Schema* loadSchema() {

}

//  RELATIONS_COUNT
//  RELATION1_SIZE
//  RELATIONS_NAME_LENGTH
//  RELATION1_NAME
//  RELATION1_COUNT_ATTRIBUTES
//      ATTRIBUTE_NAME (
//      ATTRIBUTE_TYPE (0 INT, 1 CHAR)
//      ATTRIBUTE_LEN
//      ATTRIBUTE_NOTNULL
//  ....
//  RELATION1.COUNT_PRIMARY_KEYS
//      PRIMARY_KEY1
//      ...
//
void storeSchema(Schema* schema) {
    std::ofstream stream("");
}

void serialize(Schema schema){
    size_t buffer_size = size_of_schema(schema);
    char * buffer = new char[buffer_size];

    size_t relations_size = schema.relations.size();
    memcpy(buffer, &relations_size, sizeof(size_t)); //copy number of relations
    buffer += sizeof(size_t);
    buffer_size -= sizeof(size_t);

    for (size_t r = 0; r < schema.relations.size(); ++r){
        Schema::Relation relation = schema.relations[r];

        size_t relation_name_length = relation.name.length();
        memcpy(buffer, &relation_name_length, sizeof(size_t)); //copy length of relation name
        buffer+= sizeof(size_t);
        buffer_size -= sizeof(size_t);

        memcpy(buffer, relation.name.c_str(), relation.name.size()); //copy relation name
        buffer+= relation.name.size();
        buffer_size -= relation.name.size();

        memcpy(buffer, (char *)relation.attributes.size(), sizeof(size_t)); //copy number of attributes
        buffer+= sizeof(size_t);
        buffer_size -= sizeof(size_t);

        for (size_t a = 0; a  < relation.attributes.size(); ++a){
            Schema::Relation::Attribute attribute = relation.attributes[a];

            size_t attribute_name_length = attribute.name.length();
            memcpy(buffer, &attribute_name_length, sizeof(size_t)); //copy length of attribute name
            buffer+= sizeof(size_t);
            buffer_size -= sizeof(size_t);

            memcpy(buffer, attribute.name.c_str(), relation.name.size()); //copy attribute name
            buffer+= attribute.name.size();
            buffer_size -= attribute.name.size();

            char type = attribute.type ==  Types::Tag::Integer ? 't' : 'f';
            memcpy(buffer, &type, sizeof(type)); //copy attribute type
            buffer+= sizeof(type);
            buffer_size -= sizeof(type);

            memcpy(buffer, &attribute.len, sizeof(attribute.len)); //copy attribute len
            buffer+= sizeof(attribute.len);
            buffer_size -= sizeof(attribute.len);


            char notnull = attribute.notNull ? 't' : 'f';
            memcpy(buffer, &notnull, sizeof(notnull)); //copy not null
            buffer+= sizeof(notnull);
            buffer_size -= sizeof(notnull);
        }

        size_t primary_keys_size = relation.primaryKey.size();
        memcpy(buffer, &primary_keys_size, sizeof(size_t)); //copy number of primary keys
        buffer+= sizeof(size_t);
        buffer_size -= sizeof(size_t);

        for (size_t k = 0; k < relation.primaryKey.size(); ++k){
            unsigned val = relation.primaryKey[k];
            memcpy(buffer, &val, sizeof(unsigned)); //copy id of primary key
            buffer+= sizeof(unsigned);
            buffer_size -= sizeof(unsigned);
        }
    }
    if (buffer_size != 0){
        std::cerr << "Size of Schema was calculated wrong! Expected difference 0, was: " << buffer_size << std::endl;
    }
}

size_t size_of_schema(Schema schema){
    size_t buffer_size = 0;
    buffer_size += sizeof(size_t); // number of relations
    for (size_t r = 0; r < schema.relations.size(); ++r){
        Schema::Relation relation = schema.relations[r];
        buffer_size += sizeof(size_t); // length of name
        buffer_size += relation.name.size(); // name
        buffer_size += sizeof(size_t); // number of attribues

        for (size_t a = 0; a  < relation.attributes.size(); ++a){
            Schema::Relation::Attribute attribute = relation.attributes[a];
            buffer_size += sizeof(int); // length of attribute name
            buffer_size += attribute.name.size(); // attribute name
            buffer_size += sizeof(Schema::Relation::Attribute::type); // attribute type
            buffer_size += sizeof(Schema::Relation::Attribute::len); // attribute len
            buffer_size += sizeof(bool); //attribute not null
        }
        buffer_size += sizeof(size_t); // number of primary keys
        buffer_size += sizeof(unsigned int) * relation.primaryKey.size();
    }
    return buffer_size;
}

//void write();
