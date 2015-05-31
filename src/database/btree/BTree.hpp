//
// Created by daniel on 5/19/15.
//

#ifndef PROJECT_BTREE_H
#define PROJECT_BTREE_H

#include <stdint.h>
#include <vector>
#include "Entry.hpp"
#include "InnerNode.hpp"
#include "Leaf.hpp"
#include "../buffer/BufferManager.hpp"
#include "../slotted_pages/TID.hpp"

template<class KeyType, class KeyComparator>
struct FrameNode {
  BufferFrame *frame;
  InnerNode<KeyType, KeyComparator> *node;
};
template<class KeyType, class KeyComparator>
struct FrameLeaf {
  BufferFrame *frame;
  Leaf<KeyType, KeyComparator> *leaf;
};

/**
 * Entries less than the key are on the left, entries greater than or equal to the key are on the right.
 *
 * The first key in the entries array of InnerNodes and Leafs does not have a meaning,
 * it only fills up the array from the left with the additional necessary value.
 */
template<class KeyType, class KeyComparator>
class BTree {
private:
  BufferManager &bufferManager;
  uint64_t segmentId;
  KeyComparator comparator;

  uint64_t rootPageId : 48;
  size_t treeSize; // number of elements inside of the tree
  /**
   * Longest path from the root to one of the leafs.
   * I.e., a tree with only the root leaf has a height of 0.
   */
  size_t height;

  uint64_t lastPageId;

  inline bool searchForKey(KeyType key, TID &tid, void *node, uint64_t depth);

  inline bool searchLeafForKey(KeyType key, TID &tid, Leaf<KeyType, KeyComparator> *leaf);

  inline bool searchNodeForKey(KeyType key, TID &tid,
                               InnerNode<KeyType, KeyComparator> *node, uint64_t depth);

  inline bool isLeafHeight(size_t height);

  inline FrameNode<KeyType, KeyComparator> splitInnerNode(InnerNode<KeyType, KeyComparator> *innerNode,
                                  InnerNode<KeyType, KeyComparator> *parentNode);

  /**
   * Returns the page id of the leaf that contains the key
   */
  inline FrameLeaf<KeyType, KeyComparator> splitLeaf(Leaf<KeyType, KeyComparator> *leaf,
                             InnerNode<KeyType, KeyComparator> *parentNode,
                             KeyType key);

  inline uint64_t nextPageId();

public:
  inline BTree(BufferManager &bManager, uint64_t segmentId);

  inline bool insert(KeyType key, TID tid);

  inline bool lookup(KeyType key, TID &tid);

  inline std::vector<TID>::iterator lookupRange(KeyType key);

  inline bool erase(KeyType key);

  inline uint64_t size();

  inline void visualize();
};

#include "BTree.inl.cpp"
#include "BtreeVisualize.inl.cpp"

#endif //PROJECT_BTREE_H
