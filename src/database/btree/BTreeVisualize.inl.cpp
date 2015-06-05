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
  void *rootNode = frame.getData();

  // check whether or not we only have leaves (if height == 0) and handle this case different
  if (height == 0) {
    Leaf<KeyType, KeyComparator> *leaf = reinterpret_cast<Leaf<KeyType, KeyComparator> *>(rootNode);
    stream << visualizeLeaf(leaf, 0);
  } else {
    InnerNode<KeyType, KeyComparator> *node = reinterpret_cast<InnerNode<KeyType, KeyComparator> *> (rootNode);
    stream << visualizeNode(node, &leaf_id, &node_id, 1);
  }
  bufferManager.unfixPage(frame, false);

  /* connect all leaves with each other (only in one direction) */
  leaf_id = 0;
  Leaf<KeyType, KeyComparator> currentLeaf = getMostLeftLeaf(); //beginning most left leaf
  while (currentLeaf.header.nextLeafPageId != LeafHeader::INVALID_PAGE_ID) {
    stream << "\nleaf" << leaf_id++ << ":next -> leaf" << leaf_id << "count;";
    currentLeaf = getLeaf(currentLeaf.header.nextLeafPageId);
  }
  stream << "\n}";
  return stream.str();
}


template<typename KeyType, typename KeyComparator>
std::string BTree<KeyType, KeyComparator>::visualizeNode(InnerNode<KeyType, KeyComparator> * node, uint64_t  * leafId, uint64_t * nodeId,
                                                  uint64_t curDepth) {
  std::stringstream stream;

  // visualize node
  stream << "node" << * nodeId << " [shape=record, label=\n"
  << "\"<count> " << node->header.keyCount << " | <isLeaf> false";
  for (uint64_t n = 0; n < BTreeConstants<KeyType>::maxNodeCapacity; ++n) {
    stream << " | <key" << n << "> ";
    if (n < node->header.keyCount){
      Entry<KeyType, uint64_t> entry = node->entries[n+1];
      stream << entry.key;
    }
  }
  for (uint64_t n = 0; n < BTreeConstants<KeyType>::maxNodeCapacity + 1; ++n) {
    stream << " | <ptr" << n << ">";
    if (n < node->header.keyCount + 1){
      stream << " *";
    }
  }
  stream << "\"];";

  int currentId = * nodeId;

  // visualize all subnodes/leaves
  for (uint64_t n = 0; n < node->header.keyCount + 1; ++n){
    Entry<KeyType, uint64_t> entry = node->entries[n];
    BufferFrame frame = bufferManager.fixPage(segmentId, entry.value, false);
    if (curDepth >= height) {
      //we've reached the bottom -> all subnodes are leaves
      Leaf<KeyType, KeyComparator> *leaf = reinterpret_cast<Leaf<KeyType, KeyComparator> *>(frame.getData());
      stream << "\n" << visualizeLeaf(leaf, * leafId);
      stream << "\nnode" << currentId << ":ptr" << n << "-> leaf" << * leafId << ":count;";
      (*leafId)++;
    } else {
      (* nodeId)++;
      InnerNode<KeyType, KeyComparator> *curNode = reinterpret_cast<InnerNode<KeyType, KeyComparator> * >(frame.getData());
      stream << "\n" << visualizeNode(curNode, leafId, nodeId, curDepth + 1);
      if (n < node->header.keyCount + 1) {
        stream << "\nnode" << currentId << ":ptr" << n << " -> node" << * nodeId<< ":count;";
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
  for (uint64_t e = 0; e < BTreeConstants<KeyType>::maxLeafCapacity; ++e) {
    stream << " <key" << e << "> ";
    if (e < leaf->header.keyCount){
      Entry<KeyType, TID> entry = leaf->entries[e];
      stream << entry.key;
    }
    stream << " | ";
  }
  for (uint64_t e = 0; e < BTreeConstants<KeyType>::maxLeafCapacity; ++e) {
    stream << " <tid" << e << "> ";
    if (e < leaf->header.keyCount) {
      Entry<KeyType, TID> entry = leaf->entries[e];
      stream << entry.value.pageId;
    }
    stream << " | ";
  }
  stream << " <next> ";
  if (leaf->header.nextLeafPageId != LeafHeader::INVALID_PAGE_ID){
    stream << " *";
  }else{
    stream << " /";
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
  int fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
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