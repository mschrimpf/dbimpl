//
// Created by Martin on 31.05.2015.
//

#ifndef PROJECT_ENTRIESSEARCH_HPP
#define PROJECT_ENTRIESSEARCH_HPP

#include "Entry.hpp"
#include <stdexcept>
#include <string.h>

class EntriesHelper {
public:
// iterative binary search implementation
  template<typename KeyType, typename KeyComparator, typename ValueType>
  static ValueType searchValue(Entry<KeyType, ValueType> entries[], KeyType key, int min, int max,
                               KeyComparator &smaller) {
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

// iterative binary search implementation to find the current position
  template<typename KeyType, typename KeyComparator, typename ValueType>
  static int findPosition(Entry<KeyType, ValueType> entries[], KeyType key, int min, int max, KeyComparator &smaller) {
    while (max != min) {
      int mid = (max + min) / 2;
      KeyType entryKey = entries[mid].key;
      if (smaller(entryKey, key)) {
        min = mid + 1;
      } else {
        max = mid - 1;
      }
    }
    return min;
  }

  // iterative binary search implementation
  template<typename KeyType, typename KeyComparator, typename ValueType>
  static int searchInsertPosition(Entry<KeyType, ValueType> entries[],
                                  KeyType key,
                                  int min, int max,
                                  KeyComparator &smaller) {
    if (max == min) {
      return min;
    }
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
      std::string minStr = std::to_string(min);
      std::string maxStr = std::to_string(max);
      std::string msg = "min (" + minStr + ") != max (" + maxStr + ")";
      throw std::invalid_argument(msg);
    }
    return min;
  }

  template<typename KeyType, typename ValueType>
  static void moveEntriesToRight(Entry<KeyType, ValueType> entries[], int fromIndex, int lastKeyIndex) {
    memcpy(entries + fromIndex + 1 /* move one to right */,
           entries,
           (size_t) (lastKeyIndex /* first KV pair */ - fromIndex));
  }
};

#endif //PROJECT_ENTRIESSEARCH_HPP
