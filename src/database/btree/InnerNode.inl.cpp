//
// Created by Martin on 31.05.2015.
//

#include "InnerNode.hpp"
#include "EntriesSearch.inl.cpp"

template<typename KeyType, typename KeyComparator>
inline uint64_t InnerNode<KeyType, KeyComparator>::getNextNode(KeyType key, KeyComparator &smaller) {
  int min = 1;
  int max = header.keyCount + 1;
  return searchValue<KeyType, KeyComparator, uint64_t>(entries, key, min, max, smaller);
}

template<typename KeyType, typename KeyComparator>
inline bool InnerNode<KeyType, KeyComparator>::hasSpaceForOneMoreEntry() {
  return this->header.keyCount < BTreeConstants<KeyType, KeyComparator>::maxNodeCapacity;
}

template<typename KeyType, typename KeyComparator>
void InnerNode<KeyType, KeyComparator>::insertDefiniteFit(KeyType key, uint64_t leftValue, uint64_t rightValue,
                                                          KeyComparator &smaller) {
  int insertPosition = searchInsertPosition(key, smaller);
  moveEntriesToRight(insertPosition);
  entries[insertPosition].key = key;
  entries[insertPosition].value = rightValue;
  entries[insertPosition - 1].value = leftValue;
}

// iterative binary search implementation
template<typename KeyType, typename KeyComparator>
int InnerNode<KeyType, KeyComparator>::searchInsertPosition(KeyType key, KeyComparator &smaller) {
  int min = 1;
  int max = header.keyCount + 1;

  while (max >= min) {
    int mid = (max + min) / 2;
    KeyType entriesKey = entries[mid].key;
    if (!smaller(entriesKey, key) && !smaller(key, entriesKey)) {
      throw std::invalid_argument("Key already exists");
    } else if (smaller(entriesKey, key)) {
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

template<typename KeyType, typename KeyComparator>
void InnerNode<KeyType, KeyComparator>::moveEntriesToRight(int fromIndex) {
  memcpy(entries + fromIndex + 1 /* move one to right */,
         entries,
         header.keyCount + 1 /* first KV pair */ - fromIndex);
}
