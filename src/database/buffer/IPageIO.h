#ifndef PROJECT_I_PAGE_IO_H
#define PROJECT_I_PAGE_IO_H

#include <stdint.h>

class IPageIO {
public:
	virtual void read(uint64_t pageId, uint64_t segmentId, void *data, uint64_t len);

	virtual void write(uint64_t pageId, uint64_t segmentId, void *data, uint64_t len);
};

#endif //PROJECT_I_PAGE_IO_H
