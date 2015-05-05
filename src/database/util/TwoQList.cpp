//
// Created by Daniel on 29.04.2015.
//
#include "TwoQList.h"

void TwoQList::push(BufferFrame *frame) {
	if (frame->usedBefore()) {
		/* was used before -> put it into lru queue */
		LruQueue.push_back(frame);
	} else {
		/* inserted first time*/
		FifoQueue.push_back(frame);
	}
}

BufferFrame *TwoQList::pop() {
	if (FifoQueue.size() > 0) {
		BufferFrame *front = FifoQueue.front();
		FifoQueue.pop_front();
		return front;
	} else if (LruQueue.size() > 0) {
		BufferFrame *front = LruQueue.front();
		LruQueue.pop_front();
		return front;
	} else {
		//no frame in List
		return nullptr;
	}
}

void TwoQList::remove(BufferFrame *frame) {
//	if (frame->usedBefore()) {
		LruQueue.remove(frame);
//	} else {
		FifoQueue.remove(frame);
//	}
}