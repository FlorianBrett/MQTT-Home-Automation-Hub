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
	int sleepTime;
	RuleBuffer *ruleBuffer;
	MQTTMessageBuffer *outBuffer;
public:
	RuleTimer(int inSleepTime,RuleBuffer *ruleBufferPointer,MQTTMessageBuffer *outBufferPointer);
	void startTimer();
	void ruleBufferResolution();
	virtual ~RuleTimer();
};

#endif /* TIMER_H_ */
