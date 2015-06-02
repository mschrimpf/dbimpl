//
// Created by Martin on 31.05.2015.
//

#ifndef PROJECT_ENTRIESSEARCH_HPP
#define PROJECT_ENTRIESSEARCH_HPP

#include "Entry.hpp"
#include <stdexcept>
#include <string.h>
#include <stdio.h>

class EntriesHelper {
public:
  template<typename KeyType, typename KeyComparator, typename ValueType>
  static ValueType safeSearchValue(Entry<KeyType, ValueType> entries[], KeyType key,
                                   int min, int max,
                                   KeyComparator &smaller) {
    ValueType result;
    if (!searchValue(entries, key, min, max, smaller, result)) {
      throw std::invalid_argument("Key does not exist");
    }
    return result;
  }

// iterative binary search implementation
  template<typename KeyType, typename KeyComparator, typename ValueType>
  static bool searchValue(Entry<KeyType, ValueType> entries[], KeyType key,
                          int min, int max,
                          KeyComparator &smaller, ValueType &result) {
    while (max >= min) {
      int mid = (max + min) / 2;
      KeyType entryKey = entries[mid].key;
      printf("Compare entryKey[%d]=%lu with key=%lu\n", mid, entryKey, key);
      if (!smaller(entryKey, key) && !smaller(key, entryKey)) {
        printf("Found!\n");
        result = entries[mid].value;
        return true;
      } else if (smaller(entryKey, key)) {
        min = mid + 1;
      } else {
        max = mid - 1;
      }
    }

    return false;
  }

  /**
   * Search not exactly the key but the value for its hypothetical position in the InnerNode.
   */
  template<typename KeyType, typename KeyComparator>
  static uint64_t searchDirectionValue(Entry<KeyType, uint64_t> entries[], KeyType key,
                                       int min, int max,
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

    KeyType entryKey = entries[min].key;
    if (smaller(key, entryKey)) {
      return entries[min - 1].value;
    } else {
      return entries[min].value;
    }
  }

  /**
   * Find the position of a key in an array of entries.
   * Iterative binary search implementation.
   */
  template<typename KeyType, typename KeyComparator, typename ValueType>
  static int findKeyPosition(Entry<KeyType, ValueType> entries[],
                             KeyType key,
                             int min, int max,
                             KeyComparator &smaller) {
    while (max >= min) {
      int mid = (max + min) / 2;
      KeyType entryKey = entries[mid].key;
      if (!smaller(entryKey, key) && !smaller(key, entryKey)) {
        return mid;
      } else if (smaller(entryKey, key)) {
        min = mid + 1;
      } else {
        max = mid - 1;
      }
    }

    throw std::invalid_argument("Key not found");
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
    while (max >= min) {
      int mid = (max + min) / 2;
      KeyType entryKey = entries[mid].key;
      if (!smaller(entryKey, key) && !smaller(key, entryKey)) {
        throw std::invalid_argument("Key already exists");
      } else if (smaller(entryKey, key)) {
        min = mid + 1;
      } else {
        max = mid - 1;
      }
    }

    return min;
  }

  template<typename KeyType, typename ValueType>
  static void moveEntriesToRight(Entry<KeyType, ValueType> entries[], int fromIndex, int lastKeyIndex) {
    memcpy(entries + fromIndex + 1 /* move one to right */,
           entries,
           (size_t) (lastKeyIndex /* first KV pair */ - fromIndex));
  }

  template<typename KeyType, typename ValueType>
  static void moveEntriesToLeft(Entry<KeyType, ValueType> entries[], int fromIndex, int size) {
    memcpy(entries + fromIndex - 1, entries + fromIndex, (size_t) size - fromIndex);
  }
};

#endif //PROJECT_ENTRIESSEARCH_HPP
