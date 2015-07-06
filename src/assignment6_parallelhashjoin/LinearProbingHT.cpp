#ifndef PROJECT_LINEARPROBING_H
#define PROJECT_LINEARPROBING_H

#include "hash.hpp"
#include <atomic>
#include <stdio.h>
#include "inttypes_wrapper.hpp"
#include "stdint.h"
#include "hash.hpp"

class LinearProbingHT {
public:
  struct Entry {
    uint64_t key;
    std::atomic<bool> marker;

    Entry() { }

    Entry(uint64_t key) : key(key), marker(false) { }
  };

private:
  Entry *entries;
  uint64_t size;

public:
  // Constructor
  LinearProbingHT(uint64_t size) : size(size) {
    entries = new Entry[size];
  }

  // Destructor
  ~LinearProbingHT() {
    delete[] entries;
  }

  // Returns the number of hits
  inline uint64_t lookup(uint64_t key) {
    uint64_t hash = hashKey(key);
    uint64_t pos = hash % size;

    uint64_t count = 0;
    while (entries[pos].marker) {
      if (entries[pos].key == key) {
        count++;
      }
      pos++;
    }
    return count;
  }

  // this method exists for testing purposes
  // as it allows a general interface for all parallel hash join implementations
  // accepting an entry pointer
  inline void insert(Entry *entry) {
    insert(entry->key);
  }

  inline void insert(uint64_t key) {
    uint64_t pos = hashKey(key) % size;
    // if the table is full, this loops forever.
    // derived from practical experience however,
    // we assume that the table will never be full.
    for (uint64_t i = pos; ; i = (i + 1) % size) {
      Entry &entry = entries[i];
      bool currMarker = entry.marker;
      // check entry
      if (!currMarker && entry.marker
          .compare_exchange_strong(currMarker, true,
                                   std::memory_order_seq_cst, std::memory_order_seq_cst)) {
        entry.key = key;
        entry.marker = true;
        break;
      }
    }
  }

  inline void print() {
    for (unsigned i = 0; i < size; i++) {
      Entry *entry = &entries[i];
      if (!entry->marker) {
        printf("x");
      } else {
        printf("%" PRIu64, entry->key);
      }
      if (i + 1 < size) {
        printf("|");
      }
    }
    printf("\n");
  }
};

#endif //PROJECT_LINEARPROBING_H
