//
// Created by Daniel on 30.04.2015.
//

#ifndef PROJECT_FILEIOUTIL_H
#define PROJECT_FILEIOUTIL_H

#include <unordered_map>
#include "../buffer/IPageIO.h"

struct PageInfo {
	int fd;
	unsigned offset;
};

class PageIOUtil : public IPageIO {
private:
	std::unordered_map<uint64_t, PageInfo> pageInfoMap;
	std::unordered_map<uint64_t, unsigned> segmentPagesAmountMap;

	PageInfo *getPageInfo(uint64_t pageId);

	void readFd(int fd, unsigned int offset, void *data, unsigned int len);

public:

	void read(uint64_t pageId, uint64_t segmentId, void *data, unsigned len);

	void write(uint64_t pageId, uint64_t segmentId, void *data, unsigned len);

	~PageIOUtil();

	PageIOUtil();

	void writeFd(int fd, unsigned int offset, void *data, unsigned int len);
};


#endif //PROJECT_FILEIOUTIL_H
