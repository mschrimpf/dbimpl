//
// Created by Martin on 31.05.2015.
//

#include "InnerNode.hpp"

template<class KeyType>
inline uint64_t InnerNode<KeyType>::getNextNode(KeyType key) {
  return 0;
}

template<class KeyType>
bool InnerNode<KeyType>::hasSpaceForOneMoreEntry() {
  return false;
}
