//
// Created by Martin on 31.05.2015.
//

#ifndef PROJECT_LEAF_H
#define PROJECT_LEAF_H

#include <stddef.h>
#include <stdint.h>
#include "Entry.hpp"
#include "../slotted_pages/TID.hpp"

template<class KeyType>
struct Leaf {
  struct Header {
    size_t count; // number of entries
    Leaf *previousLeaf;
    Leaf *nextLeaf;

    Header(Leaf *previous, Leaf *next) : previousLeaf(previous), nextLeaf(next), count(0) { }
  } header;

  Entry<KeyType, TID> entries[];

  Leaf(Leaf *previous, Leaf *next) : header(previous, next) { }

  inline void visualize(uint64_t *leafId);

  inline bool hasSpaceForOneMoreEntry();

  inline void insert(KeyType key, TID tid);
};

#include "Leaf.inl.cpp"

#endif //PROJECT_LEAF_H
