//
// Created by Martin on 31.05.2015.
//

#include "Leaf.hpp"
#include "BTreeConstants.hpp"

template<typename KeyType, typename KeyComparator>
inline bool Leaf<KeyType, KeyComparator>::hasSpaceForOneMoreEntry() {
  return header.keyCount < BTreeConstants<KeyType, KeyComparator>::maxLeafCapacity;
}

template<typename KeyType, typename KeyComparator>
inline void Leaf<KeyType, KeyComparator>::insertDefiniteFit(KeyType key, TID tid) {
  // TODO
}

template<typename KeyType, typename KeyComparator>
void Leaf<KeyType, KeyComparator>::erase(KeyType key) {
  // TODO
}
