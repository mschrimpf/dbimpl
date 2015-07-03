#ifndef PROJECT_CHAININGLOCKINGHT_H
#define PROJECT_CHAININGLOCKINGHT_H

#include "hash.hpp"
#include "stdint.h"
#include <mutex>
#include <stdio.h>

class ChainingLockingHT {
public:
  // Chained tuple entry
  struct Entry {
    uint64_t key;
    uint64_t value;
    Entry *next;

    Entry(uint64_t key, uint64_t value) : key(key), value(value), next(nullptr) { }
  };

private:
  Entry **entries;
  uint64_t size;
  std::mutex *locks; //TODO andere mutexe nehmen

public:
  // Constructor
  ChainingLockingHT(uint64_t size) : size(size) {
    entries = new Entry *[size];
    for (uint64_t i = 0; i < size; i++) {
      entries[i] = nullptr;
    }
    locks = new std::mutex[size];
  }

  // Destructor
  ~ChainingLockingHT() {
    delete[] entries;
    delete[] locks;
  }

  // Returns the number of hits
  inline uint64_t lookup(uint64_t key) {
    uint64_t hash = hashKey(key) % size;
    Entry *entry = entries[hash];
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
    std::lock_guard<std::mutex> lock(locks[hash]);
    Entry *currHead = entries[hash];
    entries[hash] = newEntry;
    newEntry->next = currHead;
  }

  inline void print() {
    for (unsigned i = 0; i < size; i++) {
      Entry *entry = entries[i];
      if (entry == nullptr) {
        continue;
      }
      printf("[%u]", i);
      while (entry != nullptr) {
        printf(" -> (%llu=%llu)", entry->key, entry->value);
        entry = entry->next;
      }
      printf("\n");
    }
  }
};

#endif //PROJECT_CHAININGLOCKINGHT_H
