//
// Created by Martin on 2015-05-27
//

#include <sstream>
#include <iostream>
#include "BTree.hpp"

template<class KeyType, class KeyComparator>
void BTree<KeyType, KeyComparator>::visualize() {
	std::stringstream stream;
	stream << "diagraph myBTree { \n";
	stream << "node [shape=record];";

	void * rootNode = nullptr; // TODO: retrieve from rootPageId
	if (height == 0) {
		//no elements
	} else if (height == 1) {
		Leaf<KeyType> *leaf = reinterpret_cast<Leaf<KeyType> *>(rootNode);
		leaf->visualize(0);
	} else {
		InnerNode<KeyType> *node = reinterpret_cast<InnerNode<KeyType> *> (rootNode);
		node->visualize(0, 0, 1, height);
	}

	//TODO combine leaves and leaves like: //just using lookupRange
	//
	uint64_t leafId = 0;
	Leaf<KeyType> *currentLeaf = nullptr; //beginning most left leaf
	while (currentLeaf->header.nextLeaf != nullptr) {
		stream << "leaf" << leafId++ << ":next -> leaf" << leafId << "count;";
		currentLeaf = currentLeaf->header.nextLeaf;
	}
	stream << "}";
	std::cout << stream.str() << std::endl;
}

template<class KeyType>
void InnerNode<KeyType>::visualize(uint64_t *leafId, uint64_t *nodeId, uint64_t curDepth, uint64_t maxDepth) {
	std::stringstream stream;
	stream << "node" << *nodeId << " [shape=record, label=\n"
	<< "\"<count> " << header.count << " | ";
	for (uint64_t n = 0; n < header.count; ++n) {
		Entry<KeyType, void *> entry = entries[n];
		//TODO maybe casting of value
		stream << "<key" << n << ">" << entry.key << "<value" << n << "> " << entry.value << " | <next> *\"]; \n";
		if (curDepth + 1 == maxDepth) {
			//we have reached the bottom
			Leaf<KeyType> *leaf = reinterpret_cast<Leaf<KeyType> *>(entry.value);
			leaf->visualize(leafId);
			(*leafId)++;
			//TODO combine nodes and leaves like:
			//stream << "node0:ptr0 -> leaf0:count;";
		} else {
			InnerNode<KeyType> *curNode = reinterpret_cast<InnerNode<KeyType> * >(entry.value);
			curNode->visualize(leafId, nodeId, curDepth + 1, maxDepth);
			(*nodeId)++;
			//TODO combine nodes and nodes like:
			//stream << "node0:ptr0 -> node1:count;";
		}
	}
	std::cout << stream.str() << std::endl;
}

template<class KeyType>
void Leaf<KeyType>::visualize(uint64_t *leafId) {
	std::stringstream stream;
	stream << "leaf" << *leafId << " [shape=record, label=\n"
	<< "\"<count> " << header.count << " | ";
	for (uint64_t e = 0; e < header.count; ++e) {
		Entry<KeyType, TID> entry = entries[e];
		//TODO maybe casting of value
//		stream << "<key" << e << ">" << entry.key << "<value" << e << "> " << entry.value << " | <next> *\"]; \n";
	}
	std::cout << stream.str() << std::endl;
}
