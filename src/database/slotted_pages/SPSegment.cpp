//
// Created by Martin on 12.05.2015.
//

#include "SPSegment.hpp"
#include <stdint.h>
#include <stdexcept>
#include <cstring> // memcpy

TID SPSegment::insert(const Record &record) {
	size_t data_size = record.getLen();
	BufferFrame frame = this->findOrCreatePage(data_size);
	SlottedPage * page = toSlottedPage(frame);
	uint16_t slotId = page->createAndWriteSlot(record.getData(), data_size);
	this->bufferManager.unfixPage(frame, true);
	return TID((uint16_t) frame.getPageId(), slotId);

	// when we redirect, append the 8 byte TID in front of the actual data.
	// needed for a full table scan, where we scan over memory
}

bool SPSegment::remove(TID tid) {
	return false;
}

bool SPSegment::update(TID tid, const Record &record) {
	// TODO: we might have to reorder the data
	return false;
}

Record SPSegment::lookup(TID tid) {
	uint64_t pageId = tid.pageId;
	uint16_t slotOffset = tid.slotOffset;

	BufferFrame& frame = this->bufferManager.fixPage(this->segmentId, pageId, false);
	SlottedPage * page = toSlottedPage(frame);
	SlottedPage::Slot slot = page->slots[slotOffset];
	if (slot.isTid()) {
		TID redirectTid(0, 0);
		std::memcpy(&redirectTid, &slot, sizeof(SlottedPage::Slot));
		return lookup(redirectTid);
	}

	char *dataPtr = page->getSlotData(slot);
	uint32_t length = slot.L;
	this->bufferManager.unfixPage(frame, false);
	Record record(length, dataPtr);
	return record;
}

BufferFrame &SPSegment::findOrCreatePage(size_t data_size) {
	for (unsigned pageId(0); pageId < this->pageCount; pageId++) {
		BufferFrame& frame = this->bufferManager.fixPage(this->segmentId, pageId, true);
		SlottedPage * page = toSlottedPage(frame);
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
	BufferFrame& frame = this->bufferManager.fixPage(this->segmentId, this->pageCount + 1, true);
	this->pageCount++;
	return frame;
}

SlottedPage * SPSegment::toSlottedPage(BufferFrame &frame) const { 
	SlottedPage * page = reinterpret_cast<SlottedPage *>(frame.getData());
	page->init();
	return page;
}
