//
// Created by Daniel on 30.04.2015.
//

#include <fcntl.h>
#include <stdio.h>
#include "PageIOUtil.h"
#include "../util/debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <unistd.h>
#include <inttypes.h>

#ifdef _WIN32
#define PRId64 "llu"
#else
#include <inttypes.h>
#endif

void PageIOUtil::readPage(uint64_t pageId, uint64_t segmentId, void *data, unsigned len) {
	int fd;
	long offset = pageId * len;
	SegmentInfo * segmentInfo = this->getSegmentInfo(segmentId);
	if(segmentInfo == nullptr) {
		debug("Create file %s\n", std::to_string(segmentId).c_str());
		fd = open(std::to_string(segmentId).c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
		this->segmentInfoMap[segmentId] = {fd};
	} else {
		fd = this->segmentInfoMap[segmentId].fd;
	}
	this->readFd(fd, offset, data, len);
}

void PageIOUtil::readFd(int fd, long offset_bytes, void *data, unsigned len) {
	size_t read_bytes = 0;
	while (read_bytes < len){
		read_bytes += pread(fd, (uint8_t*)data + read_bytes, len - read_bytes, offset_bytes + read_bytes);
	}
	if(read_bytes > len) {
		perror("Too many bytes were read");
	}else{
		debug("Read %d bytes\n", read_bytes);
	}
}

void PageIOUtil::writePage(uint64_t pageId, uint64_t segmentId, void *data, unsigned len) {
	int fd;
	long offset = pageId * len;

	debug("Attempting to write page %" PRId64 " on segment %" PRId64 "\n", pageId, segmentId);
	SegmentInfo * segmentInfo = this->getSegmentInfo(segmentId);
	if(segmentInfo == nullptr) {
		debug("Create/Open file %s\n", std::to_string(segmentId).c_str());
		fd = open(std::to_string(segmentId).c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
		this->segmentInfoMap[segmentId] = {fd};
	} else {
		fd = this->segmentInfoMap[segmentId].fd;
	}

	#if DEBUG == 1
		unsigned int pageValue = reinterpret_cast<unsigned*>(data)[0];
		debug(pageId, "Writing out value: %i", pageValue);
	#endif

	this->writeFd(fd, offset, data, len);
}

void PageIOUtil::writeFd(int fd, long offset_bytes, void *data, unsigned int len) {
	size_t written_bytes = 0;
	while (written_bytes < len){
		written_bytes += pwrite(fd, (uint8_t*)data + written_bytes, len - written_bytes, offset_bytes + written_bytes);
	}
	if(written_bytes > len) {
		perror("Too many bytes were written!");
	} else {
		debug("Wrote %d bytes\n", written_bytes);
	}
}


SegmentInfo *PageIOUtil::getSegmentInfo(uint64_t segmentId) {
	if (this->segmentInfoMap.find(segmentId) != this->segmentInfoMap.end()) {
		SegmentInfo *segmentInfo = &this->segmentInfoMap[segmentId];
		debug("Segment with id %" PRId64 " found on disk\n", segmentId);
		return segmentInfo;
	}
	debug("Segment with id %" PRId64 " not found on disk\n", segmentId);
	return nullptr;
}

PageIOUtil::~PageIOUtil() {
	for (auto fdPair : this->segmentInfoMap) {
		close(fdPair.second.fd);
	}
}

