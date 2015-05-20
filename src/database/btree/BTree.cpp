//
// Created by daniel on 5/19/15.
//

#include "BTree.h"


uint64_t BTree::size() {
    return treeSize;
}

template<class T, class CMP>
bool BTree::lookup(T param, TID& tid) {
    // while not found
    // while currentNode != leaf
        // check each value in node, if param < currentNode.param, look left, otherwise right
        // if currentNode = currentNode.nextNode;
    return true;
}

template<class T, class CMP>
bool BTree::erase(T param) {
    //Remove first
    //Check if Node Size < min-Capacity
        //no -> we're done
        //yes -> merge with left neighbour, if not exist, with right neighbour
                // check if we need to split afterwards.
    return true;
}

template<class T, class CMP>
bool BTree::insert(T param, TID tid) {
    // Check if key is already in tree
    if (lookup(param, tid)){
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

