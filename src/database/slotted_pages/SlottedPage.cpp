//
// Created by Martin on 13.05.2015.
//

#include "SlottedPage.hpp"
#include <vector>
#include <algorithm>

#ifdef _WIN32
#define PRIu32 "lu"
#define PRIu16 "u"
#define PRIu8 "u"
#else
#include <inttypes.h>
#endif

#include <cstring> // memcpy
#include "../util/debug.h"

// TODO: use firstFreeSlot

SlottedPage::SlottedPage() {
	init();
}

void SlottedPage::init() {
	if (!header.dataStart) {
		header.dataStart = data + sizeof(char) * (BufferManager::FRAME_SIZE_BYTE - sizeof(SPHeader));
	}
}

uint16_t SlottedPage::createAndWriteSlot(const char *data, size_t data_size) {
	uint16_t slotIndex;
	Slot * slot = retrieveSlot(slotIndex);

	writeSlotData(slot, data, data_size);

	return slotIndex;
}

Slot * SlottedPage::retrieveSlot(uint16_t &slotIndex) {
	slotIndex = header.firstFreeSlot;
	Slot * slot = &slots[slotIndex];
	debug("First free slot: %" PRIu16 " | slotCount: %u", slotIndex, header.slotCount);
	if (slotIndex == header.slotCount) {
		header.slotCount++;
	}
	uint16_t newFreeSlot = findFirstFreeSlot(slotIndex);
	header.firstFreeSlot = newFreeSlot;
	return slot;
}

void SlottedPage::writeSlotData(Slot *slot, const char *data, size_t data_size) const {
	debug("Write data");
	slot->nullTS();
	char *dataAddress = header.dataStart - data_size;
	slot->O = dataAddress - (char *) (Slot *) slots;
	slot->L = data_size;

	memcpy(dataAddress, data, data_size);
}

uint16_t SlottedPage::findFirstFreeSlot(uint16_t lastFreeSlot) {
	for (uint16_t slotIterator = (uint16_t) (lastFreeSlot + 1);
		 slotIterator <= header.slotCount; // slot after the last slot must be free
		 slotIterator++) {
		Slot potentiallyFreeSlot = slots[slotIterator];
		debug("Slot[%" PRIu32 "]: T=%" PRIu32 " | S=%" PRIu32 " | O=%" PRIu32 " | L=%" PRIu32, slotIterator,
			  potentiallyFreeSlot.T, potentiallyFreeSlot.S, potentiallyFreeSlot.O, potentiallyFreeSlot.L);
		if (potentiallyFreeSlot.isFree()) {
			return slotIterator;
		}
	}
	throw std::invalid_argument("Could not find free slot");
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

bool SlottedPage::canMakeEnoughSpace(size_t necessary_space) {
	size_t totalFreeSpace = this->header.fragmentedSpace + this->spaceBetweenSlotsAndData();
	return totalFreeSpace >= necessary_space;
}

void SlottedPage::compactify(char *pageEndPtr) {
	std::vector<Slot *> sortedSlots;
	sortedSlots.reserve(this->header.slotCount);
	for (int i = 0; i < this->header.slotCount; i++) {
		sortedSlots.emplace_back(&this->slots[i]);
	}
	struct less_than_key {
		inline bool operator()(const Slot *slot1, const Slot *slot2) {
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

Slot *SlottedPage::getExistingSlot(uint16_t slotOffset) {
	debug("Retrieving slot %" PRIu16 " | slotCount: %u", slotOffset, header.slotCount);
	if (!isInRange(slotOffset)) {
		throw std::invalid_argument("slotOffset is greater than or equal to the slotCount");
	}
	return &this->slots[slotOffset];
}

char *SlottedPage::getSlotData(Slot &slot) {
	return (char *) &this->slots[0] + (intptr_t) slot.O;
}

void SlottedPage::setSlotOffset(Slot &slot, char *data_ptr) {
	slot.O = data_ptr - (char *) &this->slots[0];
}

bool SlottedPage::isLastSlot(Slot &slot) {
	return slot.O + (char *) (Slot *) slots == this->header.dataStart;
}

bool SlottedPage::isInRange(uint16_t slotOffset) {
	return slotOffset < this->header.slotCount;
}
