//
// Created by Martin on 04.05.2015.
//

#ifndef PROJECT_DEBUG_H
#define PROJECT_DEBUG_H

#include <stdint.h>

#define DEBUG 1

void debug(const char *info, ...);

void debug(uint64_t pageId, const char *fmt, ...);

#endif //PROJECT_DEBUG_H
