//
// Created by Daniel on 29.04.2015.
//
#include <inttypes.h>
#include "TwoQList.h"
#include "../util/debug.h"

void TwoQList::push(BufferFrame *frame) {
	debug(">> Pushing page %" PRId64 " on segment %" PRId64, frame->getPageId(), frame->getSegmentId());
	for (auto it = FifoQueue.begin(); it != FifoQueue.end(); it++)
		debug("[FifoQueue] page %" PRId64 " on segment %" PRId64, (*it)->getPageId(), (*it)->getSegmentId());
	for (auto it = LruQueue.begin(); it != LruQueue.end(); it++)
		debug("[LruQueue] page %" PRId64 " on segment %" PRId64, (*it)->getPageId(), (*it)->getSegmentId());

	if (frame->usedBefore()) {
		//LruSet.insert(frame);
		LruQueue.push_back(frame);
	} else {
		FifoQueue.push_back(frame);
		//FifoSet.insert(frame);
	}
}

BufferFrame *TwoQList::pop() {
	debug(">> Popping");
	for (auto it = FifoQueue.begin(); it != FifoQueue.end(); it++)
		debug("[FifoQueue] page %" PRId64 " on segment %" PRId64, (*it)->getPageId(), (*it)->getSegmentId());
	for (auto it = LruQueue.begin(); it != LruQueue.end(); it++)
		debug("[LruQueue] page %" PRId64 " on segment %" PRId64, (*it)->getPageId(), (*it)->getSegmentId());

	if (FifoQueue.size() > 0) {
		BufferFrame *front = FifoQueue.front();
		FifoQueue.pop_front();
		//FifoSet.erase(front);
		return front;
	} else if (LruQueue.size() > 0) {
		BufferFrame *front = LruQueue.front();
		LruQueue.pop_front();
		//LruSet.erase(front);
		return front;
	} else {
		//no frame in List
		return nullptr;
	}
}

void TwoQList::remove(BufferFrame *frame) {
	debug(">> Remove page %" PRId64 " on segment %" PRId64, frame->getPageId(), frame->getSegmentId());
	for (auto it = FifoQueue.begin(); it != FifoQueue.end(); it++)
		debug("[FifoQueue] page %" PRId64 " on segment %" PRId64, (*it)->getPageId(), (*it)->getSegmentId());
	for (auto it = LruQueue.begin(); it != LruQueue.end(); it++)
		debug("[LruQueue] page %" PRId64 " on segment %" PRId64, (*it)->getPageId(), (*it)->getSegmentId());
	//ADDED
	if (frame->usedBefore()) {
		LruQueue.remove(frame);
	} else {
		FifoQueue.remove(frame);
	}
	//

	//if (FifoSet.find(frame) != FifoSet.end()){
	/* Elem in Fifo-Set -> Remove and add to LRU */
	//FifoSet.erase(frame);
	//FifoQueue.remove(frame);
	//}else if (LruSet.find(frame) != LruSet.end()) {
	/* Elem in LRU -> remove and add at the end */
	//LruQueue.remove(frame);
	//}
}