//
// Created by Martin on 23.04.2015.
//

#include "util.h"

uint64_t div_ceil(uint64_t divisor, uint64_t dividend) {
	return 1 + ((divisor - 1) / dividend);
}