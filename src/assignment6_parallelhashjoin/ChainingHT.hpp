#ifndef PROJECT_CHAININGHT_H
#define PROJECT_CHAININGHT_H

#include "hashjoinskeleton.cpp"
#include <atomic>

class ChainingHT {
private:
  Entry *entries;

public:
  // Chained tuple entry
  struct Entry {
    uint64_t key;
    uint64_t value;
    Entry *next;
  };

  // Constructor
  ChainingHT(uint64_t size) {
    entries = new Entry[size];
  }

  // Destructor
  ~ChainingHT() {
    delete[] entries;
  }

  inline Entry *lookup(uint64_t key) {
    uint64_t hash = hashKey(key);
    return &entries[hash];
  }

  inline void insert(Entry *entry) {
    uint64_t key = entry->key;
    uint64_t val = entry->value;
    Entry newEntry;
    newEntry.key = key;
    newEntry.value = val;
    uint64_t hash = hashKey(key);

    do {
      newEntry.next = entries[hash];
    } while (compare_exchange_strong(entries[hash], newEntry, std::memory_order_relaxed));
  }
};

#endif //PROJECT_CHAININGHT_H
