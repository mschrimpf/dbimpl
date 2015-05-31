//
// Created by Martin on 31.05.2015.
//

#include "InnerNode.hpp"
#include "EntriesSearch.inl.cpp"

template<class KeyType, class KeyComparator>
inline uint64_t InnerNode<KeyType, KeyComparator>::getNextNode(KeyType key) {
  int min = 1;
  int max = header.keyCount + 1;
  return searchValue<KeyType, KeyComparator, uint64_t>(entries, key, min, max);
}

template<class KeyType, class KeyComparator>
inline bool InnerNode<KeyType, KeyComparator>::hasSpaceForOneMoreEntry() {
  return this->header.keyCount < BTreeConstants<KeyType, KeyComparator>::maxNodeCapacity;
}

template<class KeyType, class KeyComparator>
void InnerNode<KeyType, KeyComparator>::insertDefiniteFit(KeyType key, uint64_t leftValue, uint64_t rightValue) {
  int insertPosition = searchInsertPosition(key);
  moveEntriesToRight(insertPosition);
  entries[insertPosition].key = key;
  entries[insertPosition].value = rightValue;
  entries[insertPosition - 1].value = leftValue;
}

// iterative binary search implementation
template<class KeyType, class KeyComparator>
int InnerNode<KeyType, KeyComparator>::searchInsertPosition(KeyType key) {
  int min = 1;
  int max = header.keyCount + 1;

  while (max >= min) {
    int mid = (max + min) / 2;
    if (entries[mid].key == key) {
      throw std::invalid_argument("Key already exists");
    } else if (entries[mid].key < key) {
      min = mid + 1;
    } else {
      max = mid - 1;
    }
  }

  if (min != max) {
    throw std::invalid_argument("min != max");
  }
  return min;
}

template<class KeyType, class KeyComparator>
void InnerNode<KeyType, KeyComparator>::moveEntriesToRight(int fromIndex) {
  memcpy(entries + fromIndex + 1 /* move one to right */,
         entries,
         header.keyCount + 1 /* first KV pair */ - fromIndex);
}
