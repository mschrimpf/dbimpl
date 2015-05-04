//
// Created by Daniel on 30.04.2015.
//


#include <fcntl.h>
#include <stdio.h>
#include "PageIOUtil.h"
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <unistd.h>
#include <inttypes.h>

void PageIOUtil::readPage(uint64_t pageId, uint64_t segmentId, void *data, unsigned len) {
	long offset = (long) (pageId * len);
	SegmentInfo segmentInfo = getOrCreateSegmentInfo(segmentId);
	int fd = segmentInfo.fd;
	this->readFd(fd, offset, data, len);
}

void PageIOUtil::readFd(int fd, long offset_bytes, void *data, unsigned len) {
	int read_bytes = pread(fd, data, len, offset_bytes);
	if(read_bytes != len) {
		perror("Could not read data");
	}
}

void PageIOUtil::writePage(uint64_t pageId, uint64_t segmentId, void *data, unsigned len) {
	long offset = (long) (pageId * len);

	SegmentInfo segmentInfo = this->getOrCreateSegmentInfo(segmentId);
	int fd = segmentInfo.fd;

	#if DEBUG == 1
		unsigned int pageValue = reinterpret_cast<unsigned*>(data)[0];
		debug(pageId, "Writing out value: %i", pageValue);
	#endif
	printf("Attempting to write page %" PRId64 " on segment %" PRId64 " | offset = %ld (%" PRId64 " * %u) | fd: %i\n",
			pageId, segmentId, offset, pageId, len, fd);

	this->writeFd(fd, offset, data, len);
}

void PageIOUtil::writeFd(int fd, long offset_bytes, void *data, unsigned int len) {
	int written_bytes = pwrite(fd, data, len, offset_bytes);
	if(written_bytes != len) {
		perror("Could not write data");
	}
}

SegmentInfo *PageIOUtil::getSegmentInfo(uint64_t segmentId) {
	if (this->segmentInfoMap.find(segmentId) != this->segmentInfoMap.end()) {
		SegmentInfo *segmentInfo = &this->segmentInfoMap[segmentId];
		printf("Segment with id %" PRId64 " found on disk\n", segmentId);
		return segmentInfo;
	}
	printf("Segment with id %" PRId64 " not found on disk\n", segmentId);
	return nullptr;
}


SegmentInfo PageIOUtil::getOrCreateSegmentInfo(uint64_t segmentId) {
	SegmentInfo * segmentInfo = this->getSegmentInfo(segmentId);
	if(segmentInfo != nullptr) {
		return *segmentInfo;
	}
	printf("Create file %s\n", std::to_string(segmentId).c_str());
	int fd = open(std::to_string(segmentId).c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	this->segmentInfoMap[segmentId] = {fd};
	return segmentInfoMap[segmentId];
}

PageIOUtil::~PageIOUtil() {
	for (auto fdPair : this->segmentInfoMap) {
		close(fdPair.second.fd);
	}
}

