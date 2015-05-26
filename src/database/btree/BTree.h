//
// Created by daniel on 5/19/15.
//

#ifndef PROJECT_BTREE_H
#define PROJECT_BTREE_H

#include <stdint.h>
#include <bits/stl_vector.h>
#include "../slotted_pages/SPSegment.hpp"

template<class KeyType, class KeyComparator>
class BTree {

private:
    BufferManager *bufferManager;
    SPSegment *spSegment;
    KeyComparator comparator;

    uint64_t treeSize; // number of elements inside of the tree
    uint64_t maxNodeCapacity; // capacity of nodes
    uint64_t maxLeafCapacity; // capacity of leaves
    uint64_t minNodeCapacity; // 50 percent is minimum
    uint64_t minLeafCapacity; // 50 percent is minimum

    void * rootNode; //either node or leaf
    size_t depth; //depth of the tree

    template<class ValueType>
    struct Entry {
        KeyType key;
        ValueType value;
    };

    // Node and Leaf are not the same struct as the sizes can differ
    struct Node {
        size_t count; // number of entries

        template<void *>
        Entry entries[];

        Node() : count(0){ }
    };

    struct Leaf {
        bool isRootNode;
        size_t count; // number of entries
        Entry entries[];

        Leaf *previousLeaf;
        Leaf *nextLeaf;

        Leaf(Leaf *previous, Leaf *next, bool isRoot) : previousLeaf(previous), nextLeaf(next), count(0) , isRootNode(isRoot) { }
    };

    inline bool shouldSplit(Leaf leaf) {
        return leaf.count > maxLeafCapacity;
    }

    inline bool shouldMerge(Leaf leaf) {
        return !leaf.isRootNode && leaf.count < minLeafCapacity; //Root node should not be splitted
    }

    inline bool shouldSplit(Node node) {
        return node.count > maxNodeCapacity;
    }

    inline bool shouldMerge(Node node) {
        return node.count < minNodeCapacity;
    }


public:
    BTree(BufferManager *bManager, SPSegment *segment, uint64_t maxNodeEntries, uint64_t maxLeafEntries) :
            bufferManager(bManager), spSegment(segment), maxNodeCapacity(maxNodeEntries),
            maxLeafCapacity(maxLeafEntries), minNodeCapacity(maxNodeEntries / 2), minLeafCapacity(minLeafCapacity / 2) { }

    bool insert(KeyType key, TID tid);

    uint64_t size();

    bool lookup(KeyType key, TID &tid);

    std::vector<TID>::iterator lookupRange (KeyType key);

    bool erase(KeyType key);

    void visualize();

    void visualizeNode(Node * node);
    void visualizeLeaf(Leaf * leaf);
    //TODO split, merge

    bool searchForKey(KeyType key, TID &tid, void * node, uint64_t depth);
    bool searchLeafForKey(KeyType key, TID &tid, Leaf * leaf);
    bool searchNodeForKey(KeyType key, TID &tid, Node * node, uint64_t depth);
};


#endif //PROJECT_BTREE_H
