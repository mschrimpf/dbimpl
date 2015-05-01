//
// Created by Daniel on 29.04.2015.
//

#include <bits/stl_list.h>
#include <streambuf>
#include <bits/stl_set.h>
#include "TwoQList.h"

void TwoQList::push(BufferFrame* frame) {
    FifoQueue.push_back(frame);
    FifoSet.insert(frame);
}

BufferFrame* TwoQList::pop() {
    if (FifoQueue.size() > 0){
        BufferFrame* front = FifoQueue.front();
        FifoQueue.pop_front();
        FifoSet.erase(front);
        return front;
    }else if (LruQueue.size() > 0){
        BufferFrame* front = LruQueue.front();
        LruQueue.pop_front();
        LruSet.erase(front);
        return front;
    }else{
        //no frame in List
        return nullptr;
    }
}

void TwoQList::onUse(BufferFrame* frame){
    if (FifoSet.find(frame) != FifoSet.end()){
        /* Elem in Fifo-Set -> Remove and add to LRU */
        FifoSet.erase(frame);
        FifoQueue.remove(frame);
        LruQueue.push_back(frame);
        LruSet.insert(frame);
    }else if (LruSet.find(frame) != LruSet.end()) {
        /* Elem in LRU -> remove and add at the end */
        LruQueue.remove(frame);
        LruQueue.push_back(frame);
    }else{
        /* Elem neither in Fifo nor in LRU -> Add to Fifo*/
        FifoSet.insert(frame);
        FifoQueue.push_back(frame);
    }
}