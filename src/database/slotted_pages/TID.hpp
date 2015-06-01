//
// Created by Martin on 12.05.2015.
//

#ifndef PROJECT_TID_HPP
#define PROJECT_TID_HPP

#include <stdint.h>
#include <functional>
#include "../buffer/BufferManager.hpp"

struct TID {
  TID(uint16_t slotOffset, uint64_t pageId) : slotOffset(slotOffset), pageId(pageId) { }

  TID(uint64_t id) : slotOffset((uint16_t) (id & BufferManager::SEGMENT_MASK)),
                     pageId(id & BufferManager::PAGE_MASK) { }

  TID() : slotOffset(0), pageId(0) { }

  uint16_t slotOffset;
  uint64_t pageId : 48;

  bool operator==(const TID &other) const {
    return pageId == other.pageId
           && slotOffset == other.slotOffset;
  }
};

namespace std {
  template<>
  struct hash<TID> {
    size_t operator()(const TID &tid) const {
      // Compute individual hash values for first,
      // second and third and combine them using XOR
      // and bit shifting:

      return (hash<uint64_t>()(tid.pageId)
              ^ (hash<uint16_t>()(tid.slotOffset) << 1)) >> 1;
    }
  };

}

#endif //PROJECT_TID_HPP
