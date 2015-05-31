//
// Created by daniel on 5/19/15.
//

#include <sstream>
#include "BTree.hpp"

template<class KeyType, class KeyComparator>
inline bool BTree<KeyType, KeyComparator>::erase(KeyType key) {
  //Remove first
  //Check if Node Size < min-Capacity
  //no -> we're done
  //yes -> merge with left neighbour, if not exist, with right neighbour
  // check if we need to split afterwards.
  return true;
}

/**
 * Note: this implementation assumes that all keys are unique.
 */
template<class KeyType, class KeyComparator>
inline bool BTree<KeyType, KeyComparator>::insert(KeyType key, TID tid) {
  size_t currHeight = 0;
  uint64_t currPageId = rootPageId;
  BufferFrame *parentFrame = nullptr;
  BufferFrame *currFrame = nullptr;
  InnerNode<KeyType> *parentNode = nullptr;
  InnerNode<KeyType> *currNode = nullptr;

  while (!isLeafHeight(currHeight)) {
    if (parentFrame != nullptr) {
      bufferManager.unfixPage(*parentFrame, true);
    }
    parentFrame = currFrame;
    currFrame = &bufferManager.fixPage(this->segmentId, currPageId, true);
    parentNode = currNode;
    currNode = reinterpret_cast<InnerNode<KeyType> *>(currFrame->getData());
    if (!currNode->hasSpaceForOneMoreEntry()) {
      splitInnerNode(currNode, parentNode);
      currNode = parentNode; // do not advance the loop
      continue;
    }
    currPageId = currNode->getNextNode(key);
    currHeight++;
  }
  currFrame = &bufferManager.fixPage(this->segmentId, currPageId, true);
  Leaf<KeyType> *leaf = reinterpret_cast<Leaf<KeyType> *>(currFrame->getData());
  if (!leaf->hasSpaceForOneMoreEntry()) {
    splitLeaf(leaf, currNode);
    bufferManager.unfixPage(*currFrame, true);
    uint64_t leafPageId = currNode->getNextNode(key);
    currFrame = &bufferManager.fixPage(this->segmentId, leafPageId, true);
  }
  leaf->insert(key, tid);
  bufferManager.unfixPage(*currFrame, true);
}

template<class KeyType, class KeyComparator>
inline std::vector<TID>::iterator BTree<KeyType, KeyComparator>::lookupRange(KeyType key) {
  return NULL;
}


template<class KeyType, class KeyComparator>
inline bool BTree<KeyType, KeyComparator>::lookup(KeyType key, TID &tid) {
  if (height == 0) {
    // no element found
    return false;
  }
  return false;
//	return searchForKey(key, tid, rootNode, 1); // TODO
}

template<class KeyType, class KeyComparator>
inline bool BTree<KeyType, KeyComparator>::searchForKey(KeyType key, TID &tid, void *node, uint64_t currentDepth) {
  if (currentDepth == height) {
    //we reached maximum height, so nodes are leaves
    Leaf<KeyType> *leaf = reinterpret_cast<Leaf<KeyType> *>(node);
    return searchLeafForKey(key, tid, leaf);
  } else {
    //we haven't reached the leaves yet
    InnerNode<KeyType> *curNode = reinterpret_cast<InnerNode<KeyType> *> (node);
    return searchNodeForKey(key, tid, curNode, currentDepth);
  }
}

template<class KeyType, class KeyComparator>
inline bool BTree<KeyType, KeyComparator>::searchNodeForKey(KeyType key, TID &tid, InnerNode<KeyType> *node, uint64_t depth) {
  uint64_t left = 0;
  uint64_t right = node->header.count;
  uint64_t middle = (left + right) / 2;

  while (left != right) {
    if (comparator(node->entries[middle].key, key)) {
      //move left end nearer to right end
      left = middle + 1;
    } else {
      right = middle;
    }
    middle = (left + right) / 2;
  }
  if (middle < node->header.count) {
    //value found
    return searchForKey(key, tid, node->entries[middle].value, depth + 1);
  }
  return false;
}

template<class KeyType, class KeyComparator>
inline bool BTree<KeyType, KeyComparator>::searchLeafForKey(KeyType key, TID &tid, Leaf<KeyType> *leaf) {
  uint64_t left = 0;
  uint64_t right = leaf->header.count;
  uint64_t middle = (left + right) / 2;

  while (left != right) {
    if (comparator(leaf->entries[middle].key, key)) {
      left = middle + 1;
    } else {
      right = middle;
    }
    middle = (left + right) / 2;
  }
  if (middle < leaf->header.count) {
    //value found
    Entry<KeyType, TID> foundEntry = leaf->entries[middle];
    tid = foundEntry.value;
    return true;
  } else {
    //value not found
    return false;
  }
}

template<class KeyType, class KeyComparator>
inline uint64_t BTree<KeyType, KeyComparator>::size() {
  return 0;
}

template<class KeyType, class KeyComparator>
inline bool BTree<KeyType, KeyComparator>::isLeafHeight(size_t height) {
  return height == this->height;
}

template<class KeyType, class KeyComparator>
inline void BTree<KeyType, KeyComparator>::splitInnerNode(InnerNode<KeyType> *innerNode, InnerNode<KeyType> *parentNode) {

}

template<class KeyType, class KeyComparator>
inline void BTree<KeyType, KeyComparator>::splitLeaf(Leaf<KeyType> *leaf, InnerNode<KeyType> *parentNode) {

}
