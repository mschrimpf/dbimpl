//
// Created by Martin on 31.05.2015.
//

#ifndef PROJECT_INNERNODE_H
#define PROJECT_INNERNODE_H

#include <stdint.h>
#include <stddef.h>
#include "BTreeConstants.hpp"
#include "Entry.hpp"
#include "InnerNodeHeader.hpp"

template<typename KeyType, typename KeyComparator>
struct InnerNode {
  InnerNodeHeader header;

  /**
   * key -> page id.
   * Only use the value of the first entry, the key is meaningless.
   */
  Entry<KeyType, uint64_t> entries[
      BTreeConstants<KeyType, KeyComparator>::maxNodeCapacity + 1 /* additional KV pair */];

  InnerNode() : header() { }

  inline bool hasSpaceForOneMoreEntry();

  inline uint64_t getNextNode(KeyType key, KeyComparator &smaller);

  void insertDefiniteFit(KeyType key, uint64_t leftValue, uint64_t rightValue, KeyComparator &smaller);

  int searchInsertPosition(KeyType key, KeyComparator &smaller);

  void moveEntriesToRight(int fromIndex);
};

#include "InnerNode.inl.cpp"

#endif //PROJECT_INNERNODE_H
