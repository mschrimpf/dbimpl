//
// Created by Daniel on 30.04.2015.
//


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "PageIOUtil.h"

void PageIOUtil::readPage(uint64_t pageId, uint64_t segmentId, void *data, unsigned len) {
	PageInfo *pageInfo = this->getPageInfo(pageId);
	if (pageInfo != nullptr) {
		int fd = pageInfo->fd;
		long offset = pageInfo->offset_bytes;
		this->readFd(fd, offset, data, len);
	}
}

void PageIOUtil::readFd(int fd, long offset_bytes, void *data, unsigned len) {
	lseek(fd, offset_bytes, SEEK_SET);
	read(fd, data, len);
}

void PageIOUtil::writePage(uint64_t pageId, uint64_t segmentId, void *data, unsigned len) {
	int fd;
	long offset;

	PageInfo *pageInfo = this->getPageInfo(pageId);
	if (pageInfo != nullptr) {
		char segmentIdStr[33];
		itoa(segmentId, segmentIdStr, 10);
		fd = open(segmentIdStr, O_CREAT | O_TRUNC | O_RDWR);
		if (this->segmentPagesAmountMap.find(segmentId) != this->segmentPagesAmountMap.end()) {
			offset = this->segmentPagesAmountMap[segmentId];
		} else {
			offset = 0;
		}
		this->pageInfoMap[pageId] = {fd, offset};

		this->segmentPagesAmountMap[segmentId] = offset + len;
	}

	this->writeFd(fd, offset, data, len);
}

void PageIOUtil::writeFd(int fd, long offset_bytes, void *data, unsigned int len) {
	lseek(fd, offset_bytes, SEEK_SET);
	write(fd, data, len);
}

PageInfo *PageIOUtil::getPageInfo(uint64_t pageId) {
	if (this->pageInfoMap.find(pageId) != this->pageInfoMap.end()) {
		PageInfo *pageInfo = &this->pageInfoMap[pageId];
		return pageInfo;
	}
	return nullptr;
}

PageIOUtil::PageIOUtil() {

}

PageIOUtil::~PageIOUtil() {
	for (auto fdPair : this->pageInfoMap) {
		close(fdPair.second);
	}
}
