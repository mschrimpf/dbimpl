//
// Created by Martin on 12.05.2015.
//

#ifndef PROJECT_SPSEGMENT_H
#define PROJECT_SPSEGMENT_H

#include "TID.hpp"
#include "SlottedPage.hpp"
#include "Record.hpp"
#include <vector>

// compactify may not move the slots since that would invalidate the TIDs (page + slot)

// insert and lookup: copy record - otherwise pointer invalid
// lookup: no lock

class SPSegment {
private:
  BufferManager &bufferManager;
  uint64_t segmentId;
  /** Page count or max page (equivalent) */
  uint64_t maxPageId;

  TID insert(const Record &record, BufferFrame &frame);

  BufferFrame &findOrCreatePage(size_t data_size);

  SlottedPage *toSlottedPage(BufferFrame &frame) const;

  void insertAtLocation(TID tid, const Record &record);

  BufferFrame &findOrCreatePage(unsigned int data_size, uint64_t *excludedPageId);

public:
  SPSegment(BufferManager &bufferManager, uint64_t segmentId)
      : bufferManager(bufferManager), segmentId(segmentId), maxPageId(0) { };

  TID insert(const Record &record);

  bool remove(TID tid);

  Record lookup(TID tid);

  bool update(TID tid, const Record &record);

  Record getSlotRecord(SlottedPage &page, const Slot &slot) const;

  //////////////////////////////////////////////////////////////////
  // Iterator
  //////////////////////////////////////////////////////////////////

  class iterator : public std::iterator<std::forward_iterator_tag, Record> {
  private:
    const SPSegment *segment;
    uint64_t pageId;
    BufferFrame *currFrame;
    SlottedPage *currPage;
    SlottedPage::iterator slotIter;
    SlottedPage::iterator slotEnd;

  public:
    static iterator fromSegmentAndPage(const SPSegment *segment, uint64_t pageId);

    iterator(const SPSegment *segment, uint64_t pageId, BufferFrame *frame);

    iterator(const SPSegment *segment, uint64_t pageId)
        : iterator(segment, pageId, nullptr) { }

    iterator() : iterator(nullptr, 0) { }

    iterator &operator++();

    bool operator==(const iterator &other) const;

    bool operator!=(const iterator &other) const;

    const Record operator*() const;

  private:
    void initPageIter();
  };

  iterator begin() const;

  iterator end() const;
};

#endif //PROJECT_SPSEGMENT_H

