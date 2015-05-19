//
// Created by daniel on 5/19/15.
//

#include "BTree.h"

BTree::BTree(SPSegment *pSegment) {
    spSegment = pSegment;
}

uint64_t BTree::size() {
    return 0;
}

template<class T, class CMP>
TID BTree::lookup(T &param, TID tid) {
    return TID(0, 0);
}

template<class T, class CMP>
void BTree::erase(T &param) {

}

template<class T, class CMP>
void BTree::insert(T &param, TID tid) {

}

