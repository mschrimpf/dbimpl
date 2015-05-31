//
// Created by Martin on 31.05.2015.
//

#include "InnerNode.hpp"
#include "BTreeConstants.hpp"
#include "EntriesSearch.inl.cpp"

template<class KeyType, class KeyComparator>
inline uint64_t InnerNode<KeyType, KeyComparator>::getNextNode(KeyType key) {
  int min = 1;
  int max = header.keyCount + 1;
  return search<KeyType, KeyComparator, uint64_t>(entries, key, min, max);
}

template<class KeyType, class KeyComparator>
inline bool InnerNode<KeyType, KeyComparator>::hasSpaceForOneMoreEntry() {
  return this->header.keyCount == BTreeConstants<KeyType, KeyComparator>::maxNodeCapacity;
}
