//
// Created by Martin on 31.05.2015.
//

#ifndef PROJECT_ENTRIESSEARCH_HPP
#define PROJECT_ENTRIESSEARCH_HPP

#include "Entry.hpp"
#include <stdexcept>

// iterative binary search implementation
template<typename KeyType, typename KeyComparator, typename ValueType>
ValueType searchValue(Entry<KeyType, ValueType> entries[], KeyType key, int min, int max) {
  while (max >= min) {
    int mid = (max + min) / 2;
    if (entries[mid].key == key) {
      return entries[mid].value;
    } else if (entries[mid].key < key) {
      min = mid + 1;
    } else {
      max = mid - 1;
    }
  }

  throw std::invalid_argument("Key does not exist");
}

// iterative binary search implementation to find the current position
template<typename KeyType, typename KeyComparator, typename ValueType>
int findPosition(Entry<KeyType, ValueType> entries[], KeyType key, int min, int max){
  while (max != min) {
    int mid = (max + min) / 2;
    if (entries[mid].key < key) {
      min = mid + 1;
    } else {
      max = mid - 1;
    }
  }
  return min;
}

#endif //PROJECT_ENTRIESSEARCH_HPP
