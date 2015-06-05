//
// Created by Martin on 31.05.2015.
//

#include <sstream>
#include "InnerNode.hpp"
#include "EntriesHelper.inl.cpp"

template<typename KeyType, typename KeyComparator>
inline uint64_t InnerNode<KeyType, KeyComparator>::getNextNode(KeyType key, KeyComparator &smaller) {
  int min = 1;
  int max = header.keyCount;
  return EntriesHelper::findDirectionValue(entries, key, min, max, smaller);
}

template<typename KeyType, typename KeyComparator>
inline bool InnerNode<KeyType, KeyComparator>::hasSpaceForOneMoreEntry() {
  return this->header.keyCount < BTreeConstants<KeyType>::maxNodeCapacity;
}

template<typename KeyType, typename KeyComparator>
void InnerNode<KeyType, KeyComparator>::insertDefiniteFit(KeyType key, uint64_t leftValue, uint64_t rightValue,
                                                          KeyComparator &smaller) {
  int min = 1;
  int max = header.keyCount + 1;
  unsigned long insertPosition = EntriesHelper::findInsertPosition(entries, key, min, max, smaller);
  EntriesHelper::moveEntriesToRight(entries, insertPosition, header.keyCount + 1);
  entries[insertPosition].key = key;
  entries[insertPosition].value = rightValue;
  entries[insertPosition - 1].value = leftValue;
  header.keyCount++;
}

template<typename KeyType, typename KeyComparator>
std::string InnerNode<KeyType, KeyComparator>::print() {
  std::stringstream out;
  out << "InnerNode[keyCount=" << header.keyCount << "] ";
  if (header.keyCount > 0) {
    for (int i = 0; i < header.keyCount + 1; i++) {
      if (i == 0) {
        out << "(" << entries[i].value;
      } else {
        out << ") " << entries[i].key << " (" << entries[i].value;
      }
    }
    out << ")";
  }
  out << "\n";
  return out.str();
}
