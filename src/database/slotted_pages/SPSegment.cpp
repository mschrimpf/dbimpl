//
// Created by Martin on 12.05.2015.
//

#include "SPSegment.hpp"
#include <stdint.h>
#include <stdexcept>
#include <cstring> // memcpy
#include "../util/debug.h"

#ifdef _WIN32
#define PRId64 "llu"
#define PRId16 "u"
#else
#include <inttypes.h>
#endif

TID SPSegment::insert(const Record &record) {
	size_t data_size = record.getLen();
	BufferFrame frame = this->findOrCreatePage(data_size);
	SlottedPage *page = toSlottedPage(frame);
	const char *dataPtr = record.getData();
	uint16_t slotId = page->createAndWriteSlot(dataPtr, data_size);
	uint64_t pageId = frame.getPageId();
	this->bufferManager.unfixPage(frame, true);
	return TID(slotId, pageId);

	// when we redirect, append the 8 byte TID in front of the actual data.
	// needed for a full table scan, where we scan over memory
}

bool SPSegment::remove(TID tid) {
	uint64_t pageId = tid.pageId;
	uint16_t slotOffset = tid.slotOffset;

	BufferFrame &frame = this->bufferManager.fixPage(this->segmentId, pageId, true);
	SlottedPage *page = toSlottedPage(frame);
	if (!page->isInRange(slotOffset)) {
		return false;
	}
	Slot *slot = page->getExistingSlot(slotOffset);
	if (slot->isFree()) {
		return false;
	}
	if (slot->isTid()) {
		TID redirectTid = slot->getTid();
		remove(redirectTid);
	}
	slot->markAsFree();
	if (!page->isLastSlot(*slot)) {
		page->header.fragmentedSpace += slot->L;
	}

	this->bufferManager.unfixPage(frame, true);
	return true;
}

bool SPSegment::update(TID tid, const Record &record) {
	return false;
}

Record SPSegment::lookup(TID tid) {
	uint64_t pageId = tid.pageId;
	uint16_t slotOffset = tid.slotOffset;

	BufferFrame &frame = this->bufferManager.fixPage(this->segmentId, pageId, false);
	SlottedPage *page = toSlottedPage(frame);
	debug("Retrieving slot %" PRId16 " from page %" PRId64, slotOffset, pageId);
	Slot *slot = page->getExistingSlot(slotOffset);
	if (slot->isFree()) {
		throw std::invalid_argument("Requested slot is free (marked as deleted)");
	}
	if (slot->isTid()) {
		TID redirectTid = slot->getTid();
		return lookup(redirectTid);
	}

	char *dataPtr = page->getSlotData(*slot);
	uint32_t length = slot->L;
	this->bufferManager.unfixPage(frame, false);
	Record record(length, dataPtr);
	return record;
}

BufferFrame &SPSegment::findOrCreatePage(size_t data_size) {
	// attempt to find existing pages with enough space
	for (uint64_t pageId(0); pageId < this->maxPageId; pageId++) {
		debug("Find or create page %" PRId64, pageId);
		BufferFrame &frame = this->bufferManager.fixPage(this->segmentId, pageId, true);
		SlottedPage *page = toSlottedPage(frame);
		if (page->hasSpaceAtDataFront(data_size)) {
			return frame;
		}

		if (page->canMakeEnoughSpace(data_size)) {
			char *pageEndPtr = (char *) &page + sizeof(SlottedPage);
			page->compactify(pageEndPtr);
			return frame;
		}

		// frame does not match criteria -> next
		this->bufferManager.unfixPage(frame, false);
	}

	// no page found -> create new
	BufferFrame &frame = this->bufferManager.fixPage(this->segmentId, this->maxPageId + 1, true);
	this->maxPageId++;
	return frame;
}

SlottedPage *SPSegment::toSlottedPage(BufferFrame &frame) const {
	SlottedPage *page = reinterpret_cast<SlottedPage *>(frame.getData());
	page->init();
	return page;
}
