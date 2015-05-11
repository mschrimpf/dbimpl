//
// Created by Martin on 17.04.2015.
//

#ifndef PROJECT_EXTERNAL_SORT_H
#define PROJECT_EXTERNAL_SORT_H

#include <stdint.h>
#include <stdio.h>
#include <algorithm>
#include <vector>

// Performs an external_sort on the 64-bit integers stored in the input file.
// The result is written to the output file.
// Closes input and output file descriptors once finished.
void external_sort(int fdInput, uint64_t number_of_elements, int fdOutput, uint64_t mem_size_mb);

int write_chunk(std::string fileprefix, unsigned int i, std::vector<uint64_t> data);

std::string getTempFileName(std::string &tempFilePrefix, unsigned int i);

#endif //PROJECT_EXTERNAL_SORT_H
