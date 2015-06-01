//
// Created by Martin on 31.05.2015.
//

#ifndef PROJECT_KEYSANDCOMPARATORS_HPP
#define PROJECT_KEYSANDCOMPARATORS_HPP

/* Comparator functor for uint64_t*/
struct MyCustomUInt64Cmp {
  bool operator()(uint64_t a, uint64_t b) const {
    return a<b;
  }
};

template <unsigned len>
struct Char {
  char data[len];
};

/* Comparator functor for char */
template <unsigned len>
struct MyCustomCharCmp {
  bool operator()(const Char<len>& a, const Char<len>& b) const {
    return memcmp(a.data, b.data, len) < 0;
  }
};

typedef std::pair<uint32_t, uint32_t> IntPair;

/* Comparator for IntPair */
struct MyCustomIntPairCmp {
  bool operator()(const IntPair& a, const IntPair& b) const {
    if (a.first < b.first)
      return true;
    else
      return (a.first == b.first) && (a.second < b.second);
  }
};

#endif //PROJECT_KEYSANDCOMPARATORS_HPP
