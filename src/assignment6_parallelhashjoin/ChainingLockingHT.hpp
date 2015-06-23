#ifndef PROJECT_CHAININGLOCKINGHT_H
#define PROJECT_CHAININGLOCKINGHT_H

class ChainingLockingHT {
public:
  // Chained tuple entry
  struct ChainingLockingHT {
    uint64_t key;
    uint64_t value;
    Entry *next;
  };

  // Constructor
  ChainingLockingHT(uint64_t size) {
  }

  // Destructor
  ~ChainingLockingHT() {
  }

  inline Entry *lookup(uint64_t key) {
  }

  inline void insert(Entry *entry) {
  }
};

#endif //PROJECT_CHAININGLOCKINGHT_H
