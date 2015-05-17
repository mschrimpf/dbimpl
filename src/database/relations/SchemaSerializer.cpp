//
// Created by daniel on 5/13/15.
//

#include <sstream>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include "SchemaSerializer.h"
#include "../../database/util/debug.h"
#include "../buffer/BufferManager.hpp"

//  ++++++++++++++++++++++
//  General Schema Idea
//
//  RELATIONS_COUNT
//      RELATION1_SIZE
//      RELATIONS_NAME_LENGTH
//      RELATION1_NAME
//      RELATION1_COUNT_ATTRIBUTES
//          ATTRIBUTE_NAME (
//          ATTRIBUTE_TYPE (0 INT, 1 CHAR)
//          ATTRIBUTE_LEN
//          ATTRIBUTE_NOTNULL
//      ....
//      RELATION1.COUNT_PRIMARY_KEYS
//          PRIMARY_KEY1
//          ...
//      ...
//  +++++++++++++++++++++

Schema loadSchema(int fd) {

    // read size of schema
    char * sizeBuffer = new char[sizeof(size_t)];
    read(fd, sizeBuffer, sizeof(size_t));
    size_t size_of_schema = *reinterpret_cast<size_t *>(&sizeBuffer);


    //* read whole schema into buffer
    char * buffer = new char[size_of_schema];
    read(fd, buffer, size_of_schema);

    //* create vector of relations
    size_t number_of_relations = *reinterpret_cast<size_t*> (buffer);
    buffer += sizeof(size_t);

    std::vector<Schema::Relation> relations;
    relations.reserve(number_of_relations);

    Schema schema = Schema();
    schema.relations = relations;

    for (size_t r = 0; r < number_of_relations; ++r){
        // read length of relation name
        size_t length_of_relation_name = *reinterpret_cast<size_t*> (buffer);
        buffer += sizeof(size_t);
        // read name
        char * relation_name = new char[length_of_relation_name];
        memcpy(relation_name, buffer, length_of_relation_name);
        buffer += length_of_relation_name;

        Schema::Relation relation (relation_name);

        // read number of attributes
        size_t number_of_attributes = *reinterpret_cast<size_t*> (buffer);
        buffer += sizeof(size_t);
        relation.attributes.reserve(number_of_attributes);

        for (size_t a = 0; a < number_of_attributes; ++a){
            Schema::Relation::Attribute attribute;

            // read length of attribute name
            size_t length_of_attribute_name = *reinterpret_cast<size_t*> (buffer);
            buffer += sizeof(size_t);
            // read name
            char * attribute_name = new char[length_of_relation_name];
            memcpy(attribute_name, buffer, length_of_relation_name);
            buffer += length_of_attribute_name;
            attribute.name = attribute_name;

            // read type
            char * type = new char[sizeof(char)];
            memcpy(type, buffer, sizeof(char));
            buffer += sizeof(char);
            attribute.type = type[0] == 't' ? Types::Tag::Integer : Types::Tag::Char;

            // read attribute length
            unsigned attribute_length = *reinterpret_cast<unsigned*> (buffer);
            buffer += sizeof(unsigned);
            attribute.len = attribute_length;

            // read not null
            char * notnull = new char[sizeof(char)];
            memcpy(type, buffer, sizeof(char));
            buffer += sizeof(char);
            attribute.notNull = notnull[0] == 't';

            relation.attributes.push_back(attribute);
        }

        size_t number_of_primary_keys = *reinterpret_cast<size_t*> (buffer);
        buffer += sizeof(size_t);
        relation.primaryKey.reserve(number_of_primary_keys);

        for (size_t k = 0; k < number_of_primary_keys; ++k){
            unsigned primary_key = *reinterpret_cast<unsigned*> (buffer);
            buffer += sizeof(unsigned);
            relation.primaryKey[k] = primary_key;
        }
        schema.relations.push_back(relation);
    }

   return schema;
}


void storeSchema(Schema schema, int fd) {
    size_t schema_size = size_of_schema(schema);
    size_t buffer_size = schema_size + sizeof(size_t);
    char *buffer = new char[buffer_size]; //size_t is for storing the length of the schema

    memcpy(buffer, &schema_size, sizeof(size_t)); //copy size of schema
    buffer += sizeof(size_t);
    size_t used_bytes = sizeof(size_t);

    size_t relations_size = schema.relations.size();
    memcpy(buffer, &relations_size, sizeof(size_t)); //copy number of relations
    buffer += sizeof(size_t);
    used_bytes += sizeof(size_t);

    for (size_t r = 0; r < schema.relations.size(); ++r) {
        Schema::Relation relation = schema.relations[r];

        size_t relation_name_length = relation.name.length();
        memcpy(buffer, &relation_name_length, sizeof(size_t)); //copy length of relation name
        buffer += sizeof(size_t);
        used_bytes += sizeof(size_t);

        memcpy(buffer, relation.name.c_str(), relation.name.size()); //copy relation name
        buffer += relation.name.size();
        used_bytes += relation.name.size();

        size_t relation_attribute_size = relation.attributes.size();
        memcpy(buffer, &relation_attribute_size, sizeof(size_t)); //copy number of attributes
        buffer += sizeof(size_t);
        used_bytes += sizeof(size_t);

        for (size_t a = 0; a < relation.attributes.size(); ++a) {
            Schema::Relation::Attribute attribute = relation.attributes[a];

            size_t attribute_name_length = attribute.name.length();
            memcpy(buffer, &attribute_name_length, sizeof(size_t)); //copy length of attribute name
            buffer += sizeof(size_t);
            used_bytes += sizeof(size_t);

            memcpy(buffer, attribute.name.c_str(), relation.name.size()); //copy attribute name
            buffer += attribute.name.size();
            used_bytes += attribute.name.size();

            char type = attribute.type == Types::Tag::Integer ? 't' : 'f';
            memcpy(buffer, &type, sizeof(type)); //copy attribute type
            buffer += sizeof(type);
            used_bytes += sizeof(type);

            memcpy(buffer, &attribute.len, sizeof(attribute.len)); //copy attribute len
            buffer += sizeof(attribute.len);
            used_bytes += sizeof(attribute.len);


            char notnull = attribute.notNull ? 't' : 'f';
            memcpy(buffer, &notnull, sizeof(notnull)); //copy not null
            buffer += sizeof(notnull);
            used_bytes += sizeof(notnull);
        }
        size_t primary_keys_size = relation.primaryKey.size();
        memcpy(buffer, &primary_keys_size, sizeof(size_t)); //copy number of primary keys
        buffer += sizeof(size_t);
        used_bytes += sizeof(size_t);

        for (size_t k = 0; k < relation.primaryKey.size(); ++k) {
            unsigned val = relation.primaryKey[k];
            memcpy(buffer, &val, sizeof(unsigned)); //copy id of primary key
            buffer += sizeof(unsigned);
            used_bytes += sizeof(unsigned);
        }
    }
    if (buffer_size != used_bytes) {
        std::cerr << "Size of Schema was calculated wrong! Expected buffer_size: " << buffer_size << " - was " << used_bytes <<
        std::endl;
    }
    // set the pointer to the beginning
    buffer -= buffer_size;
    write(fd, buffer, buffer_size);
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
            buffer_size += sizeof(size_t); // length of attribute name
            buffer_size += attribute.name.size(); // attribute name
            buffer_size += sizeof(char); // attribute type
            buffer_size += sizeof(Schema::Relation::Attribute::len); // attribute len
            buffer_size += sizeof(char); //attribute not null
        }
        buffer_size += sizeof(size_t); // number of primary keys
        buffer_size += sizeof(unsigned int) * relation.primaryKey.size();
    }
    return buffer_size;
}


//void write();
