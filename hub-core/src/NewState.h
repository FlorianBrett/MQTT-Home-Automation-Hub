/*
 * NewState.h
 *dd
 *  Created on: 15 Mar 2016
 *      Author: flo
 */

#ifndef NEWSTATE_H_
#define NEWSTATE_H_
#include <string>
#include "MQTTMessage.h"
#include "Rule.h"
#include <vector>
#include "spdlog/spdlog.h"

class NewState {
private:
	std::shared_ptr<spdlog::logger> logger;
	std::string device;
	std::string field;
	std::string value;
	std::vector<Rule> rules;

	bool checkStateChange();
	//TODO RULE queue
public:
	NewState(MQTTMessage inMessage, MQTTMessageBuffer *outBufferPointer);
	virtual ~NewState();
};

#endif /* NEWSTATE_H_ */
