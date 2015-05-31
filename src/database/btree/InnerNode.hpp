//
// Created by Martin on 31.05.2015.
//

#ifndef PROJECT_INNERNODE_H
#define PROJECT_INNERNODE_H

#include <stdint.h>
#include <stddef.h>
#include "Entry.hpp"

template<class KeyType, class KeyComparator>
struct InnerNode {
  struct Header {
    size_t keyCount;
    Header() : keyCount(0) { }
  } header;

  /**
   * key -> page id.
   * Only use the value of the first entry, the key is meaningless.
   */
  Entry<KeyType, uint64_t> entries[];

  InnerNode() : header() { }

  inline void visualize(uint64_t *leafId, uint64_t *nodeId, uint64_t depth, uint64_t maxDepth);

  inline bool hasSpaceForOneMoreEntry();

  inline uint64_t getNextNode(KeyType key);
};

#include "InnerNode.inl.cpp"

#endif //PROJECT_INNERNODE_H
