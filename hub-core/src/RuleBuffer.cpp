/*
 * RuleBuffer.cpp
 *
 *  Created on: 8 Apr 2016
 *      Author: flo
 */

#include "RuleBuffer.h"

RuleBuffer::RuleBuffer(int inBufferSize) {
	bufferSize = inBufferSize;
}
void RuleBuffer::add(Rule inRule)
{
	std::unique_lock<std::mutex> lck (mtx);
	while (ruleBuffer.size() >= bufferSize) cv.wait(lck);
	ruleBuffer.push_back(inRule);
	std::cout <<"Rule Buffer add Size:" << ruleBuffer.size() << '\n';
	lck.unlock();
	cv.notify_all();
}
Rule RuleBuffer::remove()
{
	std::unique_lock<std::mutex> lck (mtx);
	while (ruleBuffer.size() <= 0) cv.wait(lck);
	Rule outRule = ruleBuffer.back();
	ruleBuffer.pop_back();
	std::cout << "Rule Buffer remove Size:" << ruleBuffer.size() << '\n';
	lck.unlock();
	cv.notify_all();
	return outRule;
}
bool RuleBuffer::isFull()
{
	bool full = false;
	if (ruleBuffer.size() >= bufferSize)
		full = true;
	return full;
}
RuleBuffer::~RuleBuffer() {
	// TODO Auto-generated destructor stub
}

