#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include "external_sort.h"
#include "sorted_check.h"

uint64_t file_size(int fd);

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
	int fdInput = open(inputFile, O_RDONLY | O_BINARY);
	if (fdInput < 0) {
		fprintf(stderr, "Error: Cannot open inputFile '%s': %d %s\n", inputFile, errno, strerror(errno));
		return -1;
	}
	int fdOutput = open(outputFile, O_CREAT | O_TRUNC | O_WRONLY | O_APPEND | O_BINARY, S_IRUSR | S_IWUSR);
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
	uint64_t inputFileSize = file_size(fdInput);
	uint64_t numberOfValues = inputFileSize / sizeof(uint64_t);

	printf("InputFile: %s (Size: %lld bytes)\n"
				   "NumberOfValues: %lld\n"
				   "OutputFile: %s\n"
				   "MemoryBufferInMB: %lld\n",
		   inputFile, inputFileSize,
		   numberOfValues,
		   outputFile, memoryBufferInMB);

	// sort
	lseek(fdInput, 0, SEEK_SET);
	external_sort(fdInput, numberOfValues, fdOutput, memoryBufferInMB);

	// validate algorithm
	fdOutput = open(outputFile, O_RDONLY | O_BINARY);
	if (fdOutput < 0){
		perror("Cannot open temp file");
	}
	uint64_t outputFileSize = file_size(fdOutput);
	if (outputFileSize != inputFileSize) {
		fprintf(stderr, "Output file size %llu does not match input file size %llu\n", outputFileSize, inputFileSize);
//		return -1;
	}
	if (!check_sorting(fdOutput, numberOfValues)) {
		return -1;
	} else {
		printf("sorted.");
	}

	return 0;
}

uint64_t file_size(int fd) {
	struct stat inputStat;
	fstat(fd, &inputStat);
	uint64_t inputFileSize = (uint64_t) inputStat.st_size;
	return inputFileSize;
}
