//
// Created by Martin on 31.05.2015.
//

#include "Leaf.hpp"

template<class KeyType>
inline bool Leaf<KeyType>::hasSpaceForOneMoreEntry() {
  return false;
}

template<class KeyType>
inline void Leaf<KeyType>::insert(KeyType key, TID tid) {

}
