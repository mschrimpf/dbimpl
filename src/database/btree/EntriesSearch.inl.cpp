//
// Created by Martin on 31.05.2015.
//

#ifndef PROJECT_ENTRIESSEARCH_HPP
#define PROJECT_ENTRIESSEARCH_HPP

#include "Entry.hpp"
#include <stdexcept>

// iterative binary search implementation
template<typename KeyType, typename KeyComparator, typename ValueType>
ValueType searchValue(Entry<KeyType, ValueType> entries[], KeyType key, int min, int max, KeyComparator &smaller) {
  while (max >= min) {
    int mid = (max + min) / 2;
    KeyType entryKey = entries[mid].key;
    if (!smaller(entryKey, key) && !smaller(key, entryKey)) {
      return entries[mid].value;
    } else if (smaller(entryKey, key)) {
      min = mid + 1;
    } else {
      max = mid - 1;
    }
  }

  throw std::invalid_argument("Key does not exist");
}

#endif //PROJECT_ENTRIESSEARCH_HPP
