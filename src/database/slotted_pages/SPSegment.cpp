//
// Created by Martin on 12.05.2015.
//

#include <stdint.h>
#include <stdexcept>
#include "SPSegment.hpp"

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
