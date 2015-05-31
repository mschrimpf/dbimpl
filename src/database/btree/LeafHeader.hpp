//
// Created by Martin on 31.05.2015.
//

#ifndef PROJECT_LEAFHEADER_HPP
#define PROJECT_LEAFHEADER_HPP

#include <stddef.h>

// cannot use Leaf in the previous and next pointers
// since that would lead to cyclic dependency
struct LeafHeader {
  size_t keyCount;
  void *previousLeaf;
  void *nextLeaf;

  LeafHeader(void *previous, void *next)
      : previousLeaf(previous), nextLeaf(next),
        keyCount(0) { }
};

#endif //PROJECT_LEAFHEADER_HPP
