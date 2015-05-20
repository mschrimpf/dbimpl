//
// Created by Martin on 12.05.2015.
//

#ifndef PROJECT_TID_HPP
#define PROJECT_TID_HPP

#include <stdint.h>

struct TID {
	TID(uint16_t slotOffset, uint64_t pageId) : slotOffset(slotOffset), pageId(pageId) { }
	TID(uint64_t id) : slotOffset((uint16_t) id >> 48), pageId(id & 0x0000ffffffffffff){}

	uint16_t slotOffset;
	uint64_t pageId : 48 /* use only 48 bit */;
};

inline bool operator==(const TID t1, const TID t2) {
	return t1.pageId == t2.pageId
		   && t1.slotOffset == t2.slotOffset;
}

#endif //PROJECT_TID_HPP
