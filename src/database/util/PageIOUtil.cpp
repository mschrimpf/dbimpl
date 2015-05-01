//
// Created by Daniel on 30.04.2015.
//


#include <fcntl.h>
#include "PageIOUtil.h"

void PageIOUtil::read(uint64_t pageId, uint64_t segmentId, void *data, unsigned len) {
	PageInfo *pageInfo = this->getPageInfo(pageId);
	if (pageInfo != nullptr) {
		int fd = pageInfo->fd;
		unsigned offset = pageInfo->offset;
		this->readFd(fd, offset, data, len);
	}
}

void PageIOUtil::readFd(int fd, unsigned offset, void *data, unsigned len) {

}

void PageIOUtil::write(uint64_t pageId, uint64_t segmentId, void *data, unsigned len) {
	int fd;
	unsigned offset;

	PageInfo *pageInfo = this->getPageInfo(pageId);
	if (pageInfo != nullptr) {
		char *segmentIdStr = itoa(segmentId);
		fd = open(segmentIdStr, O_CREAT | O_TRUNC | O_RDWR);
		if (this->segmentPagesAmountMap.find(segmentId) != this->segmentPagesAmountMap.end()) {
			offset = this->segmentPagesAmountMap[segmentId];
		} else {
			offset = 0;
		}
		this->pageInfoMap[pageId] = {fd, offset};

		this->segmentPagesAmountMap[segmentId] = offset + 1;
	}

	this->writeFd(fd, offset, data, len);
}

void PageIOUtil::writeFd(int fd, unsigned int offset, void *data, unsigned int len) {

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
