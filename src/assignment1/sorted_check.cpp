//
// Created by Martin on 20.04.2015.
//

#include <stdio.h>
#include <stdint.h>
#include <io.h>
#include "sorted_check.h"


// Closes the file descriptor.
bool check_sorting(int fd, size_t size) {
	uint64_t input[size];
	int r = read(fd, input, sizeof(input));
	close(fd);
	if (r < 0) {
		perror("Read error");
		return false;
	}

	// validate
	uint64_t prev = 0;
	size_t length = sizeof(input) / sizeof(uint64_t);
	for (unsigned int i(0); i < length; ++i) {
		uint64_t curr = input[i];
		if (curr < prev) {
			fprintf(stderr, "Error at indices %d and %d: %llu is bigger than %llu\n", i - 1, i, prev, curr);
			return false;
		}
	}
	return true;
}
