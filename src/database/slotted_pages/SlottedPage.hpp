//
// Created by Martin on 05.05.2015.
//

#ifndef PROJECT_SLOTTEDPAGE_H
#define PROJECT_SLOTTEDPAGE_H

#include <stdint.h>
#include <sys/types.h>
#include <unordered_map>
#include "../buffer/BufferManager.hpp"
#include "TID.hpp"
#include "Slot.hpp"

// TODO: might have to implement redirects
// (initial implementation could be to just throw an exception when the page is full)
struct SlottedPage {
	struct SPHeader {
		uint64_t LSN;
		/**
		 * Size of the slot array.
		 * Unless the very last slot is deleted, this value only grows, but never decreases.
		 */
		unsigned slotCount;
		uint16_t firstFreeSlot;
		char *dataStart;
		/**
		 * This will always be zero when data is inserted one after another.
		 * However, when data is deleted, space might become free
		 * and fragment our total available space.
		 */
		unsigned fragmentedSpace;
	} header;

	union {
		/*
		 * Variable-sized array -> store at the end.
		 * Since the SlottedPage will be allocated with a variable length,
		 * this variable can be used as the offset within the struct
		 * using reinterpret_cast.
		 */
		Slot slots[(BufferManager::DATA_SIZE_BYTE - sizeof(SPHeader)) / sizeof(Slot)];
		char data[BufferManager::DATA_SIZE_BYTE - sizeof(SPHeader)];
	};

	static const unsigned int MAX_DATA_SIZE;

	SlottedPage();

	bool hasSpaceAtDataFront(size_t data_size) const;

	bool canMakeEnoughSpace(size_t necessary_space) const;

	size_t spaceBetweenSlotsAndData() const;

	void compactify();

	char *getSlotData(Slot &slot) const;

	void setSlotOffset(Slot &slot, char *data_ptr);

	void init();

	Slot *getExistingSlot(uint16_t slotOffset);

	uint16_t findFirstFreeSlot(uint16_t lastFreeSlot) const;

	bool isLastSlot(Slot &slot) const;

	bool isInRange(uint16_t slotOffset) const;

	Slot *retrieveSlot(uint16_t *slotIndex);

	void writeSlotData(Slot *slot, const char *data, size_t data_size);

	Slot *retrieveSlotIfPossible(uint16_t slotOffset);
};

#endif //PROJECT_SLOTTEDPAGE_H
