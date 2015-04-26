//
// Created by Martin on 26.04.2015.
//

#ifndef PROJECT_BUFFERMANAGER_H
#define PROJECT_BUFFERMANAGER_H


class BufferManager {
public:
	BufferManager(unsigned pageCount);

	BufferManager::~BufferManager();

	BufferFrame &fixPage(uint64_t pageId, bool exclusive);

	void unfixPage(BufferFrame &frame, bool isDirty);
};


#endif //PROJECT_BUFFERMANAGER_H
