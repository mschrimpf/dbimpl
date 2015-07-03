#ifndef PROJECT_CHAININGHT_H
#define PROJECT_CHAININGHT_H

#include "hash.hpp"
#include <atomic>
#include <stdio.h>

class ChainingHT {
public:
  // Chained tuple entry
  struct Entry {
    uint64_t key;
    Entry *next;

    Entry(uint64_t key) : key(key), next(nullptr) { }
  };

private:
  std::atomic<Entry *> *atomicEntries;
  uint64_t size;

public:
  // Constructor
  ChainingHT(uint64_t size) : size(size) {
    atomicEntries = new std::atomic<Entry *>[size];
    for (unsigned i = 0; i < size; i++) {
      atomicEntries[i] = nullptr;
    }
  }

  // Destructor
  ~ChainingHT() {
    delete[] atomicEntries;
  }

  // Returns the number of hits
  inline uint64_t lookup(uint64_t key) {
    uint64_t hash = hashKey(key) % size;
    Entry *entry = atomicEntries[hash];
    uint64_t count = 0;
    while (entry != nullptr) {
      if (entry->key == key) {
        count++;
      }
      entry = entry->next;
    }
    return count;
  }

  inline void insert(Entry *newEntry) {
    uint64_t hash = hashKey(newEntry->key) % size;

    Entry *oldEntry;
    do {
      oldEntry = atomicEntries[hash];
      newEntry->next = oldEntry;
    } while (!atomicEntries[hash]
        .compare_exchange_strong(oldEntry, newEntry,
                                 std::memory_order_seq_cst, std::memory_order_seq_cst));
  }

  inline void print() {
    for (unsigned i = 0; i < size; i++) {
      Entry *entry = atomicEntries[i];
      if (entry == nullptr) {
        continue;
      }
      printf("[%u]", i);
      while (entry != nullptr) {
        printf(" -> (%llu)", entry->key);
        entry = entry->next;
      }
      printf("\n");
    }
  }
};

#endif //PROJECT_CHAININGHT_H
