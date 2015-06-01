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
   * Only use the value of the first entry, the key is meaningless.
   */
  Entry<KeyType, TID> entries[
      BTreeConstants<KeyType, KeyComparator>::maxLeafCapacity + 1 /* additional KV pair */];

  Leaf(Leaf<KeyType, KeyComparator> *previous, Leaf<KeyType, KeyComparator> *next)
      : header(previous, next) { }

  inline void visualize(uint64_t leafId);

  inline bool hasSpaceForOneMoreEntry();

  inline void insertDefiniteFit(KeyType key, TID tid);
};

#include "Leaf.inl.cpp"

#endif //PROJECT_LEAF_H
