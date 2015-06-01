//
// Created by daniel on 5/19/15.
//

#include "BTree.hpp"
#include <sstream>
#include <stdio.h>

template<typename KeyType, typename KeyComparator>
inline BTree<KeyType, KeyComparator>::BTree(BufferManager &bManager, uint64_t segmentId, KeyComparator &smaller)
    : bufferManager(bManager), segmentId(segmentId), smallerComparator(smaller),
      lastPageId(0), treeSize(0), height(0) {
  this->rootPageId = nextPageId();
  Leaf<KeyType, KeyComparator> leaf(LeafHeader::INVALID_PAGE_ID, LeafHeader::INVALID_PAGE_ID);
  BufferFrame &frame = bufferManager.fixPage(this->segmentId, rootPageId, true);
  char *data = (char *) frame.getData();
  memcpy(data, &leaf, sizeof(leaf));
}

/**
 * Note: this implementation assumes that all keys are unique.
 */
template<typename KeyType, typename KeyComparator>
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
      FrameNode<KeyType, KeyComparator> frameNode = splitInnerNode(currNode, currPageId, parentNode);
      currFrame = frameNode.frame;
      currNode = frameNode.node;
      continue;
    }
    currPageId = currNode->getNextNode(key, this->smallerComparator);
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
    FrameLeaf<KeyType, KeyComparator> frameLeaf = splitLeaf(leaf, currFrame, currPageId, parentNode, key);
    currFrame = frameLeaf.frame;
    leaf = frameLeaf.leaf;
  }
  if (parentFrame != nullptr) {
    bufferManager.unfixPage(*parentFrame, true);
  }
  leaf->insertDefiniteFit(key, tid);
  bufferManager.unfixPage(*currFrame, true);
  treeSize++;
  return true;
}

template<typename KeyType, typename KeyComparator>
inline bool BTree<KeyType, KeyComparator>::erase(KeyType key) {
  Leaf<KeyType, KeyComparator> &leaf = getLeaf(key);
  leaf.erase(key);
  return true;
}

template<class KeyType, class KeyComparator>
inline std::vector<TID>::iterator BTree<KeyType, KeyComparator>::lookupRange(KeyType begin, KeyType end) {
  KeyType left = begin;
  KeyType right = end;

  /* if given end-limit is lower than begin, we need to swap both borders*/
  if (comparator(end, begin)){
    left = end;
    right = begin;
  }

  std::vector<TID> vec;
  return vec.begin();
}


template<typename KeyType, typename KeyComparator>
inline bool BTree<KeyType, KeyComparator>::lookup(KeyType key, TID &tid) {
  // start from the root node and receive frame */
  bool result = searchForKey(key, tid, rootPageId, 0);
  return result;
}

template<typename KeyType, typename KeyComparator>
inline bool BTree<KeyType, KeyComparator>::searchForKey(KeyType key, TID &tid, uint64_t pageId, uint64_t currentDepth) {
  BufferFrame * currentFrame = &bufferManager.fixPage(this->segmentId, pageId, false);
  bool result = false;
  if (currentDepth == height) {
    //we reached maximum height, so nodes are leaves
    Leaf<KeyType, KeyComparator> *leaf = reinterpret_cast<Leaf<KeyType, KeyComparator> *>(currentFrame->getData());
    result = searchLeafForKey(key, tid, leaf);
  } else {
    //we haven't reached the leaves yet
    InnerNode<KeyType, KeyComparator> *curNode = reinterpret_cast<InnerNode<KeyType, KeyComparator> *> (currentFrame->getData());
    result = searchNodeForKey(key, tid, curNode, currentDepth);
  }

  //return page as result was received and page is no longer required
  bufferManager.unfixPage(* currentFrame, false);
  return result;
}

template<typename KeyType, typename KeyComparator>
inline bool BTree<KeyType, KeyComparator>::searchNodeForKey(KeyType key, TID &tid,
                                                            InnerNode<KeyType, KeyComparator> *node, uint64_t depth) {
  uint64_t left = 0;
  uint64_t right = node->header.keyCount;
  uint64_t middle = (left + right) / 2;

  while (left != right) {
    if (smallerComparator(node->entries[middle].key, key)) {
      //move left end nearer to right end
      left = middle + 1;
    } else {
      right = middle;
    }
    middle = (left + right) / 2;
  }
  if (middle < node->header.keyCount) {
    //value found

    return searchForKey(key, tid, node->entries[middle].value, depth + 1);
  }
  return false;
}

template<typename KeyType, typename KeyComparator>
inline bool BTree<KeyType, KeyComparator>::searchLeafForKey(KeyType key, TID &tid, Leaf<KeyType, KeyComparator> *leaf) {
  uint64_t left = 0;
  uint64_t right = leaf->header.keyCount;
  uint64_t middle = (left + right) / 2;

  while (left != right) {
    if (smallerComparator(leaf->entries[middle].key, key)) {
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

template<typename KeyType, typename KeyComparator>
inline uint64_t BTree<KeyType, KeyComparator>::size() {
  return this->treeSize;
}

template<typename KeyType, typename KeyComparator>
inline bool BTree<KeyType, KeyComparator>::isLeafHeight(size_t height) {
  return height == this->height;
}

template<typename KeyType, typename KeyComparator>
inline FrameNode<KeyType, KeyComparator> BTree<KeyType, KeyComparator>::splitInnerNode
    (InnerNode<KeyType, KeyComparator> *node, uint64_t nodePageId,
     InnerNode<KeyType, KeyComparator> *parent) {
  // invariant: parent node has space for one more entry
  InnerNode<KeyType, KeyComparator> newNode;
  size_t arraySplitIndex = node->header.keyCount / 2 + 1 /* first key value pair */;
  size_t splitLength = node->header.keyCount - arraySplitIndex;
  KeyType splitKey = node->entries[arraySplitIndex].key;
  uint64_t newPageId = nextPageId();

  memcpy(newNode.entries, node->entries + arraySplitIndex, splitLength);
  node->header.keyCount = arraySplitIndex;
  newNode.header.keyCount = splitLength;

  parent->insertDefiniteFit(splitKey, nodePageId, newPageId, smallerComparator);

  BufferFrame &newFrame = bufferManager.fixPage(this->segmentId, newPageId, true);
  void *newFrameData = newFrame.getData();
  memcpy(newFrameData, &newNode, sizeof(newNode));

  return FrameNode<KeyType, KeyComparator>{
      &newFrame /* unfixed in calling method */,
      reinterpret_cast<InnerNode<KeyType, KeyComparator> *>(newFrameData)
  };
}

template<typename KeyType, typename KeyComparator>
inline FrameLeaf<KeyType, KeyComparator> BTree<KeyType, KeyComparator>::splitLeaf
    (Leaf<KeyType, KeyComparator> *leaf, BufferFrame *leafFrame, uint64_t leafPageId,
     InnerNode<KeyType, KeyComparator> *parent,
     KeyType key) {
  // invariant: parent node has space for one more entry
  Leaf<KeyType, KeyComparator> newLeaf(leafPageId, LeafHeader::INVALID_PAGE_ID);
  size_t arraySplitIndex = leaf->header.keyCount / 2 + 1 /* first key value pair */;
  size_t splitLength = leaf->header.keyCount - arraySplitIndex;
  KeyType splitKey = leaf->entries[arraySplitIndex].key;
  uint64_t newPageId = nextPageId();

  memcpy(newLeaf.entries, leaf->entries + arraySplitIndex, splitLength);
  leaf->header.keyCount = arraySplitIndex;
  newLeaf.header.keyCount = splitLength;

  parent->insertDefiniteFit(splitKey, leafPageId, newPageId, smallerComparator);

  BufferFrame &newFrame = bufferManager.fixPage(this->segmentId, newPageId, true);
  void *newFrameData = newFrame.getData();
  memcpy(newFrameData, &newLeaf, sizeof(newLeaf));

  if (smallerComparator(key, splitKey)) {
    bufferManager.unfixPage(newFrame, true);
    return FrameLeaf<KeyType, KeyComparator>{
        leafFrame,
        leaf
    };
  } else {
    bufferManager.unfixPage(*leafFrame, true);
    return FrameLeaf<KeyType, KeyComparator>{
        &newFrame /* unfixed in calling method */,
        reinterpret_cast<Leaf<KeyType, KeyComparator> *>(newFrameData)
    };
  }
}

template<typename KeyType, typename KeyComparator>
inline uint64_t BTree<KeyType, KeyComparator>::nextPageId() {
  return lastPageId++;
}

template<typename KeyType, typename KeyComparator>
Leaf<KeyType, KeyComparator> &BTree<KeyType, KeyComparator>::getLeaf(KeyType key) {
  // TODO
}
