#include "Math.h"

uint64_t div_ceil(uint64_t divisor, uint64_t dividend) {
	return 1 + ((divisor - 1) / dividend);
}