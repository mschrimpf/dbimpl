//
// Created by daniel on 5/13/15.
//

#ifndef PROJECT_SCHEMASERIALIZER_H
#define PROJECT_SCHEMASERIALIZER_H

#include "Schema.hpp"

void storeSchema(Schema schema, int fd);
Schema loadSchema(int fd);
size_t size_of_schema(Schema schema);

#endif //PROJECT_SCHEMASERIALIZER_H
