//
// Created by Daniel on 27.04.2015.
//

#include "LruCache.h"

template<typename Key, typename Value>
Value LruCache<Key, Value>::get(Key key) {
    auto index = cacheMap.find(key);
    if (index == cacheMap.end()) {
        /* elem not found */
        throw std::range_error("elem not found");
    }
    /* put elem to the beginning as it was "used" recently */
    cacheList.splice(cacheList.begin(), cacheList, index->second);
    return index->second->second;
}

template<typename Key, typename Value>
LruCache<Key, Value>::LruCache(size_t size) {
    maxEntries = size;
}


template<typename Key, typename Value>
LruCache<Key, Value>::~LruCache() {
    /* clean up */
}

template<typename Key, typename Value>
void LruCache<Key, Value>::put(Key key, Value value) {

    if (cacheMap.find(key) == cacheMap.end()){
        /* key not found */
        cacheList.push_front(std::make_pair(key, value));
        cacheMap[key] = cacheList.begin()->second;
        entries++;
    }else{
        /* key already added */
        /* push elem to top of the list */
    }

    if (entries > maxEntries) {
        cacheMap.erase(cacheList.back().first);
        cacheList.pop_back();
        entries--;
    }
}

template<typename Key, typename Value>
size_t LruCache<Key, Value>::size() {
    return entries;
}

template<typename Key, typename Value>
bool LruCache<Key, Value>::contains(Key key) {
    return cacheMap.find(key) != cacheMap.end();
}

//TODO update when get called

