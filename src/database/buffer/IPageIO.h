#ifndef PROJECT_I_PAGE_IO_H
#define PROJECT_I_PAGE_IO_H

#include <stdint.h>

class IPageIO {
public:
	virtual void read(int pageId, int segmentId, void * data, int len);
	virtual void write(int pageId, int segmentId, void * data);
};

#endif //PROJECT_I_PAGE_IO_H
