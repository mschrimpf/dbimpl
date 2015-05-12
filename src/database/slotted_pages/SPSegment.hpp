//
// Created by Martin on 12.05.2015.
//

#ifndef PROJECT_SPSEGMENT_H
#define PROJECT_SPSEGMENT_H

#include "TID.hpp"
#include "SlottedPage.hpp"
#include "Record.hpp"

// compactify may not move the slots since that would invalidate the TIDs (page + slot)

// insert and lookup: copy record - otherwise pointer invalid
// lookup: no lock

class SPSegment {
public:
	struct SlotData {
		uint16_t length;
		char *ptr;
	};

private:
	std::unordered_map<uint64_t, SlottedPage *> pageIdSlottedPageMap;

	SPSegment::SlotData lookupData(TID tid);

public:

	TID insert(const Record &record);

	bool remove(TID tid);

	Record lookup(TID tid);

	bool update(TID tid, const Record &record);
};

#endif //PROJECT_SPSEGMENT_H

