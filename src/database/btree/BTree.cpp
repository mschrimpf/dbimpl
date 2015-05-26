//
// Created by daniel on 5/19/15.
//

#include <sstream>
#include <iostream>
#include "BTree.hpp"

template<class KeyType, class KeyComparator>
bool BTree::erase(KeyType key) {
	//Remove first
	//Check if Node Size < min-Capacity
	//no -> we're done
	//yes -> merge with left neighbour, if not exist, with right neighbour
	// check if we need to split afterwards.
	return true;
}

template<class KeyType, class KeyComparator>
bool BTree::insert(KeyType key, TID tid) {
	// Check if key is already in tree
	if (lookup(key, tid)) {
		return false;
	}

	//Insert first, no matter if it fits or not
	//Check if capacity of node/leaf exceeds
	// no -> we're done here
	// yes ->
	// while exceeds currentNode {
	//      we need to split the node, add the "medium"-val into the parent node and check again for the "upper" node
	//      currentNode = upperNode;
	return true;
}

template<class KeyType, class KeyComparator>
std::vector<TID>::iterator BTree::lookupRange(KeyType key) {
	return NULL;
}


template<class KeyType, class KeyComparator>
bool BTree::lookup(KeyType key, TID &tid) {
	if (depth == 0) {
		// no element found
		return false;
	}
	return searchForKey(key, tid, rootNode, 1);
}

template<class KeyType, class KeyComparator>
bool BTree::searchForKey(KeyType key, TID &tid, void *node, uint64_t currentDepth) {
	if (currentDepth == depth) {
		//we reached maximum depth, so nodes are leaves
		Leaf *leaf = reinterpret_cast<Leaf *>(node);
		return searchLeafForKey(key, tid, leaf);
	} else {
		//we haven't reached the leaves yet
		Node *curNode = reinterpret_cast<Node *> (node);
		return searchNodeForKey(key, tid, curNode, currentDepth);
	}
}

template<class KeyType, class KeyComparator>
bool BTree::searchNodeForKey(KeyType key, TID &tid, BTree::Node *node, uint64_t depth) {
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
bool BTree::searchLeafForKey(KeyType key, TID &tid, Leaf *leaf) {
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
		Entry foundEntry = leaf->entries[middle];
		tid = foundEntry.value;
		return true;
	} else {
		//value not found
		return false;
	}
}

uint64_t BTree::size() {
	return 0;
}
