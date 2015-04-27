//
// Created by Daniel on 27.04.2015.
//

#ifndef PROJECT_LRUCACHE_H
#define PROJECT_LRUCACHE_H


#include <list>
#include <unordered_map>
#include "../buffer/BufferFrame.hpp"

/* Idea from http://stackoverflow.com/a/2057464/1484047
 * and https://github.com/lamerman/cpp-lru-cache/blob/master/include/lrucache.hpp */
template<typename Key, typename Value>
class LruCache {
    typedef std::pair<Key , Value> Pair;
    typedef std::list<Pair> CacheList;
    typedef std::unordered_map<Key, Value> CacheMap;

public:
    CacheList cacheList;
    CacheMap cacheMap;
    uint64_t entries = 0;
    size_t maxEntries;

    LruCache(size_t maxEntries);
    ~LruCache();
    void put(Key key, Value value);
    Value get(Key key);
    size_t size();
    bool contains(Key key);
};

#include "../util/LruCache.cpp"

#endif //PROJECT_LRUCACHE_H
