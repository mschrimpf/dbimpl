#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include "external_sort.h"

bool validate_sorted(int fd, uint64_t size);

using namespace std;

int main(int argc, char *argv[]) {
	if (argc < 4) {
		printf("Invalid number of arguments.\n"
					   "Usage: 'sort <inputFile> <outputFile> <memoryBufferInMB>'");
		return -1;
	}

	// naming
	const char *inputFile = argv[1];
	const char *outputFile = argv[2];
	const char *memoryBufferInput = argv[3];

	// check input correct
	int fdInput = open(inputFile, O_RDONLY);
	if (fdInput < 0) {
		fprintf(stderr, "Error: Cannot open inputFile '%s': %d %s\n", inputFile, errno, strerror(errno));
		return -1;
	}
	int fdOutput = open(outputFile, O_CREAT | O_TRUNC | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
	if (fdOutput < 0) {
		close(fdInput);
		fprintf(stderr, "Error: Cannot open outputFile '%s': %d %s\n", outputFile, errno, strerror(errno));
		return -1;
	}

	int memoryBufferInMBInt = atoi(memoryBufferInput);
	uint64_t memoryBufferInMB = (uint64_t) memoryBufferInMBInt;
	if (errno) {
		close(fdInput);
		close(fdOutput);
		fprintf(stderr, "memoryBufferInMB '%s' cannot not be parsed: %d %s\n", memoryBufferInput, errno,
				strerror(errno));
		return -1;
	}

	// print parameters
	struct stat inputStat;
	fstat(fdInput, &inputStat);
	uint64_t inputFileSize = (uint64_t) inputStat.st_size;

	printf("InputFile: %s (Size: %lld bytes)\n"
				   "OutputFile: %s\n"
				   "MemoryBufferInMB: %lld\n",
		   inputFile, inputFileSize,
		   outputFile, memoryBufferInMB);

	// sort
	external_sort(fdInput, inputFileSize, fdOutput, memoryBufferInMB);

	// validate algorithm
	fdOutput = open(outputFile, O_RDONLY);
	if (!validate_sorted(fdOutput, inputFileSize)) {
		return -1;
	}

	return 0;
}

// Closes the file descriptor.
bool validate_sorted(int fd, uint64_t size) {
	uint64_t input[size];
	int r = read(fd, input, sizeof(input));
	close(fd);
	if (r < 0) {
		perror("Read error");
		return false;
	}

	// validate
	uint64_t prev = (uint64_t) INT64_MIN;
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
