//
// Created by daniel on 5/13/15.
//

#ifndef PROJECT_SCHEMASERIALIZER_H
#define PROJECT_SCHEMASERIALIZER_H

#include "Schema.hpp"
#include "../buffer/BufferManager.hpp"

class SchemaSerializer{
private:
    BufferManager * bufferManager;

    void writeSchema(Schema schema, void *data);

    Schema readSchema(void *data);

    size_t size_of_schema(Schema schema);

    const uint64_t segmentId = 0;

    const uint64_t pageId = 0;
public:
    SchemaSerializer(BufferManager * manager);

    Schema loadSchema();

    void saveSchema(Schema schema);

};



#endif //PROJECT_SCHEMASERIALIZER_H
