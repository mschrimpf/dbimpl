//
// Created by Martin on 17.04.2015.
//

#include <stdexcept>
#include <io.h>
#include <stdio.h>
#include <algorithm>
#include <fcntl.h>
#include "external_sort.h"
#include <queue>

struct QueueElem{ // elem which is stored in the prio queue
	int chunkNumber; // number of the chunk
	std::vector<uint64_t>::iterator ptr; // iterator of the values of the file
};

struct CompareQueuePrio {
	bool operator()(const QueueElem& lhs, const QueueElem& rhs) const {
		return *lhs.ptr > *rhs.ptr;
	}
};

uint64_t div_ceil(uint64_t divisor, uint64_t dividend);

void external_sort(int fdInput, uint64_t size, int fdOutput, uint64_t memSize /* in MB */) {
	uint64_t chunks = size / memSize; // number of runs the sorting requires
	uint64_t chunk[memSize];
	printf("Chunk phase with %llu chunks\n", chunks);
	// steps 1 - 3
	int fdsTemp[chunks];
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
		// IDEA: could write only to one file (e.g. output file) with offsets
		char iteratorString[21];
		sprintf(iteratorString, "%d", i);
		std::string tempFile = "temp-";
		tempFile = tempFile + iteratorString;
		fdsTemp[i] = open(tempFile.c_str(), O_CREAT | O_TRUNC | O_WRONLY);
		if(fdsTemp[i] < 0) {
			std::string msg = "Cannot open temp file";
			msg = msg + tempFile;
			perror(msg.c_str());
		}
		int w = write(fdsTemp[i], chunk, sizeof(chunk));
		// TODO fehlt hier nicht der check ob die chunk-size auch korrekt ist?
		// kann ja sein dass im letzten chunk nur 1 wert ist
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
	for (unsigned int i(0); i < chunks; i++) {
		int fd = fdsTemp[i];
		read(fd, inputBuffers + i * bufferSize, (int) bufferSize);
	}


	//k-way merge
	// prio-queue
	// iterate through every input-buffer, enqueue (nextNumberIn(f), f)

	std::priority_queue<QueueElem, std::vector<QueueElem>, CompareQueuePrio> queue;
	for (unsigned int i(0); i < chunks; i++){
		//queue fuellen?
		//queue.push({i, fdsTemp[i]});
	}

	while (!queue.empty()){
		QueueElem headElement = queue.top(); // get top elem
		queue.pop(); // remove elem
		int value = *headElement.ptr; // value of elem

		//outputBuffer value hinzufügen
		//checken ob buffer nun voll,
		//wenn ja, dann rausschreiben und leeren


		headElement.ptr++; // next element of this queue elem
		//checken ob das letzte element erreicht wurde

	}

//	int w = write(fdOutput, input, sizeof(input));
//	if (w < 0) {
//		perror("Could not write data");
//		return;
//	}



	close(fdInput);
	close(fdOutput);
}

uint64_t div_ceil(uint64_t divisor, uint64_t dividend) {
	return 1 + ((divisor - 1) / dividend);
}