#include <string>
#include <cassert>
#include <vector>
#include <sstream>
#include "../../src/database/buffer/BufferManager.hpp"
#include "../../src/database/btree/BTree.hpp"
#include "KeysAndComparators.hpp"

std::vector<std::string> char20;
std::vector<IntPair> intPairs;

template<typename T>
const T &getKey(const uint64_t &i);

template<>
const uint64_t &getKey(const uint64_t &i) { return i; }

template<>
const Char<20> &getKey(const uint64_t &i) {
  std::stringstream ss;
  ss << i;
  std::string s(ss.str());
  char20.push_back(std::string(20 - s.size(), '0') + s);
  return *reinterpret_cast<const Char<20> *>(char20.back().data());
}

template<>
const IntPair &getKey(const uint64_t &i) {
  intPairs.push_back(std::make_pair(i / 3, 3 - (i % 3)));
  return intPairs.back();
}

template<typename T, typename CMP>
void test(uint64_t n) {
  // Set up stuff, you probably have to change something here to match to your interfaces
  BufferManager bm(100);
  CMP smallerComparator;
  BTree<T, CMP> bTree(bm, 1, smallerComparator);


  // Insert values
  for (uint64_t i = 0; i < n; ++i)
    bTree.insert(getKey<T>(i), TID(i * i));
  assert(bTree.size() == n);
  uint64_t size = bTree.size();

  // Check if they can be retrieved
  for (uint64_t i = 0; i < n; ++i) {
    TID tid(0);
    assert(bTree.lookup(getKey<T>(i), tid));
    assert(tid == i * i);
  }

  // Delete some values
  for (uint64_t i = 0; i < n; ++i)
    if ((i % 7) == 0)
      bTree.erase(getKey<T>(i));

  // Check if the right ones have been deleted
  for (uint64_t i = 0; i < n; ++i) {
    TID tid(0);
    if ((i % 7) == 0) {
      assert(!bTree.lookup(getKey<T>(i), tid));
    } else {
      assert(bTree.lookup(getKey<T>(i), tid));
      assert(tid == i * i);
    }
  }

  // Delete everything
  for (uint64_t i = 0; i < n; ++i)
    bTree.erase(getKey<T>(i));
  assert(bTree.size() == 0);
}

int main(int argc, char *argv[]) {
  // Get command line argument
  const uint64_t n = (argc == 2) ? strtoul(argv[1], NULL, 10) : 1000 * 1000ul;

  // Test index with 64 bit unsigned integers
  test<uint64_t, MyCustomUInt64Cmp>(n);

  // Test index with 20 character strings
  test<Char<20>, MyCustomCharCmp<20>>(n);

  // Test index with compound key
  test<IntPair, MyCustomIntPairCmp>(n);
  return 0;
}
