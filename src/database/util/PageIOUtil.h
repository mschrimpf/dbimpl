//
// Created by Daniel on 30.04.2015.
//

#ifndef PROJECT_FILEIOUTIL_H
#define PROJECT_FILEIOUTIL_H

#include <unordered_map>
#include "../buffer/IPageIO.h"
#include <stdint.h>

struct SegmentInfo {
	int fd;
};

class PageIOUtil : public IPageIO {
private:
	std::unordered_map<uint64_t, SegmentInfo> segmentInfoMap;

	SegmentInfo *getSegmentInfo(uint64_t segmentId);

	void readFd(int fd, long offset_bytes, void *data, unsigned int len);

	void writeFd(int fd, long offset_bytes, void *data, unsigned int len);

public:
	void readPage(uint64_t pageId, uint64_t segmentId, void *data, unsigned len);

	void writePage(uint64_t pageId, uint64_t segmentId, void *data, unsigned len);

	~PageIOUtil();

	SegmentInfo getOrCreateSegmentInfo(uint64_t segmentId);
};


#endif //PROJECT_FILEIOUTIL_H
