#ifndef PROJECT_CHAININGLOCKINGHT_H
#define PROJECT_CHAININGLOCKINGHT_H

#include "hash.hpp"
#include "stdint.h"
#include <mutex>

class ChainingLockingHT {

public:

  // Chained tuple entry
  struct Entry {
    uint64_t key;
    uint64_t value;
    Entry *next;
  };

  // Constructor
  ChainingLockingHT(uint64_t size) : size(size) {
    entries = new Entry[size];
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
    Entry *entry = &entries[hash];
    while (entry->next != entry && entry->key != key) {
      entry = entry->next;
    }
    if (entry->key == key) {
      //found
      return entry;
    }
    //not found
    return nullptr;
  }

  inline void insert(Entry* entry) {
    uint64_t hash = hashKey(entry->key) % size;
    locks[hash].lock();
    Entry *entry = &entries[hash];
    while (entry->next != nullptr && entry->next != entry) {
      entry = entry->next;
    }
    if (entry->next == entry) {
      //entry is set
      Entry *newEntry = new Entry();
      newEntry->key = key;
      newEntry->value = value;
      newEntry->next = newEntry;
      entry->next = newEntry;
    } else {
      entry->key = key;
      entry->value = value;
    }
    locks[hash].unlock();
  }

private:
  Entry *entries;
  uint64_t size;
  std::mutex *locks; //TODO andere mutexe nehmen
};

#endif //PROJECT_CHAININGLOCKINGHT_H
