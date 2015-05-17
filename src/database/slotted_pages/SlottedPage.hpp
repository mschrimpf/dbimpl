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

	/**
	 * 8-byte control structure.
	 * According to the slides: T | S | O | O | O | L | L | L
	 * Actual implementation: T | O | O | O | L | L | L | O due to alignment issues
	 */
	struct Slot {
		uint32_t T : 8;
		/** Memory address */
		uint32_t O : 24;
		uint32_t L : 24;
		uint32_t S : 8;

		bool isTid();

		TID getTid();

		bool wasRedirect();

		void nullTS();

		void markAsFree();

		bool isFree();

		bool isEmptyData();
	};

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

	SlottedPage();

	uint16_t createAndWriteSlot(const char *data, size_t data_size);

	bool hasSpaceAtDataFront(size_t data_size);

	bool canMakeEnoughSpace(size_t necessary_space);

	size_t spaceBetweenSlotsAndData() const;

	void compactify(char *pageEndPtr);

	char *getSlotData(Slot &slot);

	void setSlotOffset(SlottedPage::Slot &slot, char *data_ptr);

	void init();

	Slot *getExistingSlot(uint16_t slotOffset);

	uint16_t findFirstFreeSlot(uint16_t lastFreeSlot);

	bool isLastSlot(SlottedPage::Slot &slot);

	bool isInRange(uint16_t slotOffset);
};

#endif //PROJECT_SLOTTEDPAGE_H
