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

public:
    LruCache(size_t maxEntries);
    ~LruCache();
    void put(Key key, Value value);
    Value get(Key key);
    uint64_t size();

private:
    typedef std::unordered_map<Key, Value> CacheMap;
    CacheMap cacheMap;
    typedef std::pair<Key , Value> Pair;
    typedef std::list<Pair> CacheList;
    CacheList cacheList;
    uint64_t entries = 0;
    size_t maxEntries;
};


#endif //PROJECT_LRUCACHE_H
