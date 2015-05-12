//
// Created by Martin on 13.05.2015.
//

#include "SlottedPage.hpp"

bool SlottedPage::Slot::isTid() {
	return this->T == 0x11111111b;
}

bool SlottedPage::Slot::wasRedirect() {
	return this->S != 0;
}

uint32_t SlottedPage::Slot::getOffset() {
	return this->O;
}

uint32_t SlottedPage::Slot::getLength() {
	return this->L;
}

TID SlottedPage::createAndWriteSlot(const char *data, size_t data_size) {
	Slot *slot = nullptr;
	unsigned slotIterator(0);
	for (; slotIterator < this->header.slotCount; slotIterator++) {
		if (this->slots[slotIterator].isRemoved()) {
			slot = &this->slots[slotIterator];
			break;
		}
	}
	if (slot == nullptr) {
		slot = &this->slots[slotIterator];
		slot.clear();
	}

	// TODO: write data
}
