//
// Created by Martin on 15.06.2015.
//

#include <stdio.h>
#include "SPSegment.hpp"

SPSegment::iterator SPSegment::begin() const {
  return SPSegment::iterator::fromSegmentAndPage(*this, 0);
}

SPSegment::iterator SPSegment::end() const {
  return SPSegment::iterator(*this, maxPageId);
}

SPSegment::iterator SPSegment::iterator::fromSegmentAndPage(const SPSegment &segment, uint64_t pageId) {
  BufferFrame &frame = segment.bufferManager.fixPage(segment.segmentId, pageId, false);
  return SPSegment::iterator(segment, pageId, &frame);
}


SPSegment::iterator::iterator(const SPSegment &segment, uint64_t pageId, BufferFrame *frame)
    : segment(segment), pageId(pageId),
      currFrame(frame), currPage(nullptr) {
  if (frame != nullptr) {
    initPageIter();
  }
}

void SPSegment::iterator::initPageIter() {
  this->currPage = segment.toSlottedPage(*currFrame);
  this->slotIter = this->currPage->begin();
  this->slotEnd = this->currPage->end();
}

bool SPSegment::iterator::operator==(const SPSegment::iterator &other) const {
  return segment.segmentId == other.segment.segmentId && pageId == other.pageId;
}

bool SPSegment::iterator::operator!=(const SPSegment::iterator &other) const {
  return !(*this == other);
}

SPSegment::iterator &SPSegment::iterator::operator++() {
  bool slotIterReinitialized = false;
  while (pageId < segment.maxPageId) {
    if (slotIterReinitialized) {
      slotIterReinitialized = false;
    } else {
      ++slotIter;
    }
    while (slotIter != slotEnd) { // FIXME: first iter is skipped
      const Slot &slot = *slotIter;
      if (!slot.isFree()) {
        return *this;
      }
      ++slotIter;
    }

    segment.bufferManager.unfixPage(*currFrame, false);
    pageId++;
    if (pageId < segment.maxPageId) {
      currFrame = &segment.bufferManager.fixPage(segment.segmentId, pageId, false);
      initPageIter();
      slotIterReinitialized = true;
    }
  }
  segment.bufferManager.unfixPage(*currFrame, false);

  return *this;
}

const Record SPSegment::iterator::operator*() const {
  const Slot &slot = *slotIter;
  return segment.getSlotRecord(*currPage, slot);
}
