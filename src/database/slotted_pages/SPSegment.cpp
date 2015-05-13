//
// Created by Martin on 12.05.2015.
//

#include <stdint.h>
#include <stdexcept>
#include "SPSegment.hpp"

using SlottedPage::Slot;

TID SPSegment::insert(const Record &record) {
	size_t data_size = record.getLen();
	BufferFrame frame = this->findOrCreatePage(data_size);
	SlottedPage page = *reinterpret_cast<SlottedPage *>(&frame);
	uint16_t slotId = page.createAndWriteSlot(record.getData(), data_size);
	this->bufferManager.unfixPage(frame, true);
	return TID((uint16_t) frame.getPageId(), slotId);

	// when we redirect, append the 8 byte TID in front of the actual data.
	// needed for a full table scan, where we scan over memory
}

bool SPSegment::update(TID tid, const Record &record) {
	// TODO: we might have to reorder the data
	return false;
}

Record SPSegment::lookup(TID tid) {
	uint64_t pageId = tid.pageId;
	uint16_t slotOffset = tid.slotOffset;

	BufferFrame frame = this->bufferManager.fixPage(this->segmentId, pageId, false);
	SlottedPage page = *reinterpret_cast<SlottedPage *>(&frame);
	SlottedPage::Slot slot = page.slots[slotOffset];
	if (slot.isTid()) {
		TID redirectTid(0, 0);
		memcpy(&redirectTid, &slot, sizeof(Slot));
		return lookup(redirectTid);
	}

	char *dataPtr = (char *) slot.O;
	uint32_t length = slot.L;
	this->bufferManager.unfixPage(frame, false);
	Record record(length, dataPtr);
	return record;
}

BufferFrame &SPSegment::findOrCreatePage(size_t data_size) {
	for (unsigned i(0); i < this->pageCount; i++) {
		BufferFrame frame = this->bufferManager.fixPage(i, true);
		SlottedPage page = *reinterpret_cast<SlottedPage *>(&frame);
		if (page.hasSpaceAtDataFront(data_size)) {
			return frame;
		}

		if (page.canMakeSpace(data_size)) {
			page.compactify();
			return frame;
		}

		// frame does not match criteria -> next
		this->bufferManager.unfixPage(frame, false);
	}

	// no page found -> create new
	BufferFrame frame = this->bufferManager.fixPage(this->pageCount + 1, true);
	this->pageCount++;
	return frame;
}

bool SPSegment::remove(TID tid) {
	return false;
}
