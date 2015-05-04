//
// Created by Martin on 04.05.2015.
//

#ifndef PROJECT_DEBUG_H
#define PROJECT_DEBUG_H

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

#define DEBUG 1

void debug(const char *info) {
#if DEBUG == 1
	printf("%s\n", info);
#endif
}

void debug(uint64_t pageId, const char *fmt, ...) {
#if DEBUG == 1
	va_list args;
	char buf[1000];
	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);
	printf("Frame[%llu]:", pageId);
	printf(buf);
	printf("\n");
#endif
}

#endif //PROJECT_DEBUG_H
