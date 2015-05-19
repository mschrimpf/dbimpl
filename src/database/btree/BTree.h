//
// Created by daniel on 5/19/15.
//

#ifndef PROJECT_BTREE_H
#define PROJECT_BTREE_H


#include "../slotted_pages/SPSegment.hpp"

template<class T, class CMP>
class BTree {

private:
SPSegment * spSegment;

public:
    BTree(SPSegment *pSegment);

    void insert(T &param, TID tid);

    uint64_t size();

    TID lookup(T &param, TID tid);

    void erase(T &param);
};


#endif //PROJECT_BTREE_H
