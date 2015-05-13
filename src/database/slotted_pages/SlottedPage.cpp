//
// Created by Martin on 13.05.2015.
//

#include "SlottedPage.hpp"
#include <vector>
#include <algorithm>
#include <cstring> // memcpy

// TODO: use firstFreeSlot

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
	uint16_t slotIndex = this->header.firstFreeSlot;
	SlottedPage::Slot &slot = this->slots[slotIndex];
	for (uint16_t slotIterator = (uint16_t) (slotIndex + 1);
		 slotIterator <= this->header.slotCount; // slot after the last slot must be free
		 slotIterator++) {
		SlottedPage::Slot potentiallyFreeSlot = this->slots[slotIterator];
		if (potentiallyFreeSlot.isFree()) {
			this->header.firstFreeSlot = slotIterator;
			break;
		}
	}
	if (this->header.firstFreeSlot == this->header.slotCount) {
		this->header.slotCount++;
	}

	// write data
	slot.nullTS();
	char *dataAddress = this->header.dataStart - data_size;
	slot.O = dataAddress - (char *) (SlottedPage::Slot *) slots;
	slot.L = data_size;

	std::memcpy(dataAddress, data, data_size);

	return slotIndex;
}

bool SlottedPage::hasSpaceAtDataFront(size_t data_size) {
	size_t freeSpaceBetweenSlotsAndData = spaceBetweenSlotsAndData();
	return freeSpaceBetweenSlotsAndData >= data_size;
}

size_t SlottedPage::spaceBetweenSlotsAndData() const {
	SlottedPage::Slot *slotStart = (SlottedPage::Slot *) slots;
	SlottedPage::Slot *slotEnd = slotStart + header.slotCount * sizeof(Slot);
	return header.dataStart - (char *) slotEnd;
}

bool SlottedPage::canMakeEnoughSpace(size_t necessary_space) {
	size_t totalFreeSpace = this->header.fragmentedSpace + this->spaceBetweenSlotsAndData();
	return totalFreeSpace >= necessary_space;
}

bool SlottedPage::Slot::isFree() {
	return O == L == 0;
}

bool SlottedPage::Slot::isEmptyData() {
	return O > 0 && L == 0;
}

void SlottedPage::compactify(char * pageEndPtr) {
	std::vector<SlottedPage::Slot *> sortedSlots;
	sortedSlots.reserve(this->header.slotCount);
	for (int i = 0; i < this->header.slotCount; i++) {
		sortedSlots.emplace_back(&this->slots[i]);
	}
	struct less_than_key {
		inline bool operator()(const SlottedPage::Slot *slot1, const SlottedPage::Slot *slot2) {
			return (slot1->O > slot2->O); // last data pointer at the beginning
		}
	};
	std::sort(sortedSlots.begin(), sortedSlots.end(), less_than_key());

	char *prev_end = pageEndPtr;
	for (auto slot : sortedSlots) {
		char *begin = prev_end - slot->L;
		memcpy(begin, this->getSlotData(*slot), slot->L);
		this->setSlotOffset(*slot, begin);
		prev_end = begin;
	}
	this->header.fragmentedSpace = 0;
}

char *SlottedPage::getSlotData(SlottedPage::Slot &slot) {
	return (char *) &this->slots[0] + (intptr_t) slot.O;
}

void SlottedPage::setSlotOffset(SlottedPage::Slot &slot, char *data_ptr) {
	slot.O = data_ptr - (char*) &this->slots[0];
}
