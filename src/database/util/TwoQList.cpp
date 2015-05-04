//
// Created by Daniel on 29.04.2015.
//
#include "TwoQList.h"

void TwoQList::push(BufferFrame *frame) {
	lock.lock();
	if (frame->usedBefore()) {
		/* was used before -> put it into lru queue */
		LruQueue.push_back(frame);
	} else {
		/* inserted first time*/
		FifoQueue.push_back(frame);
	}
	lock.unlock();
}

BufferFrame *TwoQList::pop() {
	lock.lock();
	if (FifoQueue.size() > 0) {
		BufferFrame *front = FifoQueue.front();
		FifoQueue.pop_front();
		lock.unlock();
		return front;
	} else if (LruQueue.size() > 0) {
		BufferFrame *front = LruQueue.front();
		LruQueue.pop_front();
		lock.unlock();
		return front;
	} else {
		//no frame in List
		lock.unlock();
		return nullptr;
	}
}

void TwoQList::remove(BufferFrame *frame) {
	lock.lock();
	if (frame->isUsed()) {
		LruQueue.remove(frame);
	} else {
		FifoQueue.remove(frame);
	}
	lock.unlock();
}