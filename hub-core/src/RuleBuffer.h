/*
 * RuleBuffer.h
 *
 *  Created on: 8 Apr 2016
 *      Author: flo
 */

#ifndef RULEBUFFER_H_
#define RULEBUFFER_H_

#include <deque>
#include <mutex>
#include "Rule.h"
#include <condition_variable>

class RuleBuffer {
private:
	std::deque<Rule> ruleBuffer;
    unsigned int bufferSize;
    std::mutex mtx;
    std::condition_variable cv;
public:
	RuleBuffer(int inBufferSize);
    void add(Rule inRule);
    Rule remove();
    bool isFull();
	virtual ~RuleBuffer();
};

#endif /* RULEBUFFER_H_ */
