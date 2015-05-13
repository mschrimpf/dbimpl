//
// Created by Martin on 12.05.2015.
//

#ifndef PROJECT_TID_HPP
#define PROJECT_TID_HPP

#include <stdint.h>

struct TID {
	TID(uint16_t slotOffset, uint64_t pageId) : slotOffset(slotOffset), pageId(pageId) { }

	uint16_t slotOffset;
	uint64_t pageId : 48 /* use only 48 bit */;
};

#endif //PROJECT_TID_HPP
