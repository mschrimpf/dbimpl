//
// Created by Daniel on 29.04.2015.
//

#ifndef PROJECT_TWOQLIST_H
#define PROJECT_TWOQLIST_H


#include "../buffer/BufferFrame.hpp"

class TwoQList {

    std::set<BufferFrame> FifoSet;
    std::set<BufferFrame> LruSet;
    std::list<BufferFrame> FifoQueue;
    std::list<BufferFrame> LruQueue;
public:
    void push(BufferFrame frame);
    BufferFrame pop();
    void onUse(BufferFrame frame);
private:

};


#endif //PROJECT_TWOQLIST_H
