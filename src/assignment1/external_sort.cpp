//
// Created by Martin on 17.04.2015.
//

#include <stdexcept>
#include <io.h>
#include <stdio.h>
#include <algorithm>
#include <fcntl.h>
#include <sys/stat.h>
#include "external_sort.h"
#include "sorted_check.h"
#include <queue>

#define DEBUG 1

struct QueueElem {
	unsigned int chunkNumber;
	std::string fileName;
	uint64_t number_of_elements;
	std::vector<uint64_t>::iterator ptr;
	unsigned int index = 0;
	int fd;
};

struct CompareQueuePrio {
	bool operator()(const QueueElem& lhs, const QueueElem& rhs) const {
		return *lhs.ptr > *rhs.ptr;
	}
};

uint64_t div_ceil(uint64_t divisor, uint64_t dividend);


void external_sort(int fdInput, uint64_t size, int fdOutput, uint64_t memSize) { //TODO sicher in mb? glaube nicht
	uint64_t chunks = div_ceil(size * sizeof(uint64_t),  memSize); // number of chunks the sorting requires
	uint64_t chunk_size = (memSize / (chunks + 1)) / sizeof(uint64_t);

	std::vector<uint64_t> read_buffer;

	std::vector<QueueElem> elements;

	printf("Chunk phase with %llu chunks\n", chunks);
	// steps 1 - 3
	std::string tempFilePrefix = "temp-";

	uint64_t consumed_elements = 0;

	for (unsigned int i(0); i < chunks; i++) {
		int fdsTemp;
		std::string fdsTempName;
		// check how many elements we want to read as the last element might have less elements than the previous onse
		uint64_t elements_to_consume = std::min(memSize / sizeof(uint64_t), size - consumed_elements);
		consumed_elements += elements_to_consume;
		read_buffer.resize(elements_to_consume);

		// read chunk
		int r = read(fdInput, &read_buffer[0], elements_to_consume * sizeof(uint64_t));
		if (r < 0) {
			perror("Cannot read input");
			return;
		}
		// sort chunk
		std::sort(read_buffer.begin(), read_buffer.end());

		// write chunk to output file
		char iteratorString[21]; // every 64 bit integer will fit in here
		sprintf(iteratorString, "%d", i);
		fdsTempName = tempFilePrefix + iteratorString;
		fdsTemp = open(fdsTempName.c_str(), O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
		if (fdsTemp < 0) {
			std::string msg = "Cannot open temp file ";
			msg = msg + fdsTempName[i];
			perror(msg.c_str());
		}
		int w = write(fdsTemp, read_buffer.data(), read_buffer.size() * sizeof(uint64_t));

		if (w < 0) {
			perror("Cannot write chunk");
			return;
		}
#ifdef DEBUG
		printf("Check sorting of temporary file chunk %d...", i);
		if (check_sorting(fdsTemp, chunk_size)) {
			printf(" OK.\n");
		} else {
			printf(" ERROR!\n");
		}
#endif
		elements.push_back({i, fdsTempName, read_buffer.size()});
		read_buffer.clear();
		close(fdsTemp);
	}

	// allocate output buffer
	printf("Merge phase (%llu-way)\n", chunks);

	std::vector<std::vector<uint64_t>> input_buffer;
	std::vector<uint64_t> output_buffer;

	// read data in buffer
	for (unsigned int i(0); i < chunks; i++) {
		int fd = open((elements[i].fileName.c_str()), O_RDONLY);
		elements[i].fd = fd;
		std::vector<uint64_t> chunk;
		uint64_t  elemsToAdd = std::min(chunk_size, elements[i].number_of_elements);
		chunk.resize(elemsToAdd);
		elements[i].number_of_elements -= elemsToAdd;
		read(fd, &chunk[0], elemsToAdd * sizeof(uint64_t));
		elements[i].ptr = chunk.begin();
		input_buffer.push_back(chunk);
	}

	// k-way merge
	// prio-queue
	// iterate through every input-buffer, enqueue (nextNumberIn(f), f)

	std::priority_queue<QueueElem, std::vector<QueueElem>, CompareQueuePrio> queue;
	for (unsigned int i(0); i < input_buffer.size(); i++){
		queue.push(elements[i]);
	}

	while (!queue.empty()){
		QueueElem headElement = queue.top(); // get top elem
		queue.pop(); // remove elem
		uint64_t value = *headElement.ptr; // value of elem
		output_buffer.push_back(value);

		/*
		 * if full
		 */
		if (output_buffer.size() == chunk_size){
			write(fdOutput, output_buffer.data(), output_buffer.size() * sizeof(uint64_t));
			output_buffer.clear();
		}

		headElement.index++;
		headElement.ptr++; // next element of this queue elem
		if (headElement.index == headElement.number_of_elements){
			//next chunk
			input_buffer[headElement.chunkNumber].clear();
			uint64_t  elemsToAdd = std::min(chunk_size, elements[headElement.chunkNumber].number_of_elements);
			elements[headElement.chunkNumber].number_of_elements -= elemsToAdd;
			input_buffer.resize(elemsToAdd);
			read(headElement.fd, &input_buffer[headElement.chunkNumber][0], elemsToAdd * sizeof(uint64_t));
			headElement.ptr = input_buffer[headElement.chunkNumber].begin();
			headElement.index = 0;
		}

		if (headElement.number_of_elements > 0){
			queue.push(headElement);
		}else{
			//done with this file
			close(headElement.fd);
			if (0 != remove(headElement.fileName.c_str())) {
				perror("Cannot remove temp file");
			}
		}
	}
	// at last, we need to flush the rest of the output-buffer as there might be something inside
	write(fdOutput, output_buffer.data(), output_buffer.size() * sizeof(uint64_t));
	output_buffer.clear();

	close(fdInput);
	close(fdOutput);
}


uint64_t div_ceil(uint64_t divisor, uint64_t dividend) {
	return 1 + ((divisor - 1) / dividend);
}