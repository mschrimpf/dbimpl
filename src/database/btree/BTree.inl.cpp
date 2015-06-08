//
// Created by daniel on 5/19/15.
//

#include "BTree.hpp"
#include <sstream>
#include <stdio.h>
#include <memory.h>

template<typename KeyType, typename KeyComparator>
BTree<KeyType, KeyComparator>::BTree(BufferManager &bManager, uint64_t segmentId, KeyComparator &smaller)
    : bufferManager(bManager), segmentId(segmentId), smallerComparator(smaller),
      lastPageId(0), treeSize(0), height(0) {
  this->rootPageId = nextPageId();
  Leaf<KeyType, KeyComparator> leaf(LeafHeader::INVALID_PAGE_ID, LeafHeader::INVALID_PAGE_ID);
  BufferFrame &rootFrame = bufferManager.fixPage(this->segmentId, rootPageId, true);
  char *data = (char *) rootFrame.getData();
  memcpy(data, &leaf, sizeof(leaf));
  bufferManager.unfixPage(rootFrame, true);
}

/**
 * Note: this implementation assumes that all keys are unique.
 */
template<typename KeyType, typename KeyComparator>
inline void BTree<KeyType, KeyComparator>::insert(KeyType key, TID tid) {
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
      if (parentNode == nullptr) {
        auto newNode = createEmptyNode(currPageId);
        parentFrame = newNode.first;
        parentNode = newNode.second;
        currHeight++;
      }
      auto splitResult = splitInnerNode(currNode, currFrame, currPageId,
                                        parentNode, key);
      currFrame = splitResult.first;
      currNode = splitResult.second;
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
  auto leaf = reinterpret_cast<Leaf<KeyType, KeyComparator> *>(currFrame->getData());
  if (!leaf->hasSpaceForOneMoreEntry()) {
    if (parentNode == nullptr) {
      auto newNode = createEmptyNode(currPageId);
      parentFrame = newNode.first;
      parentNode = newNode.second;
    }

    auto splitResult = splitLeaf(leaf, currFrame, currPageId, parentNode, key);
    currFrame = splitResult.first;
    leaf = splitResult.second;
  }
  if (parentFrame != nullptr) {
    bufferManager.unfixPage(*parentFrame, true);
  }
  leaf->insertDefiniteFit(key, tid, smallerComparator);
  bufferManager.unfixPage(*currFrame, true);
  treeSize++;
}

template<typename KeyType, typename KeyComparator>
inline std::pair<BufferFrame *, InnerNode<KeyType, KeyComparator> *>
BTree<KeyType, KeyComparator>::splitInnerNode
    (InnerNode<KeyType, KeyComparator> *node, BufferFrame *nodeFrame, uint64_t nodePageId,
     InnerNode<KeyType, KeyComparator> *parent,
     KeyType key) {
  // invariant: parent node has space for one more entry
  InnerNode<KeyType, KeyComparator> newNode;
  size_t keysHalf = node->header.keyCount / 2;
  size_t splitKeyIndex = keysHalf + 1 /* first key value pair */;
  KeyType splitKey = node->entries[splitKeyIndex].key;
  size_t newKeysCount = node->header.keyCount - keysHalf;
  size_t entrySize = sizeof(node->entries[0]);
  uint64_t newPageId = nextPageId();

  memcpy(newNode.entries /* start with the first key/value pair */,
         node->entries + keysHalf /* begin with the first key/value pair (key is irrelevant) */,
         (newKeysCount + 1) * entrySize /* +1 since the first entry is not relevant */);

  node->header.keyCount = keysHalf;
  newNode.header.keyCount = newKeysCount;

  parent->insertDefiniteFit(splitKey, nodePageId, newPageId, smallerComparator);

  BufferFrame &newFrame = bufferManager.fixPage(this->segmentId, newPageId, true);
  void *newFrameData = newFrame.getData();
  memcpy(newFrameData, &newNode, sizeof(newNode));

  if (smallerComparator(key, splitKey)) {
    bufferManager.unfixPage(newFrame, true);
    return std::make_pair(
        nodeFrame,
        node
    );
  } else {
    bufferManager.unfixPage(*nodeFrame, true);
    return std::make_pair(
        &newFrame /* unfixed in calling method */,
        reinterpret_cast<InnerNode<KeyType, KeyComparator> *>(newFrameData));
  }
}

template<typename KeyType, typename KeyComparator>
inline std::pair<BufferFrame *, Leaf<KeyType, KeyComparator> *>
BTree<KeyType, KeyComparator>::splitLeaf
    (Leaf<KeyType, KeyComparator> *leaf, BufferFrame *leafFrame, uint64_t leafPageId,
     InnerNode<KeyType, KeyComparator> *parent,
     KeyType key) {
  if (parent == nullptr) {
    throw std::invalid_argument("Parent is null");
  }

  // invariant: parent node has space for one more entry
  Leaf<KeyType, KeyComparator> newLeaf(leafPageId, LeafHeader::INVALID_PAGE_ID);
  size_t arraySplitIndex = leaf->header.keyCount / 2;
  size_t splitLength = leaf->header.keyCount - arraySplitIndex;
  KeyType splitKey = leaf->entries[arraySplitIndex].key;
  size_t entrySize = sizeof(leaf->entries[0]);
  uint64_t newPageId = nextPageId();
  leaf->header.nextLeafPageId = newPageId;

  memcpy(newLeaf.entries, leaf->entries + arraySplitIndex, splitLength * entrySize);
  leaf->header.keyCount = arraySplitIndex;
  newLeaf.header.keyCount = splitLength;

  parent->insertDefiniteFit(splitKey, leafPageId, newPageId, smallerComparator);

  BufferFrame &newFrame = bufferManager.fixPage(this->segmentId, newPageId, true);
  void *newFrameData = newFrame.getData();
  memcpy(newFrameData, &newLeaf, sizeof(newLeaf));

  if (smallerComparator(key, splitKey)) {
    bufferManager.unfixPage(newFrame, true);
    return std::make_pair(
        leafFrame,
        leaf);
  } else {
    bufferManager.unfixPage(*leafFrame, true);
    return std::make_pair(
        &newFrame,
        reinterpret_cast<Leaf<KeyType, KeyComparator> *>(newFrameData));
  }
}

template<typename KeyType, typename KeyComparator>
std::pair<BufferFrame *, InnerNode<KeyType, KeyComparator> *>
BTree<KeyType, KeyComparator>::createEmptyNode(
    uint64_t currPageId) {
  uint64_t newPageId = nextPageId();
  InnerNode<KeyType, KeyComparator> newNode;
  BufferFrame &newFrame = bufferManager.fixPage(this->segmentId, newPageId, true);
  void *frameData = newFrame.getData();
  memcpy(frameData, &newNode, sizeof(newNode));
  if (currPageId == rootPageId) {
    rootPageId = newPageId;
  }
  this->height++;
  return std::make_pair(
      &newFrame,
      reinterpret_cast<InnerNode<KeyType, KeyComparator> *>(frameData)
  );
}

template<typename KeyType, typename KeyComparator>
inline void BTree<KeyType, KeyComparator>::erase(KeyType key) {
  Leaf<KeyType, KeyComparator> &leaf = getLeaf(key);
  leaf.erase(key, smallerComparator);
  treeSize--;
}

template<class KeyType, class KeyComparator>
inline std::vector<TID> BTree<KeyType, KeyComparator>::lookupRange(KeyType begin, KeyType end) {
  KeyType left = begin;
  KeyType right = end;

  /* if given end-limit is lower than begin, we need to swap both borders*/
  if (smallerComparator(end, begin)) {
    left = end;
    right = begin;
  }

  std::vector<TID> lookupSet;
  Leaf<KeyType, KeyComparator> leftLeaf = getLeaf(left);
  int position = EntriesHelper::findPosition<KeyType, KeyComparator, TID>(
      leftLeaf.entries, left,
      1, leftLeaf.header.keyCount + 1,
      smallerComparator);
  while (true) {
    while (position < leftLeaf.header.keyCount + 1) {
      Entry<KeyType, TID> entry = leftLeaf.entries[position];
      if (entry.key >= begin && entry.key <= end) {
        lookupSet.push_back(entry.value);
      } else {
        return lookupSet;
      }
      position++;
    }
    if (position == leftLeaf.header.keyCount + 1) {
      //reached end of leaf and need to check the next leaf
      uint64_t nextLeaf = leftLeaf.header.nextLeafPageId;
      if (nextLeaf != LeafHeader::INVALID_PAGE_ID) {
        // set next leaf and reset position to first entry
        leftLeaf = getLeaf(nextLeaf);
        position = 1;
      } else {
        // end of leaves reached, we cannot look further so we return the set
        return lookupSet;
      }
    } else {
      return lookupSet;
    }
  }
}


template<typename KeyType, typename KeyComparator>
inline bool BTree<KeyType, KeyComparator>::lookup(KeyType key, TID &tid) {
  // start from the root node and receive frame */
  return searchForKey(key, tid, rootPageId, 0);
}

template<typename KeyType, typename KeyComparator>
BufferFrame *BTree<KeyType, KeyComparator>::findFrameForKey(KeyType key, bool exclusive) {
  BufferFrame *currentFrame = &bufferManager.fixPage(this->segmentId, rootPageId, exclusive);
  int currentDepth = 0;
  BufferFrame *parentFrame = nullptr;
  while (!isLeafHeight(currentDepth)) {
    InnerNode<KeyType, KeyComparator> *curNode = reinterpret_cast<InnerNode<KeyType, KeyComparator> *> (currentFrame->getData());
    if (parentFrame != nullptr) {
      bufferManager.unfixPage(*parentFrame, false);
    }
    int nextPageId = curNode->getNextNode(key, smallerComparator);
    parentFrame = currentFrame;
    currentFrame = &bufferManager.fixPage(this->segmentId, nextPageId, exclusive);
    currentDepth++;
  }
  if (parentFrame != nullptr) {
    bufferManager.unfixPage(*parentFrame, false);
  }
  //frame is fixed and has to be unfixed by the caller!!
  return currentFrame;
}

template<typename KeyType, typename KeyComparator>
inline bool BTree<KeyType, KeyComparator>::searchForKey(
    KeyType key, TID &tid, uint64_t pageId, size_t currentHeight) {
  BufferFrame *currentFrame = &bufferManager.fixPage(this->segmentId, pageId, false);
  bool result;
  if (isLeafHeight(currentHeight)) {
    Leaf<KeyType, KeyComparator> *leaf = reinterpret_cast<Leaf<KeyType, KeyComparator> *>(
        currentFrame->getData());
    result = leaf->lookup(key, smallerComparator, &tid);
  } else {
    //we haven't reached the leaves yet
    InnerNode<KeyType, KeyComparator> *currNode = reinterpret_cast<InnerNode<KeyType, KeyComparator> *> (
        currentFrame->getData());
    pageId = currNode->getNextNode(key, smallerComparator);
    result = searchForKey(key, tid, pageId, currentHeight + 1);
  }

  //return page as result was received and page is no longer required
  bufferManager.unfixPage(*currentFrame, false);
  return result;
}


template<typename KeyType, typename KeyComparator>
inline uint64_t BTree<KeyType, KeyComparator>::size() {
  return this->treeSize;
}

template<typename KeyType, typename KeyComparator>
inline bool BTree<KeyType, KeyComparator>::isLeafHeight(size_t height) {
  if (height > this->height) {
    throw std::invalid_argument("Height > this.height");
  }
  return height == this->height;
}

template<typename KeyType, typename KeyComparator>
inline uint64_t BTree<KeyType, KeyComparator>::nextPageId() {
  return lastPageId++;
}

template<typename KeyType, typename KeyComparator>
Leaf<KeyType, KeyComparator> &BTree<KeyType, KeyComparator>::getLeaf(KeyType key) {
  BufferFrame *frame = findFrameForKey(key, false);
  Leaf<KeyType, KeyComparator> *leaf = reinterpret_cast<Leaf<KeyType, KeyComparator> *>(frame->getData());
  bufferManager.unfixPage(*frame, false);
  return *leaf;
}

template<typename KeyType, typename KeyComparator>
Leaf<KeyType, KeyComparator> BTree<KeyType, KeyComparator>::getMostLeftLeaf() {
  BufferFrame *currentFrame = &bufferManager.fixPage(this->segmentId, rootPageId, false);
  int currentDepth = 0;
  BufferFrame *parentFrame = nullptr;
  while (currentDepth != height) {
    InnerNode<KeyType, KeyComparator> *curNode = reinterpret_cast<InnerNode<KeyType, KeyComparator> *> (currentFrame->getData());
    if (parentFrame != nullptr) {
      bufferManager.unfixPage(*parentFrame, false);
    }
    Entry<KeyType, uint64_t> entry = curNode->entries[0]; //most left value = 1
    uint64_t pageId = entry.value;
    parentFrame = currentFrame;
    currentFrame = &bufferManager.fixPage(this->segmentId, pageId, false);
    currentDepth++;
  }
  if (parentFrame != nullptr) {
    bufferManager.unfixPage(*parentFrame, false);
  }

  Leaf<KeyType, KeyComparator> *leaf = reinterpret_cast<Leaf<KeyType, KeyComparator> *>(currentFrame->getData());
  bufferManager.unfixPage(*currentFrame, false);
  return *leaf;
}

