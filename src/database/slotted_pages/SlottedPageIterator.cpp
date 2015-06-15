//
// Created by Martin on 13.05.2015.
//

#include "SlottedPage.hpp"

#ifdef _WIN32
#define PRIu32 "lu"
#define PRIu16 "u"
#define PRIu8 "u"
#else
#include <inttypes.h>
#endif

SlottedPage::iterator SlottedPage::begin() const {
  return iterator(0, slots);
}

SlottedPage::iterator SlottedPage::end() const {
  return iterator(header.slotCount, slots);
}

bool SlottedPage::iterator::operator==(const SlottedPage::iterator &other) const {
  return index == other.index && slots == other.slots;
}

bool SlottedPage::iterator::operator!=(const SlottedPage::iterator &other) const {
  return !(*this == other);
}

SlottedPage::iterator &SlottedPage::iterator::operator++() {
  index++;
  return *this;
}

const Slot &SlottedPage::iterator::operator*() const {
  return slots[index];
}
