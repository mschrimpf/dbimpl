//
// Created by Martin on 20.04.2015.
//

#include <stdio.h>
#include <stdint.h>
#include <io.h>
#include "sorted_check.h"


// @param int fd the input file descriptor
// @param uint64_t size the amount of elements in the file
// Note that this method closes the file descriptor.
// @return true if the values in the file are sorted
bool check_sorting(int fd, uint64_t size) {
	uint64_t input[size];
	int bytes_read = read(fd, input, size * sizeof(uint64_t));
	close(fd);
	if (bytes_read < 0) {
		perror("Read error");
		return false;
	}

	// validate
	uint64_t prev = 0;
	for (unsigned int i(0); i < size; ++i) {
		uint64_t curr = input[i];
		if (curr < prev) {
			fprintf(stderr, "Error at indices %d and %d: %llu is bigger than %llu\n", i - 1, i, prev, curr);
			return false;
		}
		prev = curr;
	}
	return true;
}
