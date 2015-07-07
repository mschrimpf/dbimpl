#ifndef PROJECT_MATRIXHT_H
#define PROJECT_MATRIXHT_H

#include "hash.hpp"
#include "inttypes_wrapper.hpp"
#include <atomic>
#include <stdio.h>
#include <limits>
#include <stdexcept>

#define MATRIX_DEBUG 1

/**
 * Matrix of keys that fully use the allowed memory
 * with chained lists beginning in the last column
 * if the columns are not enough
 */
class MatrixHT {
public:
  // Chained tuple entry
  struct Entry {
    uint64_t key;
    Entry *next;

    Entry(){}
    Entry(uint64_t key) : key(key), next(nullptr) { }
  };

private:
  // constraint: (max 48 byte - sizeof(previously allocated entry)) / sizeof(key) = (48 - 16) / 8 = 4
  const unsigned COLUMNS = 4;
  const uint64_t INVALID_FLAG = std::numeric_limits<uint64_t>::max();
  std::atomic<uint64_t> *entries;
  uint64_t size;

public:
  // Constructor
  MatrixHT(uint64_t size) : size(size) {
    uint64_t entriesSize = size * COLUMNS;
    entries = new std::atomic<uint64_t>[entriesSize];
    for (unsigned i = 0; i < entriesSize; i++) {
      entries[i] = INVALID_FLAG;
    }
  }

  // Destructor
  ~MatrixHT() {
    delete[] entries;
  }

  // Returns the number of hits
  inline uint64_t lookup(uint64_t key) const {
    uint64_t hash = hashKey(key) % size;
    uint64_t r = hash * COLUMNS;
    uint64_t count = 0;

    unsigned c;
    for (c = 0; c < COLUMNS - 1; c++) {
      // TODO: jump table instead of branching
      if (entries[r + c] == INVALID_FLAG) {
        break;
      }
      if (entries[r + c] == key) {
        count++;
      }
    }
    if (c == COLUMNS - 1 && entries[r + c] != INVALID_FLAG) {
      uint64_t ptr = entries[r + c];
      Entry *entry = (Entry *) ptr;
      do {
        if(entry->key == key) {
          count++;
        }
        entry = entry->next;
      } while(entry != nullptr);
    }

    return count;
  }

  inline void insert(Entry *newEntry) {
    uint64_t newKey = newEntry->key;
#if MATRIX_DEBUG == 1
    if (newKey == INVALID_FLAG) {
      throw std::invalid_argument("Flag used as key");
    }
#endif

    uint64_t hash = hashKey(newKey) % size;

    uint64_t r = hash * COLUMNS;
    unsigned c = 0;
    uint64_t oldValue;
    uint64_t possiblePtr = INVALID_FLAG;
    do {
      for (; c < COLUMNS - 1 && entries[r + c] != INVALID_FLAG; c++);

      if (c == COLUMNS - 1) {
        possiblePtr = entries[r + c];
        if (possiblePtr != INVALID_FLAG) {
          break; // continue after loop
        }
      }
      oldValue = entries[r + c];
    } while (!entries[r + c]
        .compare_exchange_strong(oldValue, newKey,
                                 std::memory_order_seq_cst, std::memory_order_seq_cst));

    if (possiblePtr == INVALID_FLAG) {
      return;
    }

    uint64_t oldEntry; // is actually Entry *
    do {
      oldEntry = entries[r + COLUMNS];
      newEntry->next = (Entry *) oldEntry;
    } while (!entries[r + COLUMNS]
        .compare_exchange_strong(oldEntry, (uint64_t) newEntry,
                                 std::memory_order_seq_cst, std::memory_order_seq_cst));
  }

  inline void print() {
    for (unsigned r = 0; r < size; r += COLUMNS) {
      if (entries[r] == INVALID_FLAG) {
        continue;
      }
      printf("[%u]", r);
      for (unsigned i = r; i < COLUMNS - 1; i++) {
        uint64_t val = entries[i];
        if (val == INVALID_FLAG) {
          break;
        }
        printf(" | (%" PRIu64 ")", val);
      }
      uint64_t ptr = entries[r];
      if (ptr == INVALID_FLAG) {
        continue;
      }
      Entry *entryPtr = (Entry *) ptr;
      while (entryPtr != nullptr) {
        printf(" -> (%" PRIu64 ")", entryPtr->key);
        entryPtr = entryPtr->next;
      }
      printf("\n");
    }
  }
};

#endif //PROJECT_MATRIXHT_H
