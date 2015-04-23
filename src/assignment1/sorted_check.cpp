//
// Created by Martin on 20.04.2015.
//

#include <stdio.h>
#include <stdint.h>
#include <io.h>
#include <bits/stl_algobase.h>
#include "sorted_check.h"
#include "util.h"

const uint64_t MAX_BUFFER_SIZE_BYTE = 1 * 1024 * 1024;

// @param int fd the input file descriptor
// @param uint64_t size the amount of elements in the file
// @return true if the values in the file are sorted
bool check_sorting(int fd, uint64_t size) {
	uint64_t prev = 0;
	uint64_t elements_left = size;

	while (elements_left != 0) {
		uint64_t max_buffer_size = div_ceil(MAX_BUFFER_SIZE_BYTE, sizeof(uint64_t));
		uint64_t elements_left_byte = elements_left * sizeof(uint64_t);
		uint64_t buffer_size = std::min(elements_left_byte, max_buffer_size);
		uint64_t buffer[buffer_size];
		uint64_t bytes_to_read = buffer_size * sizeof(uint64_t);

		int bytes_read = read(fd, buffer, bytes_to_read);
		elements_left -= buffer_size;
		printf("elements_left new: %llu, buffer_size: %llu, bytes_read: %d\n", elements_left, buffer_size, bytes_read);
		if (bytes_read < 0) {
			perror("Read error");
			return false;
		}
		if (bytes_read != bytes_to_read) {
			fprintf(stderr, "Expected %llu bytes to be read, but got %d\n", bytes_to_read, bytes_read);
			throw;
		}

		// validate
		for (unsigned int i(0); i < buffer_size; ++i) {
			uint64_t curr = buffer[i];
			if (curr < prev) {
				fprintf(stderr, "Error at indices %d and %d: %llu is bigger than %llu\n", i - 1, i, prev, curr);
				lseek(fd, 0, SEEK_SET); // reset file offset
				return false;
			}
			prev = curr;
		}
	}
	lseek(fd, 0, SEEK_SET); // reset file offset
	return true;
}
