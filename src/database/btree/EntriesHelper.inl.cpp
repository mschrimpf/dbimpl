//
// Created by Martin on 31.05.2015.
//

#ifndef PROJECT_ENTRIESSEARCH_HPP
#define PROJECT_ENTRIESSEARCH_HPP

#include "Entry.hpp"
#include <stdexcept>
#include <string.h>
#include <stdio.h>
#include <algorithm>

class EntriesHelper {
public:
  /**
   * Find the value corresponding to the given key.
   * @return true if the key is found
   */
  template<typename KeyType, typename KeyComparator, typename ValueType>
  static bool searchValue(Entry<KeyType, ValueType> entries[], KeyType key,
                          int min, int max,
                          KeyComparator &smaller, ValueType *result) {
    int position = searchKeyPosition(entries, key, min, max, smaller);
    if (position < 0) {
      return false;
    }
    *result = entries[position].value;
    return true;
  }

  /**
   * Find the position of a key in an array of entries.
   * Throws if the key is not found.
   */
  template<typename KeyType, typename KeyComparator, typename ValueType>
  static int safeFindKeyPosition(Entry<KeyType, ValueType> entries[],
                                 KeyType key,
                                 int min, int max,
                                 KeyComparator &smaller) {
    int position = searchKeyPosition(entries, key, min, max, smaller);
    if (position < 0) {
      std::string keyString = std::to_string(key);
      throw std::invalid_argument("Key '" + keyString + "' not found");
    }

    return position;
  }

  /**
   * Find the position of a key in an array of entries.
   * @return the position of the key or -1 if the key is not found
   */
  template<typename KeyType, typename KeyComparator, typename ValueType>
  static int searchKeyPosition(Entry<KeyType, ValueType> entries[],
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

    return -1;
  }

  /**
    * Search not exactly the key but the value for its hypothetical position in the InnerNode.
    */
  template<typename KeyType, typename KeyComparator>
  static uint64_t findDirectionValue(Entry<KeyType, uint64_t> entries[], KeyType key,
                                     int min, int max,
                                     KeyComparator &smaller) {
    validateMinLessEqualMax(min, max);
    while (max > min) {
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

// iterative binary search implementation to find the current position
  template<typename KeyType, typename KeyComparator, typename ValueType>
  static int findPosition(Entry<KeyType, ValueType> entries[],
                          KeyType key,
                          int min, int max,
                          KeyComparator &smaller) {
    validateMinLessEqualMax(min, max);
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

  // lower bound implementation using iterative binary search
  template<typename KeyType, typename KeyComparator, typename ValueType>
  static unsigned long findInsertPosition(Entry<KeyType, ValueType> entries[],
                                KeyType key,
                                int min, int max,
                                KeyComparator &smaller) {
    struct EntryComparator {
      KeyComparator smaller;

      EntryComparator(KeyComparator comp) : smaller(comp) { }

      bool operator()(Entry<KeyType, ValueType> a,
                      Entry<KeyType, ValueType> b) const {
        return smaller(a.key, b.key);
      }
    } entryComparator(smaller);
    Entry<KeyType, ValueType> searchEntry;
    searchEntry.key = key;
    Entry<KeyType, ValueType> *insertEntry = std::lower_bound(
        entries + min,
        entries + max,
        searchEntry,
        entryComparator);
    unsigned long pos = insertEntry - entries;
    if(! smaller(insertEntry->key, key) && !smaller(key, insertEntry->key)) {
      std::string keyString = std::to_string(key);
      std::string posString = std::to_string(pos);
      throw std::invalid_argument("Key '" + keyString + "' already exists at position " + posString);
    }
    return pos;
  }

  template<typename KeyType, typename ValueType>
  static void moveEntriesToRight(Entry<KeyType, ValueType> *entries, unsigned long fromIndex, unsigned long keyCount) {
    auto source = &entries[fromIndex];
    size_t num = (keyCount - fromIndex) * sizeof(Entry<KeyType, ValueType>);
    unsigned long destIndex = fromIndex + 1 /* one to the right */;
    auto destination = &entries[destIndex];
    memmove(destination, source, num);
  }

  template<typename KeyType, typename ValueType>
  static void moveEntriesToLeft(Entry<KeyType, ValueType> entries[], int fromIndex, int keyCount) {
    auto source = &entries[fromIndex];
    size_t num = (keyCount - fromIndex) * sizeof(Entry<KeyType, ValueType>);
    int destIndex = fromIndex - 1 /* one to the left */;
    auto destination = &entries[destIndex];
    memmove(destination, source, num);
  }

  static void validateMinLessEqualMax(int min, int max) {
    if (min > max) {
      std::string minString = std::to_string(min);
      std::string maxString = std::to_string(max);
      throw std::invalid_argument("min " + minString + " is larger than max " + maxString);
    }
  }
};

#endif //PROJECT_ENTRIESSEARCH_HPP
