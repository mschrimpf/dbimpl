#ifndef PROJECT_MATRIXHT_H
#define PROJECT_MATRIXHT_H

#include "hash.hpp"
#include "inttypes_wrapper.hpp"
#include <atomic>
#include <stdio.h>
#include <limits>
#include <stdexcept>
#include <string.h>

#define MATRIX_DEBUG 0

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

    Entry() { }

    Entry(uint64_t key) : key(key), next(nullptr) { }
  };

private:
  // constraint: (max 48 byte - sizeof(previously allocated entry)) / sizeof(key) = (48 - 16) / 8 = 4
  const unsigned COLUMNS = 4;
  const uint64_t INVALID_FLAG = std::numeric_limits<uint64_t>::max();
  std::atomic<uint64_t> *entries;
  uint64_t numRows;

public:
  // Constructor
  MatrixHT(uint64_t size) : numRows(size) {
    uint64_t entriesSize = size * COLUMNS;
    entries = new std::atomic<uint64_t>[entriesSize];
    int invalid_int_flag = std::numeric_limits<int>::max();
    memset(entries, invalid_int_flag, (size_t) entriesSize * sizeof(std::atomic<uint64_t>));
  }

  // Destructor
  ~MatrixHT() {
    delete[] entries;
  }

  // Returns the number of hits
  inline uint64_t lookup(uint64_t key) const {
    uint64_t hash = hashKey(key) % numRows;
    uint64_t r = hash * COLUMNS;
    uint64_t count = 0;

    unsigned c;
    for (c = 0; c < COLUMNS - 1; c++) {
      if (entries[r + c] == INVALID_FLAG) {
        return count;
      }
      // branch-free. equivalent to if (entries[r + c] == key) count++
      bool inc = entries[r + c] == key;
      count += inc;
    }
    if (c == COLUMNS - 1 && entries[r + c] != INVALID_FLAG) {
      uint64_t ptr = entries[r + COLUMNS - 1];
      Entry *entry = (Entry *) ptr;
      do {
        // branch-free. equivalent to if(entry->key == key) count++
        bool inc = entry->key == key;
        count += inc;
        entry = entry->next;
      } while (entry != nullptr);
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

    uint64_t hash = hashKey(newKey) % numRows;

    uint64_t r = hash * COLUMNS;
    unsigned c = 0;
    uint64_t possiblePtr = INVALID_FLAG;
    uint64_t invalid_flag = INVALID_FLAG;
    do {
      for (; c < COLUMNS - 1 && entries[r + c] != INVALID_FLAG; c++);

      if (c == COLUMNS - 1) {
        break;
      }
    } while (!entries[r + c]
        .compare_exchange_strong(invalid_flag, newKey,
                                 std::memory_order_seq_cst, std::memory_order_seq_cst));

    if (c != COLUMNS - 1) {
      return;
    }

    uint64_t oldEntry; // is actually Entry *
    do {
      oldEntry = entries[r + COLUMNS - 1];
      bool isValid = oldEntry != INVALID_FLAG;
      newEntry->next = (Entry *) (oldEntry * isValid + 0 * !isValid);
    } while (!entries[r + COLUMNS - 1]
        .compare_exchange_strong(oldEntry, (uint64_t) newEntry,
                                 std::memory_order_seq_cst, std::memory_order_seq_cst));
    uint64_t newEntriesVal = entries[r + COLUMNS - 1];
  }

  inline void print() {
    for (unsigned r = 0; r < numRows * COLUMNS; r += COLUMNS) {
      if (entries[r] == INVALID_FLAG) {
        continue;
      }
      printf("[%u]", r / COLUMNS);
      unsigned i;
      for (i = r; i < r + COLUMNS - 1; i++) {
        uint64_t val = entries[i];
        if (val == INVALID_FLAG) {
          break;
        }
        printf(" | (%" PRIu64 ")", val);
      }
      if (i == r + COLUMNS - 1) {
        uint64_t ptr = entries[i];
        if (ptr != INVALID_FLAG) {
          Entry *entryPtr = (Entry *) ptr;
          printf(" | %p", entryPtr);
          while (entryPtr != nullptr) {
            printf(" -> (%" PRIu64 ")\n", entryPtr->key);
            entryPtr = entryPtr->next;
          }
        }
      }
      printf("\n");
    }
  }
};

#endif //PROJECT_MATRIXHT_H
