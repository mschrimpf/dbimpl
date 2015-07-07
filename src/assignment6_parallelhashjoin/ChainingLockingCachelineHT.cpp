#ifndef PROJECT_CHAININGLOCKINGCACHELINEHT_H
#define PROJECT_CHAININGLOCKINGCACHELINEHT_H

#include "hash.hpp"
#include "stdint.h"
#include "inttypes_wrapper.hpp"
#include <mutex>
#include <stdio.h>
#include <string.h>

class ChainingLockingCachelineHT {
public:
  // Chained tuple entry
  struct Entry {
    uint64_t key;
    Entry *next;

    Entry(uint64_t key) : key(key), next(nullptr) { }

    Entry() { }
  };

private:
  struct Bucket {
    Entry *entry;
    std::mutex lock;

    Bucket() : entry(nullptr) { }
  };

  Bucket *buckets;
  uint64_t size;

public:
  // Constructor
  ChainingLockingCachelineHT(uint64_t size) : size(size) {
    buckets = new Bucket[size];
    memset(buckets, 0, (size_t) size * sizeof(Bucket));
  }

  // Destructor
  ~ChainingLockingCachelineHT() {
    delete[] buckets;
  }

  // Returns the number of hits
  inline uint64_t lookup(uint64_t key) const {
    uint64_t hash = hashKey(key) % size;
    Entry *entry = buckets[hash].entry;
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
    std::lock_guard<std::mutex> lock(buckets[hash].lock);
    Entry *currHead = buckets[hash].entry;
    buckets[hash].entry = newEntry;
    newEntry->next = currHead;
  }

  inline void print() {
    for (unsigned i = 0; i < size; i++) {
      Entry *entry = buckets[i].entry;
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

#endif //CACHELINE
