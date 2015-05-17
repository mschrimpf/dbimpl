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

bool SlottedPage::Slot::isTid() {
	return T == 0x11111111b;
}

TID SlottedPage::Slot::getTid() {
	TID tid(0, 0);
	std::memcpy(&tid, this, sizeof(SlottedPage::Slot));
	return tid;
}

bool SlottedPage::Slot::wasRedirect() {
	return S != 0;
}

void SlottedPage::Slot::nullTS() {
	T = 0;
	S = 0;
}

void SlottedPage::Slot::markAsFree() {
	O = 0;
	L = 0;
}

bool SlottedPage::Slot::isFree() {
	return O == 0 && L == 0;
}

bool SlottedPage::Slot::isEmptyData() {
	return O > 0 && L == 0;
}

uint16_t SlottedPage::createAndWriteSlot(const char *data, size_t data_size) {
	uint16_t slotIndex = this->header.firstFreeSlot;
	debug("First free slot: %" PRIu16 " | slotCount: %u", slotIndex, header.slotCount);
	SlottedPage::Slot &slot = this->slots[slotIndex];
	if (slotIndex == this->header.slotCount) {
		this->header.slotCount++;
	}
	uint16_t newFreeSlot = findFirstFreeSlot(slotIndex);
	header.firstFreeSlot = newFreeSlot;

	// write data
	debug("Write data");
	slot.nullTS();
	char *dataAddress = this->header.dataStart - data_size;
	slot.O = dataAddress - (char *) (SlottedPage::Slot *) slots;
	slot.L = data_size;

	std::memcpy(dataAddress, data, data_size);

	return slotIndex;
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
	SlottedPage::Slot *slotStart = (SlottedPage::Slot *) slots;
	SlottedPage::Slot *slotEnd = slotStart + header.slotCount * sizeof(Slot);
	return header.dataStart - (char *) slotEnd;
}

bool SlottedPage::canMakeEnoughSpace(size_t necessary_space) {
	size_t totalFreeSpace = this->header.fragmentedSpace + this->spaceBetweenSlotsAndData();
	return totalFreeSpace >= necessary_space;
}

void SlottedPage::compactify(char *pageEndPtr) {
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

SlottedPage::Slot *SlottedPage::getExistingSlot(uint16_t slotOffset) {
	debug("Retrieving slot %" PRIu16 " | slotCount: %u", slotOffset, header.slotCount);
	if (!isInRange(slotOffset)) {
		throw std::invalid_argument("slotOffset is greater than or equal to the slotCount");
	}
	return &this->slots[slotOffset];
}

char *SlottedPage::getSlotData(SlottedPage::Slot &slot) {
	return (char *) &this->slots[0] + (intptr_t) slot.O;
}

void SlottedPage::setSlotOffset(SlottedPage::Slot &slot, char *data_ptr) {
	slot.O = data_ptr - (char *) &this->slots[0];
}

bool SlottedPage::isLastSlot(SlottedPage::Slot &slot) {
	return slot.O + (char *) (SlottedPage::Slot *) slots == this->header.dataStart;
}

bool SlottedPage::isInRange(uint16_t slotOffset) {
	return slotOffset < this->header.slotCount;
}
