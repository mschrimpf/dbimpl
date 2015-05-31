//
// Created by Martin on 31.05.2015.
//

#include "Leaf.hpp"
#include "BTreeConstants.hpp"

template<class KeyType, class KeyComparator>
inline bool Leaf<KeyType, KeyComparator>::hasSpaceForOneMoreEntry() {
  return header.keyCount < BTreeConstants<KeyType, KeyComparator>::maxLeafCapacity;
}

template<class KeyType, class KeyComparator>
inline void Leaf<KeyType, KeyComparator>::insertDefiniteFit(KeyType key, TID tid) {

}
