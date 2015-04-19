//
// Created by Martin on 17.04.2015.
//

#include <stdexcept>
#include <sys/types.h>
#include <io.h>
#include <stdio.h>
#include <string.h>
#include "external_sort.h"

void external_sort(int fdInput, uint64_t size, int fdOutput, uint64_t memSize) {
	// read
	uint64_t input[size];
	ssize_t const r{read(fdInput, input, sizeof(input))};
	if (r != sizeof(input) / sizeof(uint64_t)) {
		fprintf(stderr, "Could not read complete array\n");
		return;
	}
	if (r < 0) {
		perror("Read error");
		return;
	}

	close(fdInput);
	if (errno) {
		fprintf(stderr, "Could not close input file: %d %s\n", errno, strerror(errno));
		return;
	}

	// sort
	size_t length = sizeof(input) / sizeof(uint64_t);
	for (unsigned int i(0); i < length; ++i) {
		printf("%03d: %lld\n", i, input[i]);
	}
}