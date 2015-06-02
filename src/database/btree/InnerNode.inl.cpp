//
// Created by Martin on 31.05.2015.
//

#include "InnerNode.hpp"
#include "EntriesHelper.inl.cpp"

template<typename KeyType, typename KeyComparator>
inline uint64_t InnerNode<KeyType, KeyComparator>::getNextNode(KeyType key, KeyComparator &smaller) {
  int min = getMinForSearch();
  int max = getMaxForSearch();
  return EntriesHelper::searchDirectionValue(entries, key, min, max, smaller);
}

template<typename KeyType, typename KeyComparator>
inline bool InnerNode<KeyType, KeyComparator>::hasSpaceForOneMoreEntry() {
  return this->header.keyCount < BTreeConstants<KeyType, KeyComparator>::maxNodeCapacity;
}

template<typename KeyType, typename KeyComparator>
void InnerNode<KeyType, KeyComparator>::insertDefiniteFit(KeyType key, uint64_t leftValue, uint64_t rightValue,
                                                          KeyComparator &smaller) {
  int min = getMinForSearch();
  int max = getMaxForSearch();
  int insertPosition = EntriesHelper::searchInsertPosition(entries, key, min, max, smaller);
  printf("Insert key %lu at position %d\n", key, insertPosition);
  EntriesHelper::moveEntriesToRight(entries, insertPosition, header.keyCount + 1);
  entries[insertPosition].key = key;
  entries[insertPosition].value = rightValue;
  entries[insertPosition - 1].value = leftValue;
  header.keyCount++;
}

template<typename KeyType, typename KeyComparator>
int InnerNode<KeyType, KeyComparator>::getMaxForSearch() {
  size_t keyCount = header.keyCount;
  return keyCount + 1;
}

template<typename KeyType, typename KeyComparator>
int InnerNode<KeyType, KeyComparator>::getMinForSearch() {
  return 1;
}
