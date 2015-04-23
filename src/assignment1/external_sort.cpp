#include <stdexcept>
#include <io.h>
#include <stdio.h>
#include <algorithm>
#include <fcntl.h>
#include <sys/stat.h>
#include <queue>
#include "external_sort.h"
#include "sorted_check.h"

#define DEBUG 1

struct QueueElem {
	unsigned int chunkNumber;
	uint64_t number_of_elements_not_in_buffer;
	uint64_t number_of_elements_in_buffer;
	std::vector<uint64_t>::iterator current_buffer_iterator;
	unsigned int index = 0;
	int fd = 0;
};

struct CompareQueuePrio {
	bool operator()(const QueueElem &lhs, const QueueElem &rhs) const {
		return *lhs.current_buffer_iterator < *rhs.current_buffer_iterator;
	}
};

uint64_t div_ceil(uint64_t divisor, uint64_t dividend);


int write_chunk(std::string fileprefix, unsigned int i, std::vector<uint64_t> data);

std::string getTempFileName(std::string &tempFilePrefix, unsigned int i);

void external_sort(int fdInput, uint64_t number_of_elements, int fdOutput, uint64_t mem_size_mb) {
	uint64_t mem_size_byte = mem_size_mb * 1024 * 1024;
	uint64_t max_elements_in_memory = mem_size_byte / sizeof(uint64_t);
	uint64_t elements_total_required_byte = number_of_elements * sizeof(uint64_t);
	uint64_t number_of_chunks = div_ceil(elements_total_required_byte,
										 mem_size_byte); // number of chunks the sorting requires

	std::vector<uint64_t> read_buffer;

	std::vector<QueueElem> elements;

	printf("Chunk phase with %llu chunks\n", number_of_chunks);
	// steps 1 - 3
	std::string tempFileDir = "temp";
	if (0 != mkdir(tempFileDir.c_str()) && errno != EEXIST) {
		perror("Cannot create temp dir");
		return;
	}
	std::string tempFilePrefix = tempFileDir + "/";

	uint64_t consumed_elements = 0;

	for (unsigned int i(0); i < number_of_chunks; i++) {
		// check how many elements we want to read as the last element might have less elements than the previous one
		uint64_t elements_left_to_read = number_of_elements - consumed_elements;
		uint64_t elements_to_consume = std::min(max_elements_in_memory, elements_left_to_read);
		read_buffer.resize((unsigned int) elements_to_consume);

		// read chunk
		uint64_t bytes_to_read = elements_to_consume * sizeof(uint64_t);
		int bytes_read = read(fdInput, &read_buffer[0], (unsigned int) bytes_to_read);
		if (bytes_read < 0) {
			perror("Cannot read input");
			return;
		}
		if (bytes_read != bytes_to_read) { // make sure everything got read
			fprintf(stderr, "Expected %llu bytes to be read, but got %d\n", bytes_to_read, bytes_read);
//			return;
		}
		consumed_elements += elements_to_consume;
		// sort chunk
		std::sort(read_buffer.begin(), read_buffer.begin() + elements_to_consume); // read_buffer.end());

		// write chunk to output file
		int fdTemp = write_chunk(tempFilePrefix, i, read_buffer);
		if (fdTemp < 0) {
			fprintf(stderr, "Cannot write chunk\n");
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
		elem.number_of_elements_not_in_buffer = elements_to_consume;
		elements.push_back(elem);
		read_buffer.clear();
	}

	// allocate output buffer
	printf("Merge phase (%llu-way)\n", number_of_chunks);

	std::vector<std::vector<uint64_t>> input_buffers;
	std::vector<uint64_t> output_buffer;

	uint64_t max_byte_per_buffer = mem_size_byte / (number_of_chunks + 1 /* 1 output buffer */);
	uint64_t max_elements_per_buffer = max_byte_per_buffer / sizeof(uint64_t);

	// read data in buffer
	for (unsigned int i(0); i < number_of_chunks; i++) {
		std::vector<uint64_t> buffer_elements;
		uint64_t elements_to_add = std::min(max_elements_per_buffer, elements[i].number_of_elements_not_in_buffer);
		buffer_elements.resize(elements_to_add);

		read(elements[i].fd, &buffer_elements[0], elements_to_add * sizeof(uint64_t));
		elements[i].number_of_elements_not_in_buffer -= elements_to_add;
		elements[i].number_of_elements_in_buffer = elements_to_add;
		input_buffers.push_back(buffer_elements);
		elements[i].current_buffer_iterator = input_buffers[i].begin(); // buffer_elements.begin() would go out of scope!
	}

	// k-way merge using a priority queue
	std::priority_queue<QueueElem, std::vector<QueueElem>, CompareQueuePrio> queue;
	for (unsigned int i(0); i < input_buffers.size(); i++) {
		queue.push(elements[i]);
	}

	while (!queue.empty()) {
		QueueElem headElement = queue.top(); // get top elem
		queue.pop(); // remove elem
		uint64_t value = *headElement.current_buffer_iterator; // value of elem
		headElement.current_buffer_iterator++; // next element of this queue elem
		output_buffer.push_back(value);

		if (output_buffer.size() == max_elements_per_buffer) {
			write(fdOutput, output_buffer.data(), output_buffer.size() * sizeof(uint64_t));
			output_buffer.clear();
		}

		headElement.index += 1;
		// reload next part of chunk into buffer if necessary
		if (headElement.index == headElement.number_of_elements_in_buffer) {
			if (headElement.number_of_elements_not_in_buffer > 0) { //elements left
				// next chunk
				input_buffers[headElement.chunkNumber].clear();
				uint64_t elements_to_add = std::min(max_elements_per_buffer,
													elements[headElement.chunkNumber].number_of_elements_not_in_buffer);
				headElement.number_of_elements_not_in_buffer -= elements_to_add;
				headElement.number_of_elements_in_buffer = elements_to_add;
				input_buffers[headElement.chunkNumber].resize(elements_to_add);
				read(headElement.fd, &input_buffers[headElement.chunkNumber][0],
					 elements_to_add * sizeof(uint64_t));
				headElement.current_buffer_iterator = input_buffers[headElement.chunkNumber].begin();
				headElement.index = 0;
			} else {
			}
		} else { // elements left
			queue.push(headElement); // add back to the queue
		}
	}

	// at last, we need to flush the rest of the output-buffer as there might be something inside
	write(fdOutput, output_buffer.data(), output_buffer.size() * sizeof(uint64_t));
	output_buffer.clear();

	// clean up
	close(fdInput);
	close(fdOutput);
	for (unsigned int i(0); i < number_of_chunks; i++) {
		close(elements[i].fd);
		std::string tempName = getTempFileName(tempFilePrefix, i);
		if (0 != remove(tempName.c_str())) {
			perror("Cannot remove temp file");
		}
	}
	if (0 != remove(tempFileDir.c_str())) {
		perror("Cannot remove temp dir");
	}
}

int write_chunk(std::string fileprefix, unsigned int i, std::vector<uint64_t> data) {
	std::string fdsTempName = getTempFileName(fileprefix, i);
	int fdTemp = open(fdsTempName.c_str(), O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	if (fdTemp < 0) {
		std::string msg = "Cannot open temp file ";
		msg = msg + fdsTempName;
		perror(msg.c_str());
		return -1;
	}
	int bytes_written = write(fdTemp, data.data(), data.size() * sizeof(uint64_t));

	if (bytes_written < 0) {
		return -1;
	}
	lseek(fdTemp, 0, SEEK_SET);
	close(fdTemp); // flush
	fdTemp = open(fdsTempName.c_str(), O_RDONLY, S_IREAD);
	return fdTemp;
}

std::string getTempFileName(std::string &tempFilePrefix, unsigned int i) {
	std::string tempName;
	char iteratorString[21]; // every 64 bit integer will fit in here
	sprintf(iteratorString, "%d", i);
	tempName = tempFilePrefix + iteratorString;
	return tempName;
}

uint64_t div_ceil(uint64_t divisor, uint64_t dividend) {
	return 1 + ((divisor - 1) / dividend);
}