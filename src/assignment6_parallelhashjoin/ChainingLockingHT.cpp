#ifndef PROJECT_CHAININGLOCKINGHT_H
#define PROJECT_CHAININGLOCKINGHT_H

#include "hash.hpp"
#include "stdint.h"
#include "inttypes_wrapper.hpp"
#include <mutex>
#include <stdio.h>
#include <string.h>

class ChainingLockingHT {
public:
  // Chained tuple entry
  struct Entry {
    uint64_t key;
    Entry *next;

    Entry(uint64_t key) : key(key), next(nullptr) { }

    Entry() { }
  };

private:
  Entry **entries;
  uint64_t size;
  std::mutex *locks; //TODO andere mutexe nehmen

public:
  // Constructor
  ChainingLockingHT(uint64_t size) : size(size) {
    entries = new Entry *[size];
    memset(entries, 0, (size_t) size * sizeof(Entry *));
    locks = new std::mutex[size];
  }

  // Destructor
  ~ChainingLockingHT() {
    delete[] entries;
    delete[] locks;
  }

  // Returns the number of hits
  inline uint64_t lookup(uint64_t key) const {
    uint64_t hash = hashKey(key) % size;
    Entry *entry = entries[hash];
    uint64_t count = 0;
    while (entry != nullptr) {
      // branch-free. equivalent to if(entry->key == key) count++
      bool inc = entry->key == key;
      count += inc;
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
        printf(" -> (%" PRIu64 ")", entry->key);
        entry = entry->next;
      }
      printf("\n");
    }
  }
};

#endif //PROJECT_CHAININGLOCKINGHT_H
