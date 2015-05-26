//
// Created by daniel on 5/19/15.
//

#include <sstream>
#include <inttypes.h>
#include <iostream>
#include "BTree.h"

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
    if (lookup(key, tid)){
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

void BTree::visualize() {
    std::stringstream stream;
    stream << "diagraph myBTree { \n";
    stream << "node [shape=record];";

    if (depth == 0) {
        //no elements
    } else if (depth == 1) {
        Leaf *leaf = reinterpret_cast<Leaf *>(rootNode);
        visualizeLeaf(leaf, 0);
    } else {
        Node *node = reinterpret_cast<Node *> (rootNode);
        visualizeNode(node, 0, 0, 1);
    }

    //TODO combine leaves and leaves like: //just using lookupRange
    //
    uint64_t leafId = 0;
    Leaf * currentLeaf = NULL; //beginning most left leaf
    while (currentLeaf->nextLeaf != nullptr){
        stream << "leaf"<<leafId++ << ":next -> leaf"<< leafId << "count;";
        currentLeaf = currentLeaf->nextLeaf;
    }
    stream << "}";
    std::cout << stream.str() << std::endl;
}

void BTree::visualizeNode(BTree::Node * node, uint64_t * leafId, uint64_t * nodeId, uint64_t curDepth) {
    std::stringstream stream;
    stream << "node" << *nodeId << " [shape=record, label=\n"
    << "\"<count> " << node->count << " | ";
    for (uint64_t n = 0; n < node->count; ++n){
        Entry entry = node -> entries[n];
        //TODO maybe casting of value
        stream << "<key" << n << ">" << entry.key << "<value" << n << "> " << entry.value << " | <next> *\"]; \n";
        if (curDepth + 1== depth){
            //we have reached the bottom
            Leaf * leaf = reinterpret_cast<Leaf *>(entry.value);
            visualizeLeaf(leaf, leafId);
            (*leafId)++;
            //TODO combine nodes and leaves like:
            //stream << "node0:ptr0 -> leaf0:count;";
        }else{
            Node * curNode = reinterpret_cast<Node * >(entry.value);
            visualizeNode(curNode, leafId, nodeId, curDepth + 1);
            (*nodeId)++;
            //TODO combine nodes and nodes like:
            //stream << "node0:ptr0 -> node1:count;";
        }
    }
    std::cout << stream.str() << std::endl;
}

void BTree::visualizeLeaf(BTree::Leaf * leaf, uint64_t * leafId) {
    std::stringstream stream;
    stream << "leaf" << *leafId << " [shape=record, label=\n"
    << "\"<count> " << leaf->count << " | ";
    for (uint64_t e = 0; e < leaf->count; ++e) {
        Entry entry = leaf->entries[e];
        //TODO maybe casting of value
        stream << "<key" << e << ">" << entry.key << "<value" << e << "> " << entry.value << " | <next> *\"]; \n";
    }
    std::cout << stream.str() << std::endl;
}


template<class KeyType, class KeyComparator>
bool BTree::lookup(KeyType key, TID& tid) {
    if (depth == 0){
        // no element found
        return false;
    }
    return searchForKey(key, tid, rootNode, 1);
}

template<class KeyType, class KeyComparator>
bool BTree::searchLeafForKey(KeyType key, TID &tid, Leaf * leaf) {
    uint64_t left = 0;
    uint64_t right = leaf->count;
    uint64_t middle = (left + right) / 2;

    while (left != right){
        if (comparator(leaf->entries[middle].key, key)){
            left = middle + 1;
        }else{
            right = middle;
        }
        middle = (left + right) / 2;
    }
    if (middle < leaf->count){
        //value found
        Entry foundEntry = leaf->entries[middle];
        tid = foundEntry.value;
        return true;
    }else{
        //value not found
        return false;
    }
}

template<class KeyType, class KeyComparator>
bool BTree::searchNodeForKey(KeyType key, TID &tid, BTree::Node * node, uint64_t depth) {
    uint64_t left = 0;
    uint64_t right = node->count;
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
    if (middle < node->count) {
        //value found
        return searchForKey(key, tid, node->entries[middle].value, depth + 1);
    }
    return false;
}

template<class KeyType, class KeyComparator>
bool BTree::searchForKey(KeyType key, TID &tid, void * node, uint64_t currentDepth) {
    if (currentDepth == depth){
        //we reached maximum depth, so nodes are leaves
        Leaf *leaf = reinterpret_cast<Leaf *>(node);
        return searchLeafForKey(key, tid, leaf);
    }else{
        //we haven't reached the leaves yet
        Node *curNode = reinterpret_cast<Node *> (node);
        return searchNodeForKey(key, tid, curNode, currentDepth);
    }
}
