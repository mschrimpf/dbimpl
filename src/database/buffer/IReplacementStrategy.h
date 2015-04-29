//
// Created by Martin on 29.04.2015.
//

#ifndef PROJECT_IREPLACEMENT_STRATEGY_H
#define PROJECT_IREPLACEMENT_STRATEGY_H

#include "BufferFrame.hpp"

class IReplacementStrategy {
public:
	virtual void push(BufferFrame * frame);
	virtual void onUse(BufferFrame * frame);
	virtual BufferFrame * pop();
};

#endif //PROJECT_IREPLACEMENT_STRATEGY_H
