//
// Created by Martin on 17.04.2015.
//

#ifndef PROJECT_EXTERNAL_SORT_H
#define PROJECT_EXTERNAL_SORT_H

#include <stdint.h>

// Performs an external_sort on the 64-bit integers stored in the input file.
// The result is written to the output file.
// Closes input and output file descriptors once finished.
void external_sort(int fdInput, uint64_t size, int fdOutput, uint64_t memSize);

#endif //PROJECT_EXTERNAL_SORT_H
