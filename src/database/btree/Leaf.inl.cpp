//
// Created by Martin on 31.05.2015.
//

#include "Leaf.hpp"
#include "BTreeConstants.hpp"
#include "EntriesHelper.inl.cpp"

template<typename KeyType, typename KeyComparator>
inline bool Leaf<KeyType, KeyComparator>::hasSpaceForOneMoreEntry() {
  return header.keyCount < BTreeConstants<KeyType, KeyComparator>::maxLeafCapacity;
}

template<typename KeyType, typename KeyComparator>
inline void Leaf<KeyType, KeyComparator>::insertDefiniteFit(KeyType key, TID tid, KeyComparator &smaller) {
  int min = 0;
  int max = getMaxForSearch();
  int insertPosition = EntriesHelper::searchInsertPosition(entries, key, min, max, smaller);
  EntriesHelper::moveEntriesToRight(entries, insertPosition, header.keyCount);
  entries[insertPosition].key = key;
  entries[insertPosition].value = tid;
  header.keyCount++;
}

template<typename KeyType, typename KeyComparator>
void Leaf<KeyType, KeyComparator>::erase(KeyType key, KeyComparator &smaller) {
  int min = 0;
  int max = getMaxForSearch();
  int keyPosition = EntriesHelper::findKeyPosition(entries, key, min, max, smaller);
  EntriesHelper::moveEntriesToLeft(entries, keyPosition + 1, header.keyCount);
  header.keyCount--;
}

template<typename KeyType, typename KeyComparator>
size_t Leaf<KeyType, KeyComparator>::getMaxForSearch() {
  return header.keyCount - 1;
}
