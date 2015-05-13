//
// Created by Daniel on 29.04.2015.
//
#include "TwoQList.h"

void TwoQList::push(BufferFrame *frame) {
	if (frame->usedBefore()) {
		//LruSet.insert(frame);
		LruQueue.push_back(frame);
	} else {
		FifoQueue.push_back(frame);
		//FifoSet.insert(frame);
	}
}

BufferFrame *TwoQList::pop() {
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