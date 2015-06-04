//
// Created by Martin on 31.05.2015.
//

#include <sstream>
#include "Leaf.hpp"
#include "BTreeConstants.hpp"
#include "EntriesHelper.inl.cpp"

template<typename KeyType, typename KeyComparator>
inline bool Leaf<KeyType, KeyComparator>::hasSpaceForOneMoreEntry() {
  return header.keyCount < BTreeConstants<KeyType, KeyComparator>::maxLeafCapacity;
}

template<typename KeyType, typename KeyComparator>
inline void Leaf<KeyType, KeyComparator>::insertDefiniteFit(KeyType key, TID tid, KeyComparator &smaller) {
  int min = getEntriesLeftBound();
  int max = getEntriesRightBound();
  unsigned long insertPosition = EntriesHelper::findInsertPosition(entries, key, min, max, smaller);
  EntriesHelper::moveEntriesToRight(entries, insertPosition, header.keyCount);
  entries[insertPosition].key = key;
  entries[insertPosition].value = tid;
  header.keyCount++;
}

template<typename KeyType, typename KeyComparator>
void Leaf<KeyType, KeyComparator>::erase(KeyType key, KeyComparator &smaller) {
  int min = getEntriesLeftBound();
  int max = getEntriesRightBound();
  int keyPosition = EntriesHelper::safeFindKeyPosition(entries, key, min, max, smaller);
  EntriesHelper::moveEntriesToLeft(entries, keyPosition + 1, header.keyCount);
  header.keyCount--;
}

template<typename KeyType, typename KeyComparator>
bool Leaf<KeyType, KeyComparator>::lookup(KeyType key, KeyComparator &smaller, TID *resultTid) {
  int min = getEntriesLeftBound();
  int max = getEntriesRightBound();
  return EntriesHelper::searchValue(entries, key,
                                    min, max,
                                    smaller, resultTid);
}

template<typename KeyType, typename KeyComparator>
int Leaf<KeyType, KeyComparator>::getEntriesLeftBound() {
  return 0;
}

template<typename KeyType, typename KeyComparator>
int Leaf<KeyType, KeyComparator>::getEntriesRightBound() {
  return header.keyCount;
}
