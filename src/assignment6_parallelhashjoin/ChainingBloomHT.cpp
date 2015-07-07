#ifndef PROJECT_CHAININGBLOOMHT_H
#define PROJECT_CHAININGBLOOMHT_H

#include "hash.hpp"
#include "inttypes_wrapper.hpp"
#include <atomic>
#include <stdio.h>
#include <string.h>

#define CHAININGBLOOMHT_DEBUG 0

/**
 * - store no values
 * - memset entries to zero instead of settings every single entry to null
 * - use first bits of pointer to store whether a specific entry is in the list (second hash)
 */
class ChainingBloomHT {
public:
  // Chained tuple entry
  struct Entry {
    uint64_t key;
    Entry *next;

    Entry(uint64_t key) : key(key), next(nullptr) { }
    Entry(){}
  };

private:
  const size_t POINTER_CONTROL_LENGTH = 16;
  const uint64_t POINTER_CONTROL_SHIFT_LENGTH = sizeof(Entry *) * 8 - POINTER_CONTROL_LENGTH;
  /** MSB: 16 1-bits */
  const uint64_t POINTER_CONTROL_MASK = ((uint64_t) 0b1111111111111111) << POINTER_CONTROL_SHIFT_LENGTH;
  /** LSB: 48 1-bits */
  const uint64_t POINTER_POINTER_MASK = POINTER_CONTROL_MASK ^((uint64_t) 18446744073709551615LLU /* 2^64 - 1 */);
  std::atomic<Entry *> *atomicEntries;
  uint64_t size;

  inline uint64_t controlHash(uint64_t key) const {
    return (key % POINTER_CONTROL_LENGTH) << POINTER_CONTROL_SHIFT_LENGTH;
  }

public:
  // Constructor
  ChainingBloomHT(uint64_t buildSize) : size(buildSize * 3 /* 48 max per entry / sizeof(Entry) = 48 / (8 + 8) */) {
    atomicEntries = new std::atomic<Entry *>[size];
    for (unsigned i = 0; i < size; i++) {
      atomicEntries[i] = nullptr;
    }
    // TODO
//    memset(atomicEntries, 0, (size_t) size);
  }

  // Destructor
  ~ChainingBloomHT() {
    delete[] atomicEntries;
  }

  // Returns the number of hits
  inline uint64_t lookup(uint64_t key) const {
    uint64_t hash = hashKey(key) % size;
    Entry *entry = atomicEntries[hash];
    if (entry == nullptr) {
      return 0;
    }
    uint64_t hash2 = controlHash(key);
    if ((((uint64_t) entry) & hash2) != hash2) {
      return 0;
    }
    entry = (Entry *) (((uint64_t) entry) & POINTER_POINTER_MASK);
    uint64_t count = 0;
    while (entry != nullptr) {
      // TODO: jump table instead of branching
      if (entry->key == key) {
        count++;
      }
      entry = entry->next;
    }
    return count;
  }

  inline void insert(Entry *newEntry) {
    uint64_t hash = hashKey(newEntry->key) % size;
    uint64_t hash2 = controlHash(newEntry->key);

    Entry *modifiedPtr;
    Entry *currHead;
    do {
      currHead = atomicEntries[hash];
      Entry *plainHead = (Entry *) (((uint64_t) currHead) & POINTER_POINTER_MASK);
      modifiedPtr = newEntry;
      modifiedPtr->next = plainHead;

      uint64_t currHash = ((uint64_t) currHead) & POINTER_CONTROL_MASK;
      modifiedPtr = (Entry *) (((uint64_t) modifiedPtr) | hash2 | currHash);

#if CHAININGBLOOMHT_DEBUG == 1
      printf("new entry: ");
      printBits(sizeof(newEntry), &newEntry);
      printf("curr head: ");
      printBits(sizeof(currHead), &currHead);
      printf("plainHead: ");
      printBits(sizeof(plainHead), &plainHead);
      printf("hash2:     ");
      printBits(sizeof(hash2), &hash2);
      printf("mod entry: ");
      printBits(sizeof(modifiedPtr), &modifiedPtr);
#endif

    } while (!atomicEntries[hash]
        .compare_exchange_strong(currHead, modifiedPtr,
                                 std::memory_order_seq_cst, std::memory_order_seq_cst));
  }

  inline void print() {
    for (unsigned i = 0; i < size; i++) {
      Entry *entry = atomicEntries[i];
      if (entry == nullptr) {
        continue;
      }
      entry = (Entry *) (((uint64_t) entry) & POINTER_POINTER_MASK);
      printf("[%u]", i);
      while (entry != nullptr) {
        printf(" -> (%" PRIu64 ")", entry->key);
        entry = entry->next;
      }
      printf("\n");
    }
  }

  //assumes little endian
  void printBits(size_t const size, void const *const ptr) {
    unsigned char *b = (unsigned char *) ptr;
    unsigned char byte;
    int i, j;

    for (i = size - 1; i >= 0; i--) {
      for (j = 7; j >= 0; j--) {
        byte = b[i] & (1 << j);
        byte >>= j;
        printf("%u", byte);
      }
    }
    puts("");
  }
};

#endif //PROJECT_CHAININGBLOOMHT_H
