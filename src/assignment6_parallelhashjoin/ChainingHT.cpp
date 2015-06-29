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
    uint64_t value;
    Entry *next;

    Entry(uint64_t key, uint64_t value) : key(key), value(value), next(nullptr) { }
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

  inline Entry *lookup(uint64_t key) {
    uint64_t hash = hashKey(key) % size;
    Entry * entry = atomicEntries[hash];
    while(entry != nullptr) {
      if(entry->key == key) {
        return entry;
      }
      entry = entry->next;
    }
    return nullptr;
  }

  inline void insert(uint64_t key, uint64_t value) {
    Entry *newEntry = new Entry(key, value);
    uint64_t hash = hashKey(key) % size;

    Entry *oldEntry = atomicEntries[hash];
    do {
      newEntry->next = atomicEntries[hash];
    } while (!atomicEntries[hash].compare_exchange_strong(oldEntry, newEntry,
                                                          std::memory_order_relaxed, std::memory_order_relaxed));
  }

  inline void print() {
    for (unsigned i = 0; i < size; i++) {
      Entry *entry = atomicEntries[i];
      if (entry == nullptr) {
        continue;
      }
      printf("[%u]", i);
      while (entry != nullptr) {
        printf(" -> (%llu, %llu)", entry->key, entry->value);
        entry = entry->next;
      }
      printf("\n");
    }
  }
};

#endif //PROJECT_CHAININGHT_H
