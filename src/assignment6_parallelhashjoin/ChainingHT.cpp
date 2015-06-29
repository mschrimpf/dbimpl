#ifndef PROJECT_CHAININGHT_H
#define PROJECT_CHAININGHT_H

#include "hash.hpp"
#include <atomic>

class ChainingHT {
public:
  // Chained tuple entry
  struct Entry {
    uint64_t key;
    uint64_t value;
    Entry *next;
  };

private:
  std::atomic<Entry *> *atomicEntries;
  uint64_t size;

public:
  // Constructor
  ChainingHT(uint64_t size) : size(size) {
    atomicEntries = new std::atomic<Entry *>[size];
  }

  // Destructor
  ~ChainingHT() {
    delete[] atomicEntries;
  }

  inline Entry *lookup(uint64_t key) {
    uint64_t hash = hashKey(key) % size;
    return atomicEntries[hash];
  }

  inline void insert(uint64_t key, uint64_t value) {
    Entry *newEntry = new Entry;
    newEntry->key = key;
    newEntry->value = value;
    uint64_t hash = hashKey(key) % size;

    Entry *oldEntry = atomicEntries[hash];
    do {
      newEntry->next = atomicEntries[hash];
    } while (!atomicEntries[hash].compare_exchange_strong(oldEntry, newEntry,
                                                          std::memory_order_relaxed, std::memory_order_relaxed));
  }
};

#endif //PROJECT_CHAININGHT_H
