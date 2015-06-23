#ifndef PROJECT_LINEARPROBING_H
#define PROJECT_LINEARPROBING_H

#include <atomic>
#include "stdint.h"
#include "hashjoinskeleton.cpp"

class LinearProbingHT {
private:
  Entry *entries;
  uint64_t size;
public:

  // Entry
  struct Entry {
    uint64_t key;
    uint64_t value;
    std::atomic<bool> marker;
  };

  // Constructor
  LinearProbingHT(uint64_t size) : size(size) {
    entries = new Entry[size];
  }

  // Destructor
  ~LinearProbingHT() {
    delete[] entries;
  }

  inline Entry *lookup(uint64_t key) {
    uint64_t hash = hashKey(key);
    for (uint64_t i = 0; i < size; i++) {
      Entry *entry = &entries[hash % size];
      if (entry->key == key) {
        //correct position found
        if (entry->marker) {
          // value set-> return entry
          return entry;
        } else {
          //value not set -> return nothing
          return nullptr;
        }
      } else {
        if (!entry->marker) {
          //empty spot available -> our entry is not inside of the map
          return nullptr;
        }
      }
    }
    // no key matched -> no value found
    return nullptr;
  }

  inline void insert(uint64_t key, uint64_t value) {
    uint64_t hash = hashKey(key);
    for (uint64_t i = hash; i < size; i++) {
      Entry *entry = &entries[i % size];
      if (!entry->marker) {
        //value added
        entry->key = key;
        entry->value = value;
        entry->marker = true;
        break;
      }
    }
    //value not added
  }
};

#endif //PROJECT_LINEARPROBING_H
