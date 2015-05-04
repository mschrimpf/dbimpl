//
// Created by Daniel on 30.04.2015.
//


#include <fcntl.h>
#include <stdio.h>
#include "PageIOUtil.h"
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <unistd.h>
#include <inttypes.h>

void PageIOUtil::readPage(uint64_t pageId, uint64_t segmentId, void *data, unsigned len) {
	PageInfo *pageInfo = this->getPageInfo(pageId);
	if (pageInfo != nullptr) {
		int fd = pageInfo->fd;
		long offset = pageInfo->offset_bytes;
		this->readFd(fd, offset, data, len);
	}
	// else nothing to read
}

void PageIOUtil::readFd(int fd, long offset_bytes, void *data, unsigned len) {
	if(0 >= pread(fd, data, len, offset_bytes)) {
		perror("Could not read data");
	}
}

void PageIOUtil::writePage(uint64_t pageId, uint64_t segmentId, void *data, unsigned len) {
	int fd;
	long offset;

	printf("Writing page %" PRId64 " | segment %" PRId64 "\n", pageId, segmentId);
	PageInfo *pageInfo = this->getPageInfo(pageId);
	if (pageInfo == nullptr) {
		SegmentInfo * segmentInfo = this->getSegmentInfo(segmentId);
		if(segmentInfo == nullptr) {
			printf("Create file %s\n", std::to_string(segmentId).c_str());
			fd = open(std::to_string(segmentId).c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
			offset = 0;
			this->segmentInfoMap[segmentId] = {fd, 0};
		} else {
			fd = this->segmentInfoMap[segmentId].fd;
			offset = this->segmentInfoMap[segmentId].total_offset_bytes;
		}
		this->pageInfoMap[pageId] = {fd, offset};
		this->segmentInfoMap[segmentId].total_offset_bytes = offset + len;
	} else {
		fd = pageInfo->fd;
		offset = pageInfo->offset_bytes;
	}

	this->writeFd(fd, offset, data, len);
}

void PageIOUtil::writeFd(int fd, long offset_bytes, void *data, unsigned int len) {
	if(0 >= pwrite(fd, data, len, offset_bytes)) {
		perror("Could not write data");
	}
}

PageInfo *PageIOUtil::getPageInfo(uint64_t pageId) {
	if (this->pageInfoMap.find(pageId) != this->pageInfoMap.end()) {
		PageInfo *pageInfo = &this->pageInfoMap[pageId];
		printf("Page with id %" PRId64 " found on disk\n", pageId);
		return pageInfo;
	}
	printf("Page with id %" PRId64 " not found on disk\n", pageId);
	return nullptr;
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

PageIOUtil::~PageIOUtil() {
	for (auto fdPair : this->pageInfoMap) {
		close(fdPair.second.fd);
	}
}

