//
// Created by Martin on 05.05.2015.
//

#ifndef PROJECT_SLOT_H
#define PROJECT_SLOT_H

#include <stdint.h>
#include <sys/types.h>
#include "TID.hpp"

/**
 * 8-byte control structure.
 * According to the slides: T | S | O | O | O | L | L | L
 * Actual implementation: T | O | O | O | L | L | L | O due to alignment issues
 */
struct Slot {
	static const uint8_t T_NO_TID_VALUE = (uint8_t) 0x11111111b;

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

	void redirectTo(TID redirectTid);
};

#endif //PROJECT_SLOT_H
