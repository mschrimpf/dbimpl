//
// Created by Martin on 2015-05-27
//

#include <sstream>
#include <iostream>
#include "BTree.hpp"
#include <fcntl.h>

template<typename KeyType, typename KeyComparator>
std::string BTree<KeyType, KeyComparator>::visualize() {
    std::stringstream stream;
    stream << "digraph myBTree { \n";
    stream << "node [shape=record];\n";
  uint64_t node_id = 0;
  uint64_t leaf_id = 0;
    BufferFrame frame = bufferManager.fixPage(segmentId, rootPageId, false);
    void *rootNode = frame.getData(); // TODO: retrieve from rootPageId
    if (height == 0) {
        Leaf<KeyType, KeyComparator> *leaf = reinterpret_cast<Leaf<KeyType, KeyComparator> *>(rootNode);
        stream << visualizeLeaf(leaf, 0);
    } else {
        InnerNode<KeyType, KeyComparator> *node = reinterpret_cast<InnerNode<KeyType, KeyComparator> *> (rootNode);
        stream << visualizeNode(node, &leaf_id, &node_id, 0);
    }
    bufferManager.unfixPage(frame, false);

    uint64_t leafId = 0;
    Leaf <KeyType, KeyComparator> currentLeaf = getMostLeftLeaf(); //beginning most left leaf
    while (currentLeaf.header.nextLeafPageId != LeafHeader::INVALID_PAGE_ID) {
        stream << "leaf" << leafId++ << ":next -> leaf" << leafId << "count;";
        currentLeaf = getLeaf(currentLeaf.header.nextLeafPageId);
    }
    stream << "\n}";
  return stream.str();
    //std::cout << stream.str() << std::endl;
}


template<typename KeyType, typename KeyComparator>
std::string BTree<KeyType, KeyComparator>::visualizeNode(InnerNode<KeyType, KeyComparator> * node, uint64_t  * leafId, uint64_t * nodeId,
                                                  uint64_t curDepth) {
  std::stringstream stream;
  stream << "node" << * nodeId << " [shape=record, label=\n"
  << "\"<count> " << node->header.keyCount << " | <isLeaf> false";
  for (uint64_t n = 0; n < node->header.keyCount; ++n) {
    Entry<KeyType, uint64_t> entry = node->entries[n];
    //TODO maybe casting of value
    stream << " | <key" << n << ">" << entry.key;
  }
  for (uint64_t n = 0; n < node->header.keyCount; ++n) {
    Entry<KeyType, uint64_t> entry = node->entries[n];
    //TODO maybe casting of value
    stream << " | <ptr" << n << "> *";
  }
  stream << "\"]; \n";

  for (uint64_t n = 0; n < node->header.keyCount; ++n){
    Entry<KeyType, uint64_t> entry = node->entries[n];
    BufferFrame frame = bufferManager.fixPage(segmentId, entry.value, false);
    if (curDepth == height) {
      //we have reached the bottom
      Leaf<KeyType, KeyComparator> *leaf = reinterpret_cast<Leaf<KeyType, KeyComparator> *>(frame.getData());
      (*leafId)++;
      visualizeLeaf(leaf, * leafId);
      stream << "node" << * nodeId << ":ptr" << n << "-> leaf" << * leafId << ":count;";
    } else {
      InnerNode<KeyType, KeyComparator> *curNode = reinterpret_cast<InnerNode<KeyType, KeyComparator> * >(frame.getData());
      (*nodeId)++;
      visualizeNode(curNode, leafId, nodeId, curDepth + 1);
      if (n < node->header.keyCount) {
        stream << "node" << (* nodeId) - 1 << ":ptr" << n << " -> node" << * nodeId << ":count;";
      }
    }
    bufferManager.unfixPage(frame, false);
  }
  return stream.str();
  //std::cout << stream.str() << std::endl;
}

template<class KeyType, class KeyComparator>
std::string BTree<KeyType, KeyComparator>::visualizeLeaf(Leaf<KeyType, KeyComparator> * leaf, uint64_t leafId) {
  std::stringstream stream;
  stream << "leaf" << leafId << " [shape=record, label=\n"
  << "\"<count> " << leaf->header.keyCount << " | <isLeaf> true | ";
  for (uint64_t e = 0; e < leaf->header.keyCount; ++e) {
    Entry<KeyType, TID> entry = leaf->entries[e];
    stream << " <key" << e << "> " << entry.key << " | ";
  }
  stream << " | "; //makes image more clear
  for (uint64_t e = 0; e < leaf->header.keyCount; ++e){
    Entry<KeyType, TID> entry = leaf->entries[e];
    stream << " <tid" << e << "> " << entry.value.pageId << " | ";
  }
  stream << " <next> ";
  if (leaf->header.nextLeafPageId != LeafHeader::INVALID_PAGE_ID){
    stream << " *";
  }
  stream << "\"];";
  return stream.str();
  //std::cout << stream.str() << std::endl;
}

template<typename KeyType, typename KeyComparator>
void BTree<KeyType, KeyComparator>::outputVisualize() {
  std::string result = visualize();
  char filename[] = "tree.dot";
  char output[] = "tree.png";
  int fd = open(filename, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  if (fd == -1){
    //ERROR
    std::cerr << "Could not open file " << filename << std::endl;
  }
  write(fd, result.c_str(), strlen(result.c_str()));
  close(fd);
  std::stringstream cmd;
  cmd << "dot -Tpng " << filename << " -o " << output;
  system(cmd.str().c_str());

  //unlink(filename); //remove file
}