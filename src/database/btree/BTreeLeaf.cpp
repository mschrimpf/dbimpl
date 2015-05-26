//
// Created by Martin on 2015-05-27
//

#include <sstream>
#include <iostream>
#include "BTree.hpp"

bool BTree::Leaf::shouldSplit() {
	return header.count > maxLeafCapacity;
}
