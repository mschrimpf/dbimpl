//
// Created by Martin on 05.05.2015.
//

#ifndef PROJECT_SLOTTEDPAGE_H
#define PROJECT_SLOTTEDPAGE_H

#include <stdint.h>
#include <sys/types.h>
#include <unordered_map>
#include "../buffer/BufferManager.hpp"

// TODO: might have to implement redirects
// (initial implementation could be to just throw an exception when the page is full)
/**
 * If we did everything right: sizeof(SlottedPage) = PAGE_SIZE
 */
struct SlottedPage {
	struct SPHeader {
		uint64_t LSN;
		unsigned slotCount;
		char *firstFreeSlot;
		char *dataStart;
		/**
		 * This will always be zero when data is inserted one after another.
		 * However, when data is deleted, space might become free
		 * and fragment our total available space.
		 */
		unsigned fragmentedSpace;
	} header;

	struct Slot {
		uint16_t length;
		uint16_t is_tid : 1;
		uint16_t offset : 15;
	};

	union {
		/*
		 * Variable-sized array -> store at the end.
		 * Since the SlottedPage will be allocated with a variable length,
		 * this variable can be used as the offset within the struct
		 * using reinterpret_cast.
		 */
		Slot slots[(PAGE_SIZE_BYTE - sizeof(SPHeader)) / sizeof(Slot)];
		char data[PAGE_SIZE_BYTE - sizeof(SPHeader)];
	};
};

#endif //PROJECT_SLOTTEDPAGE_H
