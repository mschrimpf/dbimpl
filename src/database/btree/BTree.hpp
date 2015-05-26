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
	static const uint64_t maxNodeCapacity = (BufferManager::DATA_SIZE_BYTE - sizeof(Node::Header))
											/ sizeof(Entry) - 1 /* one less for the additional K/V pair */;
	static const uint64_t minNodeCapacity = maxNodeCapacity / 2; // 50 percent is minimum
	static const uint64_t maxLeafCapacity = (BufferManager::DATA_SIZE_BYTE - sizeof(Node::Header))
											/ sizeof(Entry) - 1 /* one less for the additional K/V pair */;
	static const uint64_t minLeafCapacity = maxLeafCapacity / 2;

	BufferManager *bufferManager;
	KeyComparator comparator;

	uint64_t rootPageId : 48;
	size_t treeSize; // number of elements inside of the tree
	size_t depth;

	bool searchForKey(KeyType key, TID &tid, void *node, uint64_t depth);

	bool searchLeafForKey(KeyType key, TID &tid, Leaf *leaf);

	bool searchNodeForKey(KeyType key, TID &tid, Node *node, uint64_t depth);


public:
	BTree(BufferManager *bManager) : bufferManager(bManager) { }

	bool insert(KeyType key, TID tid);

	bool lookup(KeyType key, TID &tid);

	std::vector<TID>::iterator lookupRange(KeyType key);

	bool erase(KeyType key);

	uint64_t size();

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
};


#endif //PROJECT_BTREE_H
