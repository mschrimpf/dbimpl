//
// Created by Daniel on 27.04.2015.
//

#include <stdio.h>
#include "../util/LruCache.h"

int main(){
    LruCache<std::string, std::string> cache (5);
    cache.put("Test","Test1");
    printf("Cache size %l", cache.size());
}
