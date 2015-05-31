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

/**
 * Entries less than the key are on the left, entries greater than or equal to the key are on the right.
 *
 * The first key in the entries array of InnerNodes and Leafs does not have a meaning,
 * it only fills up the array from the left with the additional necessary value.
 */
template<class KeyType, class KeyComparator>
class BTree {
public:
  static const uint64_t maxNodeCapacity = (BufferManager::DATA_SIZE_BYTE - sizeof(InnerNode<KeyType>::Header))
                                          / sizeof(Entry<KeyType, void *>)
                                          - 1 /* one less for the additional K/V pair */;
  static const uint64_t minNodeCapacity = maxNodeCapacity / 2;
  static const uint64_t maxLeafCapacity = (BufferManager::DATA_SIZE_BYTE - sizeof(InnerNode<KeyType>::Header))
                                          / sizeof(Entry<KeyType, TID>)
                                          - 1 /* one less for the additional K/V pair */;
  static const uint64_t minLeafCapacity = maxLeafCapacity / 2;

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

  inline bool searchForKey(KeyType key, TID &tid, void *node, uint64_t depth);

  inline bool searchLeafForKey(KeyType key, TID &tid, Leaf<KeyType> *leaf);

  inline bool searchNodeForKey(KeyType key, TID &tid, InnerNode<KeyType> *node, uint64_t depth);

public:
  BTree(BufferManager &bManager, uint64_t segmentId) : bufferManager(bManager), segmentId(segmentId) { }

  inline bool insert(KeyType key, TID tid);

  inline bool lookup(KeyType key, TID &tid);

  inline std::vector<TID>::iterator lookupRange(KeyType key);

  inline bool erase(KeyType key);

  inline uint64_t size();

  inline void visualize();

  inline bool isLeafHeight(size_t height);

  inline void splitInnerNode(InnerNode<KeyType> *innerNode, InnerNode<KeyType> *parentNode);

  inline void splitLeaf(Leaf<KeyType> *leaf, InnerNode<KeyType> *parentNode);
};

#include "BTree.inl.cpp"
#include "BtreeVisualize.inl.cpp"

#endif //PROJECT_BTREE_H
