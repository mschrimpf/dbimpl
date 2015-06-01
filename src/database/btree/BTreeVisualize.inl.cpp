//
// Created by Martin on 2015-05-27
//

#include <sstream>
#include <iostream>
#include "BTree.hpp"

template<typename KeyType, typename KeyComparator>
void BTree<KeyType, KeyComparator>::visualize() {
  std::stringstream stream;
  stream << "diagraph myBTree { \n";
  stream << "node [shape=record];";
  BufferFrame frame = bufferManager.fixPage(segmentId, rootPageId, false);
  void *rootNode = frame.getData(); // TODO: retrieve from rootPageId
  if (height == 0) {
    Leaf<KeyType, KeyComparator> *leaf = reinterpret_cast<Leaf<KeyType, KeyComparator> *>(rootNode);
    leaf->visualize(0);
  } else {
    InnerNode<KeyType, KeyComparator> *node = reinterpret_cast<InnerNode<KeyType, KeyComparator> *> (rootNode);
    node->visualize(0, 0, 0, height);
  }
    bufferManager.unfixPage(frame, false);

  //TODO combine leaves and leaves like: //just using lookupRange
  /*uint64_t leafId = 0;
  Leaf<KeyType, KeyComparator> *currentLeaf = nullptr; //beginning most left leaf
  while (currentLeaf->header.nextLeaf != nullptr) {
    stream << "leaf" << leafId++ << ":next -> leaf" << leafId << "count;";
    currentLeaf = currentLeaf->header.nextLeaf;
  }
  stream << "}";
  std::cout << stream.str() << std::endl;
  */
}

template<typename KeyType, typename KeyComparator>
void InnerNode<KeyType, KeyComparator>::visualize(uint64_t *leafId, uint64_t *nodeId,
                                                  uint64_t curDepth, uint64_t maxDepth) {
  std::stringstream stream;
  stream << "node" << *nodeId << " [shape=record, label=\n"
  << "\"<count> " << header.keyCount << " | ";
  for (uint64_t n = 0; n < header.keyCount; ++n) {
    Entry<KeyType, uint64_t> entry = entries[n];
    //TODO maybe casting of value
    stream << "<key" << n << ">" << entry.key << "<value" << n << "> " << entry.value << " | <next> *\"]; \n";
    if (curDepth == maxDepth) {
      //we have reached the bottom
      Leaf<KeyType, KeyComparator> *leaf = reinterpret_cast<Leaf<KeyType, KeyComparator> *>(entry.value);
      leaf->visualize(*leafId);
      (*leafId)++;
      //TODO combine nodes and leaves like:
      //stream << "node0:ptr0 -> leaf0:keyCount;";
    } else {
      InnerNode<KeyType, KeyComparator> *curNode = reinterpret_cast<InnerNode<KeyType, KeyComparator> * >(entry.value);
      curNode->visualize(leafId, nodeId, curDepth + 1, maxDepth);
      (*nodeId)++;
      //TODO combine nodes and nodes like:
      //stream << "node0:ptr0 -> node1:keyCount;";
    }
  }
  std::cout << stream.str() << std::endl;
}

template<class KeyType, class KeyComparator>
void Leaf<KeyType, KeyComparator>::visualize(uint64_t leafId) {
  std::stringstream stream;
  stream << "leaf" << leafId << " [shape=record, label=\n"
  << "\"<count> " << header.keyCount << " | ";
  for (uint64_t e = 0; e < header.keyCount; ++e) {
    Entry<KeyType, TID> entry = entries[e];
    //TODO maybe casting of value
//		stream << "<key" << e << ">" << entry.key << "<value" << e << "> " << entry.value << " | <next> *\"]; \n";
  }
  std::cout << stream.str() << std::endl;
}
