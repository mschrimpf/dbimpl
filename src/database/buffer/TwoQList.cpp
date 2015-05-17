//
// Created by Daniel on 29.04.2015.
//
#include "TwoQList.h"

void TwoQList::push(BufferFrame *frame) {
	if(frame->isFlagSet(BufferFrame::IN_FIFO_QUEUE)) {
		FifoQueue.remove(frame);
	} else if(frame->isFlagSet(BufferFrame::IN_LRU_QUEUE)) {
		LruQueue.remove(frame);
	}

	if (frame->usedBefore()) {
		//LruSet.insert(frame);
		LruQueue.push_back(frame);
		frame->setFlag(BufferFrame::IN_LRU_QUEUE);
	} else {
		FifoQueue.push_back(frame);
		//FifoSet.insert(frame);
		frame->setFlag(BufferFrame::IN_FIFO_QUEUE);
	}
}

BufferFrame *TwoQList::pop() {
	BufferFrame *frame = nullptr;
	if (FifoQueue.size() > 0) {
		frame = FifoQueue.front();
		FifoQueue.pop_front();
		frame->unsetFlag(BufferFrame::IN_FIFO_QUEUE);
	} else if (LruQueue.size() > 0) {
		frame = LruQueue.front();
		LruQueue.pop_front();
		frame->unsetFlag(BufferFrame::IN_LRU_QUEUE);
	}
	return frame;
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