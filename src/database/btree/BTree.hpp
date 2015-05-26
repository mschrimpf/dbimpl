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

	size_t treeSize; // number of elements inside of the tree
	static uint64_t maxNodeCapacity; // capacity of nodes
	static uint64_t maxLeafCapacity; // capacity of leaves
	static uint64_t minNodeCapacity; // 50 percent is minimum
	static uint64_t minLeafCapacity; // 50 percent is minimum

	void *rootNode; //either node or leaf
	size_t depth; //depth of the tree

	bool searchForKey(KeyType key, TID &tid, void *node, uint64_t depth);

	bool searchLeafForKey(KeyType key, TID &tid, Leaf *leaf);

	bool searchNodeForKey(KeyType key, TID &tid, Node *node, uint64_t depth);


public:
	BTree(BufferManager *bManager, SPSegment *segment, uint64_t maxNodeEntries, uint64_t maxLeafEntries) :
			bufferManager(bManager), spSegment(segment), maxNodeCapacity(maxNodeEntries),
			maxLeafCapacity(maxLeafEntries), minNodeCapacity(maxNodeEntries / 2),
			minLeafCapacity(minLeafCapacity / 2) { }

	bool insert(KeyType key, TID tid);

	bool lookup(KeyType key, TID &tid);

	std::vector<TID>::iterator lookupRange(KeyType key);

	bool erase(KeyType key);

	void visualize();

	template<class ValueType>
	struct Entry {
		KeyType key;
		ValueType value;
	};

	// Node and Leaf are not the same struct as the sizes can differ
	struct Node {
		struct Header {
			size_t count; // number of entries
			Header() : count(0) { }
		} header;

		template<void *>
		Entry entries[];

		Node() : header() { }

		void visualize(uint64_t *leafId, uint64_t *nodeId, uint64_t depth, uint64_t maxDepth);
	};

	struct Leaf {
		struct Header {
			size_t count; // number of entries
			Leaf *previousLeaf;
			Leaf *nextLeaf;

			Header(Leaf *previous, Leaf *next) : previousLeaf(previous), nextLeaf(next), count(0) { }
		} header;

		template<void *>
		Entry entries[];

		Leaf(Leaf *previous, Leaf *next) : header(previous, next) { }

		void visualize(uint64_t *leafId);

		bool shouldSplit();
	};

	uint64_t size();
};


#endif //PROJECT_BTREE_H
