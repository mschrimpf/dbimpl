#ifndef PROJECT_I_PAGE_IO_H
#define PROJECT_I_PAGE_IO_H

#include <stdint.h>

class IPageIO {
public:
	virtual void readPage(uint64_t pageId, uint64_t segmentId, void *data, unsigned int len) = 0;

	virtual void writePage(uint64_t pageId, uint64_t segmentId, void *data, unsigned int len) = 0;
};

#endif //PROJECT_I_PAGE_IO_H
