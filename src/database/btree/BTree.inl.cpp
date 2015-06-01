//
// Created by daniel on 5/19/15.
//

#include "BTree.hpp"
#include <sstream>
#include <stdio.h>

template<typename KeyType, typename KeyComparator>
inline BTree<KeyType, KeyComparator>::BTree(BufferManager &bManager, uint64_t segmentId)
    : bufferManager(bManager), segmentId(segmentId),
      lastPageId(0), treeSize(0), height(0) {
  this->rootPageId = nextPageId();
  Leaf<KeyType, KeyComparator> leaf(nullptr, nullptr);
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
inline std::vector<TID> BTree<KeyType, KeyComparator>::lookupRange(KeyType begin, KeyType end) {
    KeyType left = begin;
    KeyType right = end;

    /* if given end-limit is lower than begin, we need to swap both borders*/
    if (comparator(end, begin)) {
        left = end;
        right = begin;
    }

    std::vector<TID> lookupSet;
    Leaf<KeyType, KeyComparator> leftLeaf = getLeaf(left);
    int position = findPosition<KeyType, KeyComparator, TID>(leftLeaf.entries, left, 1, leftLeaf.header.keyCount + 1);
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
            uint64_t * nextLeaf = leftLeaf.header.nextLeafPageId;
            if (nextLeaf != nullptr){
                // set next leaf and reset position to first entry
                leftLeaf =  getLeaf(* nextLeaf);
                position = 1;
            }else{
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
BufferFrame * BTree<KeyType, KeyComparator>::findFrameForKey(KeyType key, bool exclusive){
    BufferFrame *currentFrame = &bufferManager.fixPage(this->segmentId, rootPageId, exclusive);
    int currentDepth = 0;
    BufferFrame * parentFrame = nullptr;
    while (currentDepth != height){
        InnerNode<KeyType, KeyComparator> *curNode = reinterpret_cast<InnerNode<KeyType, KeyComparator> *> (currentFrame->getData());
        if (parentFrame != nullptr){
            bufferManager.unfixPage(* parentFrame, false);
        }
        int curPosition = findPosition<KeyType, KeyComparator, uint64_t>(curNode->entries, key, 1, curNode->header.keyCount + 1);
        if (curPosition < curNode->header.keyCount){
            Entry<KeyType, uint64_t> entry = curNode->entries[curPosition];
            uint64_t pageId = entry.value;
            parentFrame = currentFrame;
            currentFrame = &bufferManager.fixPage(this->segmentId, pageId, exclusive);
        }else{
            //value not found
            //should never happen?
        }
        currentDepth++;
    }
    if (parentFrame != nullptr){
        bufferManager.unfixPage(* parentFrame, false);
    }
    //frame is fixed and has to be unfixed by the caller!!
    return currentFrame;
}

template<typename KeyType, typename KeyComparator>
inline bool BTree<KeyType, KeyComparator>::searchForKey(KeyType key, TID &tid, uint64_t pageId, uint64_t currentDepth) {
    BufferFrame *currentFrame = &bufferManager.fixPage(this->segmentId, pageId, false);
    bool result = false;
    if (currentDepth == height) {
        //we reached maximum height, so nodes are leaves
        Leaf<KeyType, KeyComparator> *leaf = reinterpret_cast<Leaf<KeyType, KeyComparator> *>(currentFrame->getData());
        try {
            tid = searchValue<KeyType, KeyComparator, TID>(leaf->entries, key, 1, leaf->header.keyCount + 1);
            result = true;
        } catch (const std::invalid_argument &ia) {
            result = false;
        }
    } else {
        //we haven't reached the leaves yet
        InnerNode<KeyType, KeyComparator> *curNode = reinterpret_cast<InnerNode<KeyType, KeyComparator> *> (currentFrame->getData());
        try {
            uint64_t pageId = searchValue<KeyType, KeyComparator, uint64_t>(curNode->entries, key, 1, curNode->header.keyCount + 1);
            result = searchForKey(key, tid, pageId, currentDepth + 1);
        } catch (const std::invalid_argument &ia) {
            result = false;
        }
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

  parent->insertDefiniteFit(splitKey, nodePageId, newPageId);

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
  Leaf<KeyType, KeyComparator> newLeaf(& leafPageId, nullptr);
  size_t arraySplitIndex = leaf->header.keyCount / 2 + 1 /* first key value pair */;
  size_t splitLength = leaf->header.keyCount - arraySplitIndex;
  KeyType splitKey = leaf->entries[arraySplitIndex].key;
  uint64_t newPageId = nextPageId();

  memcpy(newLeaf.entries, leaf->entries + arraySplitIndex, splitLength);
  leaf->header.keyCount = arraySplitIndex;
  newLeaf.header.keyCount = splitLength;

  parent->insertDefiniteFit(splitKey, leafPageId, newPageId);

  BufferFrame &newFrame = bufferManager.fixPage(this->segmentId, newPageId, true);
  void *newFrameData = newFrame.getData();
  memcpy(newFrameData, &newLeaf, sizeof(newLeaf));

  if (key < splitKey) {
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
    BufferFrame * frame = findFrameForKey(key, false);
    Leaf<KeyType, KeyComparator> *leaf = reinterpret_cast<Leaf<KeyType, KeyComparator> *>(frame->getData());
    bufferManager.unfixPage(* frame, false);
    return * leaf;
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
        Entry<KeyType, uint64_t> entry = curNode->entries[1]; //most left value = 1
        uint64_t pageId = entry.value;
        parentFrame = currentFrame;
        currentFrame = &bufferManager.fixPage(this->segmentId, pageId, false);
        currentDepth++;
    }
    if (parentFrame != nullptr) {
        bufferManager.unfixPage(*parentFrame, false);
    }

    Leaf<KeyType, KeyComparator> * leaf = reinterpret_cast<Leaf<KeyType, KeyComparator> *>(currentFrame->getData());
    bufferManager.unfixPage(* currentFrame, false);
    return * leaf;
}

template<typename KeyType, typename KeyComparator>
Leaf<KeyType, KeyComparator> BTree<KeyType, KeyComparator>::getMostRightLeaf() {
    BufferFrame *currentFrame = &bufferManager.fixPage(this->segmentId, rootPageId, false);
    int currentDepth = 0;
    BufferFrame *parentFrame = nullptr;
    while (currentDepth != height) {
        InnerNode<KeyType, KeyComparator> *curNode = reinterpret_cast<InnerNode<KeyType, KeyComparator> *> (currentFrame->getData());
        if (parentFrame != nullptr) {
            bufferManager.unfixPage(*parentFrame, false);
        }
        Entry<KeyType, uint64_t> entry = curNode->entries[curNode->header.keyCount]; //most right value = 1 //TODO vllt header.keyCount + 1!!!
        uint64_t pageId = entry.value;
        parentFrame = currentFrame;
        currentFrame = &bufferManager.fixPage(this->segmentId, pageId, false);
        currentDepth++;
    }
    if (parentFrame != nullptr) {
        bufferManager.unfixPage(*parentFrame, false);
    }

    Leaf<KeyType, KeyComparator> *leaf = reinterpret_cast<Leaf<KeyType, KeyComparator> *>(currentFrame->getData());
    Entry<KeyType, TID> entry = leaf->entries[leaf->header.keyCount]; //TODO vllt header.keyCount + 1!!!
    bufferManager.unfixPage(* currentFrame, false);
    return entry;
}