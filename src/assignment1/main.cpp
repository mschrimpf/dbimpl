#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <string>
#include "external_sort.h"

bool validate_sorted(int fd);

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
	int fdInput = open(inputFile, O_RDONLY | O_EXCL);
	if (fdInput < 0) {
		fprintf(stderr, "Error: Cannot open inputFile '%s': %d %s\n", inputFile, errno, strerror(errno));
		return -1;
	}
	int fdOutput = open(outputFile, O_RDWR | O_CREAT | O_TRUNC);
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
		fprintf(stderr, "memoryBufferInMB '%s' cannot not be parsed: %d %s\n", memoryBufferInput, errno, strerror(errno));
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
	if (!validate_sorted(fdOutput)) {
		return -1;
	}

	close(fdOutput);

	return 0;
}

bool validate_sorted(int fd) {


	return true;
}
