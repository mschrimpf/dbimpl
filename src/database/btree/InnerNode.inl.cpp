//
// Created by Martin on 31.05.2015.
//

#include "InnerNode.hpp"
#include "EntriesHelper.inl.cpp"

template<typename KeyType, typename KeyComparator>
inline uint64_t InnerNode<KeyType, KeyComparator>::getNextNode(KeyType key, KeyComparator &smaller) {
  int min = 1;
  int max = header.keyCount + 1;
  return EntriesHelper::safeSearchValue<KeyType, KeyComparator, uint64_t>(entries, key, min, max, smaller);
}

template<typename KeyType, typename KeyComparator>
inline bool InnerNode<KeyType, KeyComparator>::hasSpaceForOneMoreEntry() {
  return this->header.keyCount < BTreeConstants<KeyType, KeyComparator>::maxNodeCapacity;
}

template<typename KeyType, typename KeyComparator>
void InnerNode<KeyType, KeyComparator>::insertDefiniteFit(KeyType key, uint64_t leftValue, uint64_t rightValue,
                                                          KeyComparator &smaller) {
  int min = 1;
  int max = header.keyCount + 1;
  int insertPosition = EntriesHelper::searchInsertPosition(entries, key, min, max, smaller);
  EntriesHelper::moveEntriesToRight(entries, insertPosition, max);
  entries[insertPosition].key = key;
  entries[insertPosition].value = rightValue;
  entries[insertPosition - 1].value = leftValue;
}
