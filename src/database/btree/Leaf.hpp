//
// Created by Martin on 31.05.2015.
//

#ifndef PROJECT_LEAF_H
#define PROJECT_LEAF_H

#include <stddef.h>
#include <stdint.h>
#include "Entry.hpp"
#include "BTreeConstants.hpp"
#include "../slotted_pages/TID.hpp"
#include "LeafHeader.hpp"

template<typename KeyType, typename KeyComparator>
struct Leaf {
  LeafHeader header;

  /**
   * key -> TID.
   * Only use the value of the first entry, entries[0].key is meaningless.
   */
  Entry<KeyType, TID> entries[
      BTreeConstants<KeyType, KeyComparator>::maxLeafCapacity + 1 /* additional KV pair */];

  Leaf(uint64_t previousPageId, uint64_t nextPageId)
      : header(previousPageId, nextPageId) { }

  inline bool hasSpaceForOneMoreEntry();

  inline void insertDefiniteFit(KeyType key, TID tid);

  inline void erase(KeyType key);
};

#include "Leaf.inl.cpp"

#endif //PROJECT_LEAF_H
