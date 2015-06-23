#ifndef PROJECT_CHAININGHT_H
#define PROJECT_CHAININGHT_H

#include "hashjoinskeleton.cpp"
#include <atomic>

class ChainingHT {
private:
  Entry *entries;
  uint64_t size;

public:
  // Chained tuple entry
  struct Entry {
    uint64_t key;
    uint64_t value;
    Entry *next;
  };

  // Constructor
  ChainingHT(uint64_t size) : size(size) {
    entries = new Entry[size];
  }

  // Destructor
  ~ChainingHT() {
    delete[] entries;
  }

  inline Entry *lookup(uint64_t key) {
    uint64_t hash = hashKey(key) % size;
    return &entries[hash];
  }

  inline void insert(uint64_t key, uint64_t value) {
    Entry newEntry;
    newEntry.key = key;
    newEntry.value = value;
    uint64_t hash = hashKey(key) % size;

    do {
      newEntry.next = &entries[hash];
    } while (compare_exchange_strong(entries[hash], newEntry, std::memory_order_relaxed));
  }
};

#endif //PROJECT_CHAININGHT_H
