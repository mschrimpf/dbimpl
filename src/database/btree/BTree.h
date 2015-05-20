//
// Created by daniel on 5/19/15.
//

#ifndef PROJECT_BTREE_H
#define PROJECT_BTREE_H


#include "../slotted_pages/SPSegment.hpp"

template<class T, class CMP>
class BTree {

private:
    BufferManager *bufferManager;
    SPSegment *spSegment;

    uint64_t treeSize; // number of elements inside of the tree
    uint64_t maxNodeCapacity; // capacity of nodes
    uint64_t maxLeafCapacity; // capacity of leaves
    uint64_t minNodeCapacity = maxNodeCapacity / 2; // 50 percent is minimum
    uint64_t minLeafCapacity = maxLeafCapacity / 2; // 50 percent is minimum

    struct Entry {
        Node * nodeLeft; //TODO Kann auch Leaf sein, Idee eventuell falsch
        T key;
        TID tid;
        Node * nodeRight;
    };

    // Node and Leaf are not the same struct as the sizes can differ
    struct Node {
        uint64_t count; // number of entries
        Entry * entries[];
        Node() : count(0){ }
    };

    struct Leaf {
        bool isRootNode;
        uint64_t count; // number of entries
        Entry * entries[];

        Leaf *previousLeaf;
        Leaf *nextLeaf;
        void *data;

        Leaf(Leaf *previous, Leaf *next, bool isRoot) : previousLeaf(previous), nextLeaf(next), count(0) , isRootNode(isRoot) { }
    };

    inline bool isFull(Leaf leaf) {
        return leaf.count > maxLeafCapacity;
    }

    inline bool shouldMerge(Leaf leaf) {
        return !leaf.isRootNode && leaf.count < minLeafCapacity; //Root node should not be splitted
    }

    inline bool isFull(Node node) {
        return node.count > maxNodeCapacity;
    }

    inline bool shouldMerge(Node node) {
        return node.count < minNodeCapacity;
    }


public:
    BTree(BufferManager *bManager, SPSegment *segment, uint64_t maxNodeEntries, uint64_t maxLeafEntries) :
            bufferManager(bManager), spSegment(segment), maxNodeCapacity(maxNodeEntries),
            maxLeafCapacity(maxLeafEntries) { }

    bool insert(T param, TID tid);

    uint64_t size();

    bool lookup(T param, TID &tid);

    bool erase(T param);

    //TODO split, merge

};


#endif //PROJECT_BTREE_H
