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
	uint64_t number_of_elements;
	std::vector<uint64_t>::iterator ptr;
	unsigned int index = 0;
	int fd = 0;
};

struct CompareQueuePrio {
	bool operator()(const QueueElem &lhs, const QueueElem &rhs) const {
		return *lhs.ptr > *rhs.ptr;
	}
};

uint64_t div_ceil(uint64_t divisor, uint64_t dividend);


int write_chunk(std::string fileprefix, unsigned int i, std::vector<uint64_t> data);

void external_sort(int fdInput, uint64_t number_of_elements, int fdOutput, uint64_t mem_size_mb) {
	uint64_t mem_size_byte = mem_size_mb * 1024 * 1024;
	uint64_t max_elements_in_memory = mem_size_byte / sizeof(uint64_t);
	uint64_t elements_byte = number_of_elements * sizeof(uint64_t);
	uint64_t number_of_chunks = div_ceil(elements_byte, mem_size_byte); // number of chunks the sorting requires

	std::vector<uint64_t> read_buffer;

	std::vector<QueueElem> elements;

	printf("Chunk phase with %llu chunks\n", number_of_chunks);
	// steps 1 - 3
	std::string tempFileDir = "temp";
	if (0 != mkdir(tempFileDir.c_str())) {
		perror("Cannot create temp dir");
	}
	std::string tempFilePrefix = tempFileDir + "/";

	uint64_t consumed_elements = 0;

	for (unsigned int i(0); i < number_of_chunks; i++) {
		// check how many elements we want to read as the last element might have less elements than the previous one
		uint64_t elements_left_to_read = number_of_elements - consumed_elements;
		uint64_t elements_to_consume = std::min(max_elements_in_memory, elements_left_to_read);
		read_buffer.resize(elements_to_consume);

		// read chunk
		int bytes_read = read(fdInput, &read_buffer[0], elements_to_consume * sizeof(uint64_t));
		if (bytes_read < 0) {
			perror("Cannot read input");
			return;
		}
		if(read_buffer.size() != elements_to_consume) { // just to be sure a.k.a. defensive programming
			fprintf(stderr, "sizes do not match\n");
		}
		consumed_elements += elements_to_consume;
		// sort chunk
		std::sort(read_buffer.begin(), read_buffer.end());

		// write chunk to output file
		int fdTemp = write_chunk(tempFilePrefix, i, read_buffer);
		if(fdTemp < 0) {
			return;
		}
#ifdef DEBUG
		printf("Check sorting of temporary file chunk %d...", i);
		if (check_sorting(fdTemp, elements_to_consume)) {
			printf(" OK.\n");
		} else {
			printf(" ERROR!\n");
		}
#endif
		QueueElem elem;
		elem.fd = fdTemp;
		elem.chunkNumber = i;
		elem.number_of_elements = elements_to_consume;
		elements.push_back(elem);
		read_buffer.clear();
	}

	// allocate output buffer
	printf("Merge phase (%llu-way)\n", number_of_chunks);

	std::vector<std::vector<uint64_t>> input_buffer;
	std::vector<uint64_t> output_buffer;

	uint64_t byte_per_buffer = mem_size_byte / (number_of_chunks + 1 /* 1 output buffer */);
	uint64_t elements_per_buffer = byte_per_buffer / sizeof(uint64_t);

	// read data in buffer
	for (unsigned int i(0); i < number_of_chunks; i++) {
		std::vector<uint64_t> buffer_elements;
		uint64_t elems_to_add = std::min(elements_per_buffer, elements[i].number_of_elements);
		buffer_elements.resize(elems_to_add);
		elements[i].number_of_elements -= elems_to_add;
		read(elements[i].fd, &buffer_elements[0], elems_to_add * sizeof(uint64_t));
		elements[i].ptr = buffer_elements.begin();
		input_buffer.push_back(buffer_elements);
	}

	// k-way merge
	// prio-queue
	// iterate through every input-buffer, enqueue (nextNumberIn(f), f)

	std::priority_queue<QueueElem, std::vector<QueueElem>, CompareQueuePrio> queue;
	for (unsigned int i(0); i < input_buffer.size(); i++) {
		queue.push(elements[i]);
	}

	while (!queue.empty()) {
		QueueElem headElement = queue.top(); // get top elem
		queue.pop(); // remove elem
		uint64_t value = *headElement.ptr; // value of elem
		output_buffer.push_back(value);

		if (output_buffer.size() == elements_per_buffer) {
			write(fdOutput, output_buffer.data(), output_buffer.size() * sizeof(uint64_t));
			output_buffer.clear();
		}

		headElement.index += 1;
		headElement.ptr++; // next element of this queue elem
		if (headElement.index == headElement.number_of_elements) {
			//next chunk
			input_buffer[headElement.chunkNumber].clear();
			uint64_t elemsToAdd = std::min(elements_per_buffer, elements[headElement.chunkNumber].number_of_elements);
			headElement.number_of_elements -= elemsToAdd;
			input_buffer.resize(elemsToAdd);
			read(headElement.fd, &input_buffer[headElement.chunkNumber][0], elemsToAdd * sizeof(uint64_t));
			headElement.ptr = input_buffer[headElement.chunkNumber].begin();
			headElement.index = 0;
		}

		if (headElement.number_of_elements > 0) {
			queue.push(headElement);
		} else {
			//done with this file
			close(headElement.fd);
//			if (0 != remove(headElement.fileName.c_str())) { // TODO: delete
//				perror("Cannot remove temp file");
//			}
		}
	}
	// at last, we need to flush the rest of the output-buffer as there might be something inside
	write(fdOutput, output_buffer.data(), output_buffer.size() * sizeof(uint64_t));
	output_buffer.clear();

	close(fdInput);
	close(fdOutput);
	// TODO: close and delete temp files
}

int write_chunk(std::string fileprefix, unsigned int i, std::vector<uint64_t> data) {
	std::string fdsTempName;
	char iteratorString[21]; // every 64 bit integer will fit in here
	sprintf(iteratorString, "%d", i);
	fdsTempName = fileprefix + iteratorString;
	int fdTemp = open(fdsTempName.c_str(), O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	if (fdTemp < 0) {
		std::string msg = "Cannot open temp file ";
		msg = msg + fdsTempName;
		perror(msg.c_str());
		return -1;
	}
	int w = write(fdTemp, data.data(), data.size() * sizeof(uint64_t));

	if (w < 0) {
		perror("Cannot write chunk");
		return -1;
	}
	return fdTemp;
}


uint64_t div_ceil(uint64_t divisor, uint64_t dividend) {
	return 1 + ((divisor - 1) / dividend);
}