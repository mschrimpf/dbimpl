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
  std::cout << stream.str() << std::endl;
  stream.str("");
  stream.clear();
    BufferFrame frame = bufferManager.fixPage(segmentId, rootPageId, false);
    void *rootNode = frame.getData(); // TODO: retrieve from rootPageId
    if (height == 0) {
        Leaf<KeyType, KeyComparator> *leaf = reinterpret_cast<Leaf<KeyType, KeyComparator> *>(rootNode);
        visualizeLeaf(leaf, 0);
    } else {
        InnerNode<KeyType, KeyComparator> *node = reinterpret_cast<InnerNode<KeyType, KeyComparator> *> (rootNode);
        visualizeNode(node, 0, 0, 0, height);
    }
    bufferManager.unfixPage(frame, false);

    uint64_t leafId = 0;
    Leaf <KeyType, KeyComparator> currentLeaf = getMostLeftLeaf(); //beginning most left leaf
    while (currentLeaf.header.nextLeafPageId != LeafHeader::INVALID_PAGE_ID) {
        stream << "leaf" << leafId++ << ":next -> leaf" << leafId << "count;";
        currentLeaf = getLeaf(currentLeaf.header.nextLeafPageId);
    }
    stream << "}";
    std::cout << stream.str() << std::endl;
}


template<typename KeyType, typename KeyComparator>
void BTree<KeyType, KeyComparator>::visualizeNode(InnerNode<KeyType, KeyComparator> * node, uint64_t *leafId, uint64_t *nodeId,
                                                  uint64_t curDepth, uint64_t maxDepth) {
    std::stringstream stream;
    stream << "node" << *nodeId << " [shape=record, label=\n"
    << "\"<count> " << node->header.keyCount << " | ";
    for (uint64_t n = 1; n < node->header.keyCount + 1; ++n) {
        Entry<KeyType, uint64_t> entry = node->entries[n];
        BufferFrame frame = bufferManager.fixPage(segmentId, entry.value, false);
        //TODO maybe casting of value
        stream << "<key" << n << ">" << entry.key << "<value" << n << "> " << entry.value << " | <next> *\"]; \n";
        if (curDepth == maxDepth) {
            //we have reached the bottom
            Leaf<KeyType, KeyComparator> *leaf = reinterpret_cast<Leaf<KeyType, KeyComparator> *>(frame.getData());
            visualizeLeaf(leaf, *leafId);
            stream << "node" << nodeId << ":ptr0" << "-> leaf" << leafId << ":keyCount;";
            (*leafId)++;
        } else {
            InnerNode<KeyType, KeyComparator> *curNode = reinterpret_cast<InnerNode<KeyType, KeyComparator> * >(frame.getData());
            visualizeNode(curNode, leafId, nodeId, curDepth + 1, maxDepth);
            stream << "node" << nodeId << ":ptr0 -> node" << nodeId + 1 << ":keyCount;";
            (*nodeId)++;
        }
        bufferManager.unfixPage(frame, false);
    }
    std::cout << stream.str() << std::endl;
}

template<class KeyType, class KeyComparator>
void BTree<KeyType, KeyComparator>::visualizeLeaf(Leaf<KeyType, KeyComparator> * leaf, uint64_t leafId) {
  std::stringstream stream;
  stream << "leaf" << leafId << " [shape=record, label=\n"
  << "\"<count> " << leaf->header.keyCount << " | ";
  for (uint64_t e = 0; e < leaf->header.keyCount; ++e) {
    Entry<KeyType, TID> entry = leaf->entries[e];
    stream << "<key" << e << ">" << entry.key << "<value" << e << ">" << entry.value.pageId;
  }
  stream << " | <next> ";
  if (leaf->header.nextLeafPageId != LeafHeader::INVALID_PAGE_ID){
    stream << " *";
  }
  stream << "\"];";
  std::cout << stream.str() << std::endl;
}
