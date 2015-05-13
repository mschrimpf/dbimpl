//
// Created by Martin on 13.05.2015.
//

#include "SlottedPage.hpp"

using SlottedPage::Slot;

bool SlottedPage::Slot::isTid() {
	return this->T == 0x11111111b;
}

bool SlottedPage::Slot::wasRedirect() {
	return this->S != 0;
}

void SlottedPage::Slot::nullTS() {
	this->T = this->S = 0;
}

uint16_t SlottedPage::createAndWriteSlot(const char *data, size_t data_size) {
	Slot *slot = nullptr;
	uint16_t slotIterator(0);
	for (; slotIterator < this->header.slotCount; slotIterator++) {
		if (this->slots[slotIterator].isRemoved()) {
			slot = &this->slots[slotIterator];
			break;
		}
	}
	if (slot == nullptr) {
		slot = &this->slots[slotIterator];
	}

	// TODO: write data
	slot->nullTS();
	char *dataAddress = this->header.dataStart - data_size;
	slot->O = (uint32_t) dataAddress;
	slot->L = data_size;

	memcpy(dataAddress, data, data_size);

	return slotIterator;
}

bool SlottedPage::hasSpaceAtDataFront(size_t data_size) {
	size_t freeSpaceBetweenSlotsAndData = spaceBetweenSlotsAndData();
	return freeSpaceBetweenSlotsAndData >= data_size;
}

size_t SlottedPage::spaceBetweenSlotsAndData() const {
	Slot *slotStart = (Slot *) slots;
	Slot *slotEnd = slotStart + header.slotCount * sizeof(Slot);
	return header.dataStart - (char *) slotEnd;
}

bool SlottedPage::canMakeSpace(size_t necessary_space) {
	size_t totalFreeSpace = this->header.fragmentedSpace + this->spaceBetweenSlotsAndData();
	return totalFreeSpace >= necessary_space;
}
