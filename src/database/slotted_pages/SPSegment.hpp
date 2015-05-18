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
	uint64_t segmentId;
	/** Page count or max page (equivalent) */
	uint64_t maxPageId;

public:
	SPSegment(BufferManager &bufferManager, uint64_t segmentId)
			: bufferManager(bufferManager), segmentId(segmentId) { };

	TID insert(const Record &record);

	bool remove(TID tid);

	Record lookup(TID tid);

	bool update(TID tid, const Record &record);

	BufferFrame &findOrCreatePage(size_t data_size);

	SlottedPage * toSlottedPage(BufferFrame &frame) const;

	void insertAtLocation(TID tid, const Record &record);
};

#endif //PROJECT_SPSEGMENT_H

