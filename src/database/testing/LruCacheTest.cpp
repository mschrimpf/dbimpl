//
// Created by Daniel on 27.04.2015.
//

#include <stdio.h>
#include "../util/LruCache.h"

int main(){
    LruCache<std::string, std::string> cache (5);
    cache.put("Test","Test1");
    printf("Cache size %zu\n", cache.size());
    cache.put("Test2", "Test2");
    cache.put("Test3", "Test2");
    cache.put("Test4", "Test2");
    cache.put("Test5", "Test2");
    cache.put("Test6", "Test2");
    printf("Cache size %zu", cache.size());
    printf("Should not contain value: %zu", cache.contains("Test"));
    printf("Should contain value: %zu", cache.contains("Test2"));
}
