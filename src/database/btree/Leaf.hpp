//
// Created by Martin on 31.05.2015.
//

#ifndef PROJECT_LEAF_H
#define PROJECT_LEAF_H

#include <stddef.h>
#include <stdint.h>
#include "Entry.hpp"
#include "../slotted_pages/TID.hpp"

template<class KeyType, class KeyComparator>
struct Leaf {
  struct Header {
    size_t keyCount;
    Leaf<KeyType, KeyComparator> *previousLeaf;
    Leaf<KeyType, KeyComparator> *nextLeaf;

    Header(Leaf<KeyType, KeyComparator> *previous, Leaf<KeyType, KeyComparator> *next)
        : previousLeaf(previous), nextLeaf(next),
          keyCount(0) { }
  } header;

  /**
   * key -> TID.
   * Only use the value of the first entry, the key is meaningless.
   */
  Entry<KeyType, TID> entries[];

  Leaf(Leaf<KeyType, KeyComparator> *previous, Leaf<KeyType, KeyComparator> *next)
      : header(previous, next) { }

  inline void visualize(uint64_t *leafId);

  inline bool hasSpaceForOneMoreEntry();

  inline void insert(KeyType key, TID tid);
};

#include "Leaf.inl.cpp"

#endif //PROJECT_LEAF_H
