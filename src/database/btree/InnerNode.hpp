//
// Created by Martin on 31.05.2015.
//

#ifndef PROJECT_INNERNODE_H
#define PROJECT_INNERNODE_H

#include <stdint.h>
#include <stddef.h>
#include "Entry.hpp"

template<class KeyType>
struct InnerNode {
  struct Header {
    size_t count; // number of entries
    Header() : count(0) { }
  } header;

  Entry<KeyType, void *> entries[];

  InnerNode() : header() { }

  inline void visualize(uint64_t *leafId, uint64_t *nodeId, uint64_t depth, uint64_t maxDepth);

  inline bool hasSpaceForOneMoreEntry();

  inline uint64_t getNextNode(KeyType key);
};

#include "InnerNode.inl.cpp"

#endif //PROJECT_INNERNODE_H
