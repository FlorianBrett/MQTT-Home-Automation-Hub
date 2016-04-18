/*
 * Timer.h
 *
 *  Created on: 7 Apr 2016
 *      Author: flo
 */

#ifndef TIMER_H_
#define TIMER_H_
#include "RuleBuffer.h"
class RuleTimer {
private:
	int normalSleepTime;
	RuleBuffer *ruleBuffer;
	MQTTMessageBuffer *outBuffer;
	std::shared_ptr<spdlog::logger> logger;
public:
	RuleTimer(int inSleepTime,RuleBuffer *ruleBufferPointer,MQTTMessageBuffer *outBufferPointer);
	void startTimer();
	void ruleBufferResolution();
	virtual ~RuleTimer();
};

#endif /* TIMER_H_ */
