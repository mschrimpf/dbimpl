//
// Created by Daniel on 30.04.2015.
//

#ifndef PROJECT_FILEIOUTIL_H
#define PROJECT_FILEIOUTIL_H

#include <unordered_map>
#include "../buffer/IPageIO.h"
#include <stdint.h>

struct PageInfo {
	int fd;
	long offset_bytes;
};

class PageIOUtil : public IPageIO {
private:
	std::unordered_map<uint64_t, PageInfo> pageInfoMap;
	std::unordered_map<uint64_t, unsigned> segmentPagesAmountMap;

	PageInfo *getPageInfo(uint64_t pageId);

	void readFd(int fd, long offset_bytes, void *data, unsigned int len);

	void writeFd(int fd, long offset_bytes, void *data, unsigned int len);

public:
	void readPage(uint64_t pageId, uint64_t segmentId, void *data, unsigned len);

	void writePage(uint64_t pageId, uint64_t segmentId, void *data, unsigned len);

	~PageIOUtil();

	PageIOUtil();
};


#endif //PROJECT_FILEIOUTIL_H
