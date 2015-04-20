//
// Created by Martin on 17.04.2015.
//

#include <stdexcept>
#include <io.h>
#include <stdio.h>
#include <algorithm>
#include "external_sort.h"

uint64_t div_ceil(uint64_t divisor, uint64_t dividend);

void external_sort(int fdInput, uint64_t size, int fdOutput, uint64_t memSize /* in MB */) {
	uint64_t chunks = size / memSize;
	uint64_t chunk[memSize];
	printf("Chunk phase with %llu chunks\n", chunks);
	// steps 1 - 3
	for (unsigned int i(0); i < chunks; i++) {
		// read chunk
		int r = read(fdInput, chunk, sizeof(chunk));
		if (r < 0) {
			perror("Read error");
			return;
		}
		// sort chunk
		std::sort(std::begin(chunk), std::end(chunk));
		// write chunk to output file
		int w = write(fdOutput, chunk, sizeof(chunk));
		if (w < 0) {
			perror("Could not write chunk");
			return;
		}
	}

	// allocate output buffer
	printf("Merge phase (%llu-way)\n", chunks);
	uint64_t bufferSize = div_ceil(memSize, chunks + 1);
	uint64_t inputBuffers[bufferSize * chunks];
	uint64_t outputBuffer[bufferSize];

	// read data in buffer


	// read
	uint64_t input[size];
	int r = read(fdInput, input, sizeof(input));
	if (r < 0) {
		perror("Read error");
		return;
	}

	// sort
	size_t length = sizeof(input) / sizeof(uint64_t);
	for (unsigned int i(0); i < length; ++i) {
		uint64_t data = input[i];
	}
	int w = write(fdOutput, input, sizeof(input));
	if (w < 0) {
		perror("Could not write data");
		return;
	}


	close(fdInput);
	close(fdOutput);
}

uint64_t div_ceil(uint64_t divisor, uint64_t dividend) {
	return 1 + ((divisor - 1) / dividend);
}