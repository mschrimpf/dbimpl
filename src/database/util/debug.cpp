#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <inttypes.h>
#include "debug.h"

void debug(const char *fmt, ...) {
    #if DEBUG == 1
    va_list args;
    char buf[1000];
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    printf(buf);
    printf("\n");
    #endif
}

void debug(uint64_t pageId, const char *fmt, ...){
    #if DEBUG == 1
    va_list args;
    char buf[1000];
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    printf("Page[%" PRId64 "]: ", pageId);
    printf(buf);
    printf("\n");
    #endif
}
