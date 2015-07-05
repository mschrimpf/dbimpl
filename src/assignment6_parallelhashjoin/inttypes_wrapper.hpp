#ifndef PROJECT_INTTYPES_WRAPPER_HPP
#define PROJECT_INTTYPES_WRAPPER_HPP

#ifdef _WIN32
#define PRIu64 "llu"
#else
#include <inttypes.h>
#endif

#endif //PROJECT_INTTYPES_WRAPPER_HPP
