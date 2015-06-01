//
// Created by Martin on 31.05.2015.
//

#ifndef PROJECT_INNERNODEHEADER_HPP
#define PROJECT_INNERNODEHEADER_HPP

#include <stddef.h>

struct InnerNodeHeader {
  size_t keyCount;

  InnerNodeHeader() : keyCount(0) { }
};

#endif //PROJECT_INNERNODEHEADER_HPP
