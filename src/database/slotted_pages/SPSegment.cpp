//
// Created by Martin on 12.05.2015.
//

#include <stdint.h>
#include <stdexcept>
#include "SPSegment.hpp"

// TODO: when should we use & ?

SPSegment::SPSegment(BufferManager &bufferManager) : bufferManager(bufferManager) {
}

TID SPSegment::insert(const Record &record) {
	size_t data_size = record.getLen();
	BufferFrame frame = this->findOrCreatePage(data_size);
	SlottedPage page = *reinterpret_cast<SlottedPage *>(&frame);
	TID tid = page.createAndWriteSlot(record.getData(), data_size);
	this->bufferManager.unfixPage(frame, true);
	return tid;

	// when we redirect, append the 8 byte TID in front of the actual data.
	// needed for a full table scan, where we scan over memory
}

bool SPSegment::update(TID tid, const Record &record) {
	// TODO: we might have to reorder the data
}

Record SPSegment::lookup(TID tid) {
	SPSegment::SlotData data = this->lookupData(tid);
	Record record(data.length, data.ptr);
	return record;
}

SPSegment::SlotData SPSegment::lookupData(TID tid) {
	uint64_t pageId = tid.pageId;
	uint16_t slotOffset = tid.slotOffset;

	SlottedPage *page = this->pageIdSlottedPageMap[pageId];
	SlottedPage::Slot slot = page->slots[slotOffset];
	char *dataPtr = page->header.dataStart + slot.offset;
	uint16_t length = slot.length;
	if (slot.is_tid == 1) {
		TID redirectTid;
		memcpy(&redirectTid, dataPtr, (size_t) length);
		return lookupData(redirectTid);
	}

	SlotData data;
	data.length = length;
	data.ptr = dataPtr;
	return data;
}

BufferFrame &SPSegment::findOrCreatePage(size_t data_size) {
	for (unsigned i(0); i < this->pageCount; i++) {
		BufferFrame frame = this->bufferManager.fixPage(i, true);
		SlottedPage page = *reinterpret_cast<SlottedPage *>(&frame);
		if (page.hasSpaceAtDataFront()) {
			return frame;
		}

		if (page.canMakeSpace(data_size)) {
			page.fragment();
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
