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
private:
	BufferManager &bufferManager;
	/** Page count or max page (equivalent) */
	uint64_t pageCount;

public:
	struct SlotData {
		uint16_t length;
		char *ptr;
	};
private:
	SPSegment::SlotData lookupData(TID tid);

public:

	void SPSegment(BufferManager &bufferManager);

	TID insert(const Record &record);

	bool remove(TID tid);

	Record lookup(TID tid);

	bool update(TID tid, const Record &record);

	BufferFrame &findOrCreatePage(size_t data_size);
};

#endif //PROJECT_SPSEGMENT_H

