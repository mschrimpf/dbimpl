//
// Created by daniel on 5/19/15.
//

#include "BTree.hpp"
#include <sstream>
#include <stdio.h>

template<class KeyType, class KeyComparator>
inline BTree<KeyType, KeyComparator>::BTree(BufferManager &bManager, uint64_t segmentId)
    : bufferManager(bManager), segmentId(segmentId), lastPageId(0) {
  this->rootPageId = nextPageId();
  Leaf<KeyType, KeyComparator> leaf(nullptr, nullptr);
  BufferFrame &frame = bufferManager.fixPage(this->segmentId, rootPageId, true);
  char *data = (char *) frame.getData();
  memcpy(data, &leaf, sizeof(leaf));
}

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
  InnerNode<KeyType, KeyComparator> *parentNode = nullptr;
  InnerNode<KeyType, KeyComparator> *currNode = nullptr;

  while (!isLeafHeight(currHeight)) {
    if (parentFrame != nullptr) {
      bufferManager.unfixPage(*parentFrame, true);
    }
    parentFrame = currFrame;
    parentNode = currNode;
    currFrame = &bufferManager.fixPage(this->segmentId, currPageId, true);
    currNode = reinterpret_cast<InnerNode<KeyType, KeyComparator> *>(currFrame->getData());
    if (!currNode->hasSpaceForOneMoreEntry()) {
      FrameNode<KeyType, KeyComparator> frameNode = splitInnerNode(currNode, parentNode);
      currFrame = frameNode.frame;
      currNode = frameNode.node;
      continue;
    }
    currPageId = currNode->getNextNode(key);
    currHeight++;
  }
  // we are now at leaf height - the currPageId points to a leaf
  if (parentFrame != nullptr) {
    bufferManager.unfixPage(*parentFrame, true);
  }
  parentFrame = currFrame;
  parentNode = currNode;
  currNode = nullptr;

  currFrame = &bufferManager.fixPage(this->segmentId, currPageId, true);
  Leaf<KeyType, KeyComparator> *leaf = reinterpret_cast<Leaf<KeyType, KeyComparator> *>(currFrame->getData());
  if (!leaf->hasSpaceForOneMoreEntry()) {
    FrameLeaf<KeyType, KeyComparator> frameLeaf = splitLeaf(leaf, parentNode, key);
    currFrame = frameLeaf.frame;
    leaf = frameLeaf.leaf;
  }
  if (parentFrame != nullptr) {
    bufferManager.unfixPage(*parentFrame, true);
  }
  leaf->insertDefiniteFit(key, tid);
  bufferManager.unfixPage(*currFrame, true);
  return true;
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
    Leaf<KeyType, KeyComparator> *leaf = reinterpret_cast<Leaf<KeyType, KeyComparator> *>(node);
    return searchLeafForKey(key, tid, leaf);
  } else {
    //we haven't reached the leaves yet
    InnerNode<KeyType, KeyComparator> *curNode = reinterpret_cast<InnerNode<KeyType, KeyComparator> *> (node);
    return searchNodeForKey(key, tid, curNode, currentDepth);
  }
}

template<class KeyType, class KeyComparator>
inline bool BTree<KeyType, KeyComparator>::searchNodeForKey(KeyType key, TID &tid,
                                                            InnerNode<KeyType, KeyComparator> *node, uint64_t depth) {
  uint64_t left = 0;
  uint64_t right = node->header.keyCount;
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
  if (middle < node->header.keyCount) {
    //value found
    return searchForKey(key, tid, (Leaf<KeyType, KeyComparator> *) node->entries[middle].value, depth + 1);
  }
  return false;
}

template<class KeyType, class KeyComparator>
inline bool BTree<KeyType, KeyComparator>::searchLeafForKey(KeyType key, TID &tid, Leaf<KeyType, KeyComparator> *leaf) {
  uint64_t left = 0;
  uint64_t right = leaf->header.keyCount;
  uint64_t middle = (left + right) / 2;

  while (left != right) {
    if (comparator(leaf->entries[middle].key, key)) {
      left = middle + 1;
    } else {
      right = middle;
    }
    middle = (left + right) / 2;
  }
  if (middle < leaf->header.keyCount) {
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
  return this->treeSize;
}

template<class KeyType, class KeyComparator>
inline bool BTree<KeyType, KeyComparator>::isLeafHeight(size_t height) {
  return height == this->height;
}

template<class KeyType, class KeyComparator>
inline FrameNode<KeyType, KeyComparator> BTree<KeyType, KeyComparator>::splitInnerNode(
    InnerNode<KeyType, KeyComparator> *innerNode,
    InnerNode<KeyType, KeyComparator> *parentNode) {

}

template<class KeyType, class KeyComparator>
inline FrameLeaf<KeyType, KeyComparator> BTree<KeyType, KeyComparator>::splitLeaf(Leaf<KeyType, KeyComparator> *leaf,
                                                                                  InnerNode<KeyType, KeyComparator> *parentNode,
                                                                                  KeyType key) {

}

template<class KeyType, class KeyComparator>
inline uint64_t BTree<KeyType, KeyComparator>::nextPageId() {
  return lastPageId++;
}
